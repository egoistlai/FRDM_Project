/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "gui_guider.h"
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

// 定义传递给线程的参数
typedef struct
{
    char ssid[128];
    char password[128];
} wifi_conn_params_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

lv_obj_t *wifi_spinner = NULL; // 全局加载动画指针
char selected_ssid[128] = "";

static FILE *gst_fp = NULL;
static int gst_fd = -1;
static lv_timer_t *cam_timer = NULL;
static uint32_t bytes_read = 0;
static int debug_frame_count = 0;

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define FRAME_BPP 2
#define FRAME_SIZE (FRAME_WIDTH * FRAME_HEIGHT * FRAME_BPP)

static uint8_t cam_buf[FRAME_SIZE];

static lv_img_dsc_t cam_img_dsc = {
    .header.always_zero = 0,
    .header.w = FRAME_WIDTH,
    .header.h = FRAME_HEIGHT,
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .data_size = FRAME_SIZE,
    .data = cam_buf};

// 全局 Socket 变量
static int server_socket = -1;
static bool is_transmitting = false;
static pthread_t tx_thread_id;

// 服务器参数预留
char target_ip[32] = "100.114.174.61";
int target_port = 9999;

#define PHONE_WIDTH 480
#define PHONE_HEIGHT 320
#define PHONE_SIZE (PHONE_WIDTH * PHONE_HEIGHT * 2)

static uint8_t phone_buf[PHONE_SIZE];
static lv_img_dsc_t phone_img_dsc = {
    .header.always_zero = 0,
    .header.w = PHONE_WIDTH,
    .header.h = PHONE_HEIGHT,
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .data_size = PHONE_SIZE,
    .data = phone_buf};

// 全局记录手机画面的 UI 控件指针
static lv_obj_t *phone_ui_img_ptr = NULL;

static int phone_socket = -1;
static pthread_t phone_rx_thread_id;

static int cmd_socket = -1;
static pthread_t cmd_rx_thread_id;

// ================= 新增防撕裂锁和缓存 =================
static pthread_mutex_t cam_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint8_t local_send_buf[FRAME_SIZE]; // 专门用于发送的独立缓存
static bool is_first_frame_ready = false;

bool is_local_rendering = false;        // 控制是否刷新本地屏幕
static lv_obj_t *cam_ui_img_ptr = NULL; // 记录屏幕上的图像控件指针
/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    // 拦截 Ctrl+C 信号
    signal(SIGINT, app_cleanup);
    // 拦截 kill 命令产生的终止信号
    signal(SIGTERM, app_cleanup);
    // 拦截网络异常断开可能引发的管道破裂信号 (防止程序意外闪退)
    signal(SIGPIPE, SIG_IGN);

    start_camera_backend();
}

void list_item_clicked_event_handler(lv_event_t *e)
{
    // 1. 获取当前触发事件的对象（即用户点击的那个具体的列表项按钮）
    lv_obj_t *clicked_btn = lv_event_get_target(e);
    // 2. 获取列表对象容器 (注意替换为你 GUI Guider 里实际的变量名)
    lv_obj_t *list = guider_ui.screen_wifi_list_1;
    // 3. 调用 LVGL 内置函数，提取该按钮上的文本 (即 WiFi SSID)
    const char *txt = lv_list_get_btn_text(list, clicked_btn);
    if (txt != NULL)
    {
        // 4. 将 WiFi 名字安全地拷贝到我们的全局变量中
        strncpy(selected_ssid, txt, sizeof(selected_ssid) - 1);
        selected_ssid[sizeof(selected_ssid) - 1] = '\0'; // 确保字符串正确结束
        // 5. (可选) 给用户一个友好的 UI 反馈
        char status_msg[256];
        snprintf(status_msg, sizeof(status_msg), "已选择: %s, 请输入密码", selected_ssid);
        lv_label_set_text(guider_ui.screen_wifi_label_1, status_msg);
    }
}

