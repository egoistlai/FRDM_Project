/*
 * Copyright 2026 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "events_init.h"
#include <stdio.h>
#include <stdlib.h>
#include "lvgl.h"
#include "custom.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static void screen_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_vedio, guider_ui.screen_vedio_del, &guider_ui.screen_del, setup_scr_screen_vedio, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void screen_btn_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void screen_btn_3_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_settings, guider_ui.screen_settings_del, &guider_ui.screen_del, setup_scr_screen_settings, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void screen_btn_5_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_server, guider_ui.screen_server_del, &guider_ui.screen_del, setup_scr_screen_server, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_1, screen_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_2, screen_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_3, screen_btn_3_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_5, screen_btn_5_event_handler, LV_EVENT_ALL, ui);
}

static void screen_vedio_sw_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t *status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;

        /* 提取在 events_init_screen 中绑定的 ui 结构体指针 */
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);

        if (status)
        {
            /* 开关处于打开状态：开启摄像头 */
            start_camera(ui->screen_vedio_img_1);
        }
        else
        {
            /* 开关处于关闭状态：关闭摄像头，并清除画面残留 */
            /* 请确保 ui->screen_video_area 与你在 GUI Guider 中设置的容器名称一致。
               如果不叫 video_area，请修改为对应的 ui->screen_xxxx */
            stop_camera(ui->screen_vedio_img_1);
        }
        break;
    }
    default:
        break;
    }
}

static void screen_vedio_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  拍照  */
        capture_frame();
        break;
    }
    default:
        break;
    }
}

static void screen_vedio_btn_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_vedio_del, setup_scr_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_vedio(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_vedio_sw_1, screen_vedio_sw_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_vedio_btn_1, screen_vedio_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_vedio_btn_2, screen_vedio_btn_2_event_handler, LV_EVENT_ALL, ui);
}

static void screen_log_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  获取日志照片  */
        break;
    }
    default:
        break;
    }
}

static void screen_log_btn_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_log_del, setup_scr_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_log(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_log_btn_1, screen_log_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_log_btn_2, screen_log_btn_2_event_handler, LV_EVENT_ALL, ui);
}

static void screen_settings_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_wifi, guider_ui.screen_wifi_del, &guider_ui.screen_settings_del, setup_scr_screen_wifi, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_settings_btn_4_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_settings_del, setup_scr_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_settings(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_settings_btn_1, screen_settings_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_settings_btn_4, screen_settings_btn_4_event_handler, LV_EVENT_ALL, ui);
}

static void screen_wifi_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  扫描WIFI  */
        btn_scan_clicked_event_handler(e);
        break;
    }
    default:
        break;
    }
}

static void screen_wifi_btn_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  连接WIFI  */
        btn_connect_clicked_event_handler(e);
        break;
    }
    default:
        break;
    }
}

static void screen_wifi_btn_3_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_settings, guider_ui.screen_settings_del, &guider_ui.screen_wifi_del, setup_scr_screen_settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_wifi(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_wifi_btn_1, screen_wifi_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_wifi_btn_2, screen_wifi_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_wifi_btn_3, screen_wifi_btn_3_event_handler, LV_EVENT_ALL, ui);
}

static void screen_server_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  连接服务器  */
        const char *ip = lv_textarea_get_text(guider_ui.screen_server_ta_2);
        int port = atoi(lv_textarea_get_text(guider_ui.screen_server_ta_3));

        // 调用 custom.c 中的连接函数
        connect_to_server(ip, port);
        break;
    }
    default:
        break;
    }
}

static void screen_server_btn_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        /*  推流  */
        toggle_transmission();
        break;
    }
    default:
        break;
    }
}

static void screen_server_btn_3_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_server_del, setup_scr_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_server(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_server_btn_1, screen_server_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_server_btn_2, screen_server_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_server_btn_3, screen_server_btn_3_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{
}
