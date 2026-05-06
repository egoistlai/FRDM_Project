/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "gui_guider.h"

    void custom_init(lv_ui *ui);

    void btn_scan_clicked_event_handler(lv_event_t *e);
    void btn_connect_clicked_event_handler(lv_event_t *e);
    void wifi_conn_done_callback(void *result);

    void stop_camera(lv_obj_t *img_obj);
    void capture_frame();

    void *video_tx_thread_func(void *arg);
    void *cmd_rx_thread_func(void *arg);
    void connect_to_server(const char *ip, int port);
    void set_transmission(bool enable);
    void toggle_transmission();

    void start_phone_monitor(lv_obj_t *img_obj);
    void *phone_rx_thread_func(void *arg);

    void app_cleanup(int signo);

    void start_camera_backend();
    void toggle_local_render(lv_obj_t *img_obj, bool enable);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