// 这是一个运行在后台的函数
void *wifi_connect_thread_func(void *arg)
{
    wifi_conn_params_t *params = (wifi_conn_params_t *)arg;
    char cmd[512];

    // 拼装命令 (同你之前的逻辑)
    snprintf(cmd, sizeof(cmd),
             "wpa_cli -i mlan0 remove_network all && "
             "wpa_cli -i mlan0 add_network && "
             "wpa_cli -i mlan0 set_network 0 ssid '\"%s\"' && "
             "wpa_cli -i mlan0 set_network 0 psk '\"%s\"' && "
             "wpa_cli -i mlan0 enable_network 0 && "
             "wpa_cli -i mlan0 save_config && "
             "dhclient -r mlan0 && dhclient -i mlan0",
             params->ssid, params->password);

    // 执行阻塞操作
    int ret = system(cmd);

    // 【重要】操作完成后，不能直接在这里删 UI (LVGL 非线程安全)
    // 我们使用 lv_async_call 通知主线程来处理结果
    if (ret == 0)
    {
        lv_async_call((void (*)(void *))wifi_conn_done_callback, (void *)1);
    }
    else
    {
        lv_async_call((void (*)(void *))wifi_conn_done_callback, (void *)0);
    }

    free(params); // 释放内存
    return NULL;
}

// 这个函数由 LVGL 主线程调用，是安全的
void wifi_conn_done_callback(void *result)
{
    // 1. 销毁加载动画
    if (wifi_spinner != NULL)
    {
        lv_obj_del(wifi_spinner);
        wifi_spinner = NULL;
    }

    // 2. 更新状态文字
    if ((intptr_t)result == 1)
    {
        lv_label_set_text(guider_ui.screen_wifi_label_1, "连接成功!");
        system("sudo tailscale up &");
    }
    else
    {
        lv_label_set_text(guider_ui.screen_wifi_label_1, "连接失败，请检查密码");
    }
}

void btn_connect_clicked_event_handler(lv_event_t *e)
{
    // 1. 创建加载动画 (Spinner)
    if (wifi_spinner == NULL)
    {
        wifi_spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
        lv_obj_set_size(wifi_spinner, 100, 100);
        lv_obj_center(wifi_spinner);
    }

    lv_label_set_text(guider_ui.screen_wifi_label_1, "正在后台连接...");

    // 2. 准备参数
    wifi_conn_params_t *params = malloc(sizeof(wifi_conn_params_t));
    strncpy(params->ssid, selected_ssid, 127);
    strncpy(params->password, lv_textarea_get_text(guider_ui.screen_wifi_ta_1), 127);

    // 3. 创建子线程
    pthread_t tid;
    pthread_create(&tid, NULL, wifi_connect_thread_func, params);
    pthread_detach(tid); // 线程结束后自动回收资源
}

void btn_scan_clicked_event_handler(lv_event_t *e)
{
    lv_obj_t *list = guider_ui.screen_wifi_list_1;
    lv_obj_clean(list);
    lv_label_set_text(guider_ui.screen_wifi_label_1, "正在扫描 WiFi...");
    // 1. 发送扫描指令
    system("wpa_cli -i mlan0 scan");
    // 给你 1 秒钟的硬件扫描时间 (如果在 LVGL 中，最好用定时器，这里用 system sleep 简略代替)
    system("sleep 1");
    // 2. 获取扫描结果，使用 awk 提取第 5 列 (SSID)，忽略空行和标题头
    FILE *fp;
    char buffer[128];
    fp = popen("wpa_cli -i mlan0 scan_results | awk -F'\\t' 'NR>1 {print $5}' | sort | uniq", "r");
    if (fp == NULL)
    {
        lv_label_set_text(guider_ui.screen_wifi_label_1, "扫描失败!");
        return;
    }
    // 3. 动态添加到 GUI 列表中
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        buffer[strcspn(buffer, "\r\n")] = 0; // 去除回车
        // 过滤掉隐藏的或者空的 SSID
        if (strlen(buffer) > 0)
        {
            lv_obj_t *list_btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, buffer);
            lv_obj_add_event_cb(list_btn, list_item_clicked_event_handler, LV_EVENT_CLICKED, NULL);
        }
    }
    pclose(fp);
    lv_label_set_text(guider_ui.screen_wifi_label_1, "扫描完成，请选择");
}

