/*
 * Copyright 2026 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

void setup_scr_screen_log(lv_ui *ui)
{
    LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18);

    // Write codes screen_log
    ui->screen_log = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_log, 1024, 600);
    lv_obj_set_scrollbar_mode(ui->screen_log, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_log, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_log, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_log_img_1
    ui->screen_log_img_1 = lv_img_create(ui->screen_log);
    lv_obj_add_flag(ui->screen_log_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_pivot(ui->screen_log_img_1, 50, 50);
    lv_img_set_angle(ui->screen_log_img_1, 0);
    lv_obj_set_pos(ui->screen_log_img_1, 21, 58);
    lv_obj_set_size(ui->screen_log_img_1, 640, 480);

    // Write style for screen_log_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_log_img_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_log_img_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_img_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_log_img_1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_log_list_1
    ui->screen_log_list_1 = lv_list_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_list_1, 688, 98);
    lv_obj_set_size(ui->screen_log_list_1, 176, 395);
    lv_obj_set_scrollbar_mode(ui->screen_log_list_1, LV_SCROLLBAR_MODE_OFF);

    // Write style state: LV_STATE_DEFAULT for &style_screen_log_list_1_main_main_default
    static lv_style_t style_screen_log_list_1_main_main_default;
    ui_init_style(&style_screen_log_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_log_list_1_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_log_list_1_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_log_list_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_log_list_1_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_log_list_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_log_list_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_log_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_log_list_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_log_list_1_main_main_default, 255);
    lv_style_set_border_color(&style_screen_log_list_1_main_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_log_list_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_log_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_log_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_log_list_1, &style_screen_log_list_1_main_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_log_list_1_main_scrollbar_default
    static lv_style_t style_screen_log_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_log_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_log_list_1_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_log_list_1_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_log_list_1_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_log_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_log_list_1, &style_screen_log_list_1_main_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_log_list_1_extra_btns_main_default
    static lv_style_t style_screen_log_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_log_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_log_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_log_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_log_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_log_list_1_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_log_list_1_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_log_list_1_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_log_list_1_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_log_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_log_list_1_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_log_list_1_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_log_list_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_log_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);

    // Write style state: LV_STATE_DEFAULT for &style_screen_log_list_1_extra_texts_main_default
    static lv_style_t style_screen_log_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_log_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_log_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_log_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_log_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_log_list_1_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_log_list_1_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_log_list_1_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_log_list_1_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_log_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_log_list_1_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_log_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_log_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_log_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_log_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);

    // Write codes screen_log_btn_1
    ui->screen_log_btn_1 = lv_btn_create(ui->screen_log);
    ui->screen_log_btn_1_label = lv_label_create(ui->screen_log_btn_1);
    lv_label_set_text(ui->screen_log_btn_1_label, "获取");
    lv_label_set_long_mode(ui->screen_log_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_log_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_log_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_log_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_log_btn_1, 899, 346);
    lv_obj_set_size(ui->screen_log_btn_1, 100, 50);

    // Write style for screen_log_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_log_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_log_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_log_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_log_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_btn_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_log_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_log_btn_1, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_log_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_log_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_log_btn_2
    ui->screen_log_btn_2 = lv_btn_create(ui->screen_log);
    ui->screen_log_btn_2_label = lv_label_create(ui->screen_log_btn_2);
    lv_label_set_text(ui->screen_log_btn_2_label, "返回");
    lv_label_set_long_mode(ui->screen_log_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_log_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_log_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_log_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_log_btn_2, 904, 24);
    lv_obj_set_size(ui->screen_log_btn_2, 100, 50);

    // Write style for screen_log_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_log_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_log_btn_2, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_log_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_log_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_btn_2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_log_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_log_btn_2, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_log_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_log_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of screen_log.

    // Update current screen layout.
    lv_obj_update_layout(ui->screen_log);

    // Init events for screen.
    events_init_screen_log(ui);
}
