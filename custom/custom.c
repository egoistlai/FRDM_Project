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
/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
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
    lv_obj_t *img_obj = timer->user_data;

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
            break; // 没数据了，立刻退出，不卡死 GUI
        }
    }

    // 只有在真正凑齐一帧（614,400字节）时，才消耗 CPU 去重绘屏幕！
    if (bytes_read >= FRAME_SIZE)
    {
        bytes_read = 0;
        debug_frame_count++;

        // 强迫 LVGL 刷新
        lv_img_cache_invalidate_src(&cam_img_dsc);
        lv_obj_invalidate(img_obj);

        if (debug_frame_count % 30 == 0)
        {
            printf("[CAM DEBUG] 流畅渲染第 %d 帧...\n", debug_frame_count);
        }
    }
}

void start_camera(lv_obj_t *img_obj)
{
    if (gst_fp != NULL)
        return;

    printf("[CAM DEBUG] 准备启动流畅版 GStreamer...\n");
    bytes_read = 0;

    // 【核心性能优化】
    // 1. queue: 开启 GStreamer 内部多线程队列缓冲
    // 2. videorate ! ...framerate=15/1: 强制降为 15 帧，大幅降低总线和 CPU 压力
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

        lv_img_set_src(img_obj, &cam_img_dsc);

        // 【核心性能优化】将接水频率提高到 5ms 一次，消除画面撕裂和等待
        if (cam_timer == NULL)
        {
            cam_timer = lv_timer_create(cam_refresh_cb, 5, img_obj);
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
        if (cam_buf != NULL)
        {
            uint32_t data_len = FRAME_SIZE;

            // 新增打印：确认即将发送的大小
            printf("[BOARD DEBUG] 准备发送一帧，声明大小: %u bytes\n", data_len);

            send(server_socket, &data_len, sizeof(data_len), MSG_NOSIGNAL);

            int total_sent = 0;
            while (total_sent < FRAME_SIZE && is_transmitting)
            {
                int sent = send(server_socket, cam_buf + total_sent, FRAME_SIZE - total_sent, MSG_NOSIGNAL);
                if (sent <= 0)
                {
                    printf("[SERVER ERROR] 远端已断开，推流停止\n");
                    is_transmitting = false;
                    close(server_socket);
                    server_socket = -1;
                    break;
                }
                total_sent += sent;
            }
            // 新增打印：确认实际发送出去的大小
            printf("[BOARD DEBUG] 成功发送一帧，实际发出: %d bytes\n", total_sent);
        }
        usleep(66000);
    }
    return NULL;
}

void connect_to_server(const char *ip, int port)
{
    if (server_socket != -1)
    {
        close(server_socket);
    }

    // 1. 杀掉后台可能残留的旧进程（注意名字改成了 board_full_client.py）
    system("pkill -f board_full_client.py");

    // 2. 拼装带虚拟环境激活的启动命令
    // 使用 bash -c 确保 source 指令正常解析，并在最后加上 & 让其在后台运行
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "bash -c 'source /home/debian/npu_env/bin/activate && python3 /home/debian/client/board_full_client.py %s %d' &",
             ip, port);

    printf("[SERVER INFO] 正在启动后台 AI 桥接进程: %s\n", cmd);
    system(cmd);

    // 3. 给 Python 留出足够的启动时间（加载 NPU 环境和 YOLO 模型可能比较慢，这里延长到 3 秒）
    usleep(3000000);

    // 4. 连接到本地的 Python 服务端
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(8888); // Python 监听的本地端口
    inet_pton(AF_INET, "127.0.0.1", &local_addr.sin_addr);

    if (connect(server_socket, (struct sockaddr *)&local_addr, sizeof(local_addr)) == 0)
    {
        printf("[SERVER SUCCESS] C 语言已成功连接到本地 AI 中间件!\n");
    }
    else
    {
        printf("[SERVER ERROR] 连接本地 Python 失败! 请检查 NPU 虚拟环境或模型加载情况。\n");
        server_socket = -1;
    }

    phone_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in rx_addr;
    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons(8889);
    inet_pton(AF_INET, "127.0.0.1", &rx_addr.sin_addr);

    if (connect(phone_socket, (struct sockaddr *)&rx_addr, sizeof(rx_addr)) == 0)
    {
        printf("[SERVER SUCCESS] 收发双通道均已建立!\n");
        // 启动接收线程
        pthread_create(&phone_rx_thread_id, NULL, phone_rx_thread_func, NULL);
        pthread_detach(phone_rx_thread_id);
    }
}

void toggle_transmission()
{
    if (gst_fd < 0)
    {
        printf("[SERVER WARNING] 摄像头未启动！请先打开摄像头画面再推流。\n");
        return;
    }
    if (!is_transmitting && server_socket != -1)
    {
        is_transmitting = true;
        pthread_create(&tx_thread_id, NULL, video_tx_thread_func, NULL);
        pthread_detach(tx_thread_id);
        printf("[SERVER INFO] 开始向本地 AI 节点推流...\n");
    }
    else
    {
        is_transmitting = false;
        printf("[SERVER INFO] 停止推流，关闭 AI 节点...\n");

        // 【修改这里】停止推流时，杀掉正确的 Python 进程
        system("pkill -f board_full_client.py");
        if (server_socket != -1)
        {
            close(server_socket);
            server_socket = -1;
        }
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

void *phone_rx_thread_func(void *arg)
{
    uint32_t bytes_read = 0;
    while (phone_socket != -1)
    {
        // 阻塞接收 Python 传来的 RGB565 裸流
        int ret = recv(phone_socket, phone_buf + bytes_read, PHONE_SIZE - bytes_read, 0);
        if (ret > 0)
        {
            bytes_read += ret;
            // 凑齐一帧 320*240*2 = 153600 字节后刷新 UI
            if (bytes_read >= PHONE_SIZE)
            {
                bytes_read = 0;
                if (phone_ui_img_ptr != NULL)
                {
                    lv_img_cache_invalidate_src(&phone_img_dsc);
                    lv_obj_invalidate(phone_ui_img_ptr);
                }
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