// 定时器回调：高频拉取，低频刷新
static void cam_refresh_cb(lv_timer_t *timer)
{
    if (gst_fd < 0)
        return;

    // 疯狂吸水：有数据就一直读，直到管道暂时空了
    while (bytes_read < FRAME_SIZE)
    {
        ssize_t ret = read(gst_fd, cam_buf + bytes_read, FRAME_SIZE - bytes_read);
        if (ret > 0)
        {
            bytes_read += ret;
        }
        else
        {
            break;
        }
    }

    if (bytes_read >= FRAME_SIZE)
    {
        // 【核心解耦 1】：无论是否渲染，永远给推流端准备最新画面的快照！
        pthread_mutex_lock(&cam_mutex);
        memcpy(local_send_buf, cam_buf, FRAME_SIZE);
        is_first_frame_ready = true;
        pthread_mutex_unlock(&cam_mutex);

        bytes_read = 0;
        debug_frame_count++;

        // 【核心解耦 2】：只有当 UI 开关打开时，才消耗 CPU 去重绘屏幕！
        if (is_local_rendering && cam_ui_img_ptr != NULL)
        {
            lv_img_cache_invalidate_src(&cam_img_dsc);
            lv_obj_invalidate(cam_ui_img_ptr);
        }
    }
}

// 【新函数】：程序一启动就在后台默默打开摄像头硬件
void start_camera_backend()
{
    if (gst_fp != NULL)
        return;

    printf("[CAM INFO] 正在后台启动摄像头底层流水线...\n");
    bytes_read = 0;

    const char *cmd = "gst-launch-1.0 -q v4l2src device=/dev/video2 ! "
                      "image/jpeg,width=640,height=480 ! jpegdec ! queue ! "
                      "videoconvert ! video/x-raw,format=RGB16 ! queue ! "
                      "videorate ! video/x-raw,framerate=15/1 ! fdsink sync=false";

    gst_fp = popen(cmd, "r");
    if (gst_fp)
    {
        gst_fd = fileno(gst_fp);
        int flags = fcntl(gst_fd, F_GETFL, 0);
        fcntl(gst_fd, F_SETFL, flags | O_NONBLOCK);

        // 创建定时器，但不依赖任何 UI 控件
        if (cam_timer == NULL)
        {
            cam_timer = lv_timer_create(cam_refresh_cb, 5, NULL);
        }
    }
}

void stop_camera(lv_obj_t *img_obj)
{
    if (cam_timer)
    {
        lv_timer_del(cam_timer);
        cam_timer = NULL;
    }

    if (gst_fp != NULL)
    {
        pclose(gst_fp);
        gst_fp = NULL;
        gst_fd = -1;
    }

    lv_img_set_src(img_obj, NULL);
}

// 【新函数】：仅仅控制是否将画面绘制到屏幕上
void toggle_local_render(lv_obj_t *img_obj, bool enable)
{
    is_local_rendering = enable;
    cam_ui_img_ptr = img_obj;

    if (enable)
    {
        printf("[UI INFO] 已开启开发板本地画面渲染\n");
        // 挂载数据源
        lv_img_set_src(img_obj, &cam_img_dsc);
    }
    else
    {
        printf("[UI INFO] 已关闭开发板本地画面渲染，进入省电模式\n");
        // 卸载数据源，屏幕画面会变为空白/变黑
        lv_img_set_src(img_obj, NULL);
    }
}

void capture_frame()
{
    // 如果还没启动摄像头，或者当前没有有效数据，则不拍照
    if (gst_fd < 0 || cam_buf == NULL)
    {
        printf("[CAM ERROR] 摄像头未开启，无法拍照！\n");
        return;
    }

    // 瞬间将内存中的当前帧拷贝并写入硬盘
    FILE *fp = fopen("/home/debian/gui/vedio/capture.raw", "wb");
    if (fp)
    {
        fwrite(cam_buf, 1, FRAME_SIZE, fp);
        fclose(fp);
        printf("[CAM SUCCESS] 照片已瞬间保存为 RAW 数据！\n");
    }
    else
    {
        printf("[CAM ERROR] 无法写入文件路径\n");
    }
}

void *video_tx_thread_func(void *arg)
{
    while (is_transmitting && server_socket != -1)
    {
        if (is_first_frame_ready)
        {
            uint32_t data_len = FRAME_SIZE;

            // 1. 发送帧大小包头
            int head_sent = send(server_socket, &data_len, sizeof(data_len), MSG_NOSIGNAL);
            if (head_sent <= 0)
            {
                printf("[SERVER ERROR] Python中间件堵塞或断开，推流紧急熔断！\n");
                is_transmitting = false;
                close(server_socket);
                server_socket = -1;
                break;
            }

            // 2. 直接发送安全隔离好的 local_send_buf
            int total_sent = 0;
            while (total_sent < FRAME_SIZE && is_transmitting)
            {
                int sent = send(server_socket, local_send_buf + total_sent, FRAME_SIZE - total_sent, MSG_NOSIGNAL);
                if (sent <= 0)
                {
                    printf("[SERVER ERROR] 画面发送超时，网络拥塞，断开连接！\n");
                    is_transmitting = false;
                    close(server_socket);
                    server_socket = -1;
                    break;
                }
                total_sent += sent;
            }
        }

        // 推流引擎全开：40000 微秒 = 25 FPS 的检测频率
        usleep(40000);
    }
    return NULL;
}

void *cmd_rx_thread_func(void *arg)
{
    uint8_t cmd_byte;
    while (cmd_socket != -1)
    {
        int ret = recv(cmd_socket, &cmd_byte, 1, 0);
        if (ret > 0)
        {
            if (cmd_byte == 0x01) {
                set_transmission(true); // 收到 1，开启推流
            } else if (cmd_byte == 0x00) {
                set_transmission(false); // 收到 0，关闭推流
            }
        }
        else if (ret <= 0)
        {
            printf("[CMD ERROR] 本地信令通道断开\n");
            close(cmd_socket);
            cmd_socket = -1;
            break;
        }
    }
    return NULL;
}

void connect_to_server(const char *ip, int port)
{
    if (server_socket != -1)
        close(server_socket);
    if (cmd_socket != -1)
        close(cmd_socket);
    if (phone_socket != -1)
        close(phone_socket);

    system("pkill -f board_full_client.py");

    char cmd[512];
    // 启动我们刚重构好的本地路由中间件
    snprintf(cmd, sizeof(cmd),
             "bash -c 'source /home/debian/npu_env/bin/activate && python3 /home/debian/client/board_full_client.py %s' &",
             ip);

    printf("[SERVER INFO] 正在启动后台 AI 桥接与路由进程...\n");
    system(cmd);

    // 等待 Python 启动并连接远端
    usleep(3000000);

    // ==================================================
    // 1. 连接视频推流端口 (8888) 并【强制自动推流】
    // ==================================================
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &local_addr.sin_addr);

    if (connect(server_socket, (struct sockaddr *)&local_addr, sizeof(local_addr)) == 0)
    {
        printf("[SERVER SUCCESS] C 语言已成功连接到本地视频总线 (8888)!\n");

        // 【核心修改】：一旦连上 Python，立刻无条件开启底层推流！
        // 这样 NPU 就能随时拿到最新画面做跌倒检测，而不用等手机端请求
        is_transmitting = true;
        pthread_create(&tx_thread_id, NULL, video_tx_thread_func, NULL);
        pthread_detach(tx_thread_id);

        // 设置防死锁超时锁 (如果 Python 卡住，C 语言不至于死等)
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 500000;
        setsockopt(server_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv));
    }
    else
    {
        printf("[SERVER ERROR] 连接本地 Python 失败!\n");
        server_socket = -1;
    }

    // ==================================================
    // 2. 连接视频拉流端口 (8889)
    // ==================================================
    phone_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in rx_addr;
    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons(8889);
    inet_pton(AF_INET, "127.0.0.1", &rx_addr.sin_addr);

    if (connect(phone_socket, (struct sockaddr *)&rx_addr, sizeof(rx_addr)) == 0)
    {
        pthread_create(&phone_rx_thread_id, NULL, phone_rx_thread_func, NULL);
        pthread_detach(phone_rx_thread_id);
    }

    // ==================================================
    // 3. 连接信令端口 (8890)
    // ==================================================
    cmd_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in cmd_addr;
    cmd_addr.sin_family = AF_INET;
    cmd_addr.sin_port = htons(8890);
    inet_pton(AF_INET, "127.0.0.1", &cmd_addr.sin_addr);

    if (connect(cmd_socket, (struct sockaddr *)&cmd_addr, sizeof(cmd_addr)) == 0)
    {
        printf("[SERVER SUCCESS] 本地信令控制通道建立成功!\n");
        // 保留信令接收线程，为后续（比如接收服务器下发的其他指令）做预留
        pthread_create(&cmd_rx_thread_id, NULL, cmd_rx_thread_func, NULL);
        pthread_detach(cmd_rx_thread_id);
    }
}

void set_transmission(bool enable)
{
    if (enable && !is_transmitting && server_socket != -1)
    {
        is_transmitting = true;
        pthread_create(&tx_thread_id, NULL, video_tx_thread_func, NULL);
        pthread_detach(tx_thread_id);
        printf("[SERVER INFO] 收到手机端指令，已启动高速推流...\n");
    }
    else if (!enable && is_transmitting)
    {
        is_transmitting = false;
        printf("[SERVER INFO] 收到手机端指令，推流已挂起，进入低功耗待机...\n");
        // 注意：千万不要 close(server_socket)，只是让线程优雅退出，保持 socket 活着！
    }
}

// GUI 按钮的回调仍然可以使用这个（允许开发板端手动覆盖）
void toggle_transmission()
{
    // 现在的云端推流由手机 App 控制，C 语言只负责底层数据供应。
    // 为了防止误触导致底层管线崩溃，我们将此按钮修改为单纯的状态提示。
    if (is_transmitting)
    {
        printf("[SERVER INFO] 底层视频流正在稳定运行中。云端视频是否开启请在手机 App 上操作。\n");
    }
    else
    {
        printf("[SERVER WARNING] 底层尚未连接服务器，请先点击【连接服务器】。\n");
    }
}

// 【新增】绑定函数
void start_phone_monitor(lv_obj_t *img_obj)
{
    if (img_obj == NULL)
        return;

    phone_ui_img_ptr = img_obj;
    // 将图像控件的数据源设置为我们的接收缓冲区
    lv_img_set_src(img_obj, &phone_img_dsc);
    printf("[UI INFO] 手机画面控件绑定成功！\n");
}

static void refresh_phone_ui_cb(void *arg)
{
    if (phone_ui_img_ptr != NULL)
    {
        lv_img_cache_invalidate_src(&phone_img_dsc);
        lv_obj_invalidate(phone_ui_img_ptr);
    }
}

void *phone_rx_thread_func(void *arg)
{
    uint32_t bytes_read = 0;
    while (phone_socket != -1)
    {
        // 阻塞接收 Python 传来的 RGB565 裸流
        // 这里 recv 限定了最大读取量为 PHONE_SIZE - bytes_read，天然防越界防粘包
        int ret = recv(phone_socket, phone_buf + bytes_read, PHONE_SIZE - bytes_read, 0);
        if (ret > 0)
        {
            bytes_read += ret;
            // 凑齐一帧完整的画面后
            if (bytes_read >= PHONE_SIZE)
            {
                bytes_read = 0;
                // 【核心修复】：切勿在 pthread 中直接刷新 UI！
                // 使用 lv_async_call 委托给 LVGL 主线程，保证绝对的线程安全
                lv_async_call(refresh_phone_ui_cb, NULL);
            }
        }
        else if (ret <= 0)
        {
            printf("[RX ERROR] 手机画面数据流断开\n");
            close(phone_socket);
            phone_socket = -1;
            break;
        }
    }
    return NULL;
}

void app_cleanup(int signo)
{
    printf("\n[SYSTEM] 收到退出信号 (%d)，正在执行安全清理...\n", signo);

    // 1. 强行关闭摄像头 GStreamer 管道
    if (gst_fp != NULL)
    {
        printf("[SYSTEM] 正在释放摄像头设备节点...\n");
        // 如果有定时器，为了安全起见不去碰它，直接关底层的管道
        pclose(gst_fp);
        gst_fp = NULL;
        gst_fd = -1;
    }

    // 2. 关掉所有的 TCP Socket 链接
    if (cmd_socket != -1)
    {
        close(cmd_socket);
        cmd_socket = -1;
    }
    if (server_socket != -1)
    {
        close(server_socket);
        server_socket = -1;
    }
    if (phone_socket != -1)
    {
        close(phone_socket);
        phone_socket = -1;
    }

    // 3. 顺手把后台可能还在跑的 Python AI 进程也斩草除根
    printf("[SYSTEM] 正在清理后台 Python 桥接进程...\n");
    system("pkill -f board_full_client.py");

    printf("[SYSTEM] 资源释放完毕，程序安全退出！\n");
    exit(0); // 彻底结束程序
}