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



void setup_scr_screen_wifi(lv_ui *ui)
{
    LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18);

    //Write codes screen_wifi
    ui->screen_wifi = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_wifi, 1024, 600);
    lv_obj_set_scrollbar_mode(ui->screen_wifi, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_list_1
    ui->screen_wifi_list_1 = lv_list_create(ui->screen_wifi);
    lv_obj_set_pos(ui->screen_wifi_list_1, 49, 63);
    lv_obj_set_size(ui->screen_wifi_list_1, 403, 485);
    lv_obj_set_scrollbar_mode(ui->screen_wifi_list_1, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_1_main_main_default
    static lv_style_t style_screen_wifi_list_1_main_main_default;
    ui_init_style(&style_screen_wifi_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_1_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_1_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_1_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_wifi_list_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_wifi_list_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_wifi_list_1_main_main_default, 255);
    lv_style_set_border_color(&style_screen_wifi_list_1_main_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_wifi_list_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_wifi_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_wifi_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_wifi_list_1, &style_screen_wifi_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_1_main_scrollbar_default
    static lv_style_t style_screen_wifi_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_wifi_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_wifi_list_1_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_wifi_list_1_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_1_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_wifi_list_1, &style_screen_wifi_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_1_extra_btns_main_default
    static lv_style_t style_screen_wifi_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_wifi_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_1_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_wifi_list_1_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_wifi_list_1_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_wifi_list_1_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_wifi_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_wifi_list_1_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_wifi_list_1_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_1_extra_texts_main_default
    static lv_style_t style_screen_wifi_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_wifi_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_1_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_wifi_list_1_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_wifi_list_1_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_wifi_list_1_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_wifi_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_wifi_list_1_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_wifi_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_wifi_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes screen_wifi_ta_1
    ui->screen_wifi_ta_1 = lv_textarea_create(ui->screen_wifi);
    lv_textarea_set_text(ui->screen_wifi_ta_1, "");
    lv_textarea_set_placeholder_text(ui->screen_wifi_ta_1, "");
    lv_textarea_set_password_bullet(ui->screen_wifi_ta_1, "*");
    lv_textarea_set_password_mode(ui->screen_wifi_ta_1, true);
    lv_textarea_set_one_line(ui->screen_wifi_ta_1, false);
    lv_textarea_set_accepted_chars(ui->screen_wifi_ta_1, "");
    lv_textarea_set_max_length(ui->screen_wifi_ta_1, 32);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->screen_wifi_ta_1, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->screen_wifi_ta_1, 575, 115);
    lv_obj_set_size(ui->screen_wifi_ta_1, 344, 92);

    //Write style for screen_wifi_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_wifi_ta_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_ta_1, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_ta_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_ta_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_wifi_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_wifi_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_wifi_ta_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_wifi_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_ta_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_wifi_btn_1
    ui->screen_wifi_btn_1 = lv_btn_create(ui->screen_wifi);
    ui->screen_wifi_btn_1_label = lv_label_create(ui->screen_wifi_btn_1);
    lv_label_set_text(ui->screen_wifi_btn_1_label, "扫描");
    lv_label_set_long_mode(ui->screen_wifi_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_wifi_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_wifi_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_wifi_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_wifi_btn_1, 580, 433);
    lv_obj_set_size(ui->screen_wifi_btn_1, 146, 60);

    //Write style for screen_wifi_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_btn_1, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_btn_2
    ui->screen_wifi_btn_2 = lv_btn_create(ui->screen_wifi);
    ui->screen_wifi_btn_2_label = lv_label_create(ui->screen_wifi_btn_2);
    lv_label_set_text(ui->screen_wifi_btn_2_label, "连接");
    lv_label_set_long_mode(ui->screen_wifi_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_wifi_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_wifi_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_wifi_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_wifi_btn_2, 792, 433);
    lv_obj_set_size(ui->screen_wifi_btn_2, 146, 60);

    //Write style for screen_wifi_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_btn_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_btn_2, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_btn_3
    ui->screen_wifi_btn_3 = lv_btn_create(ui->screen_wifi);
    ui->screen_wifi_btn_3_label = lv_label_create(ui->screen_wifi_btn_3);
    lv_label_set_text(ui->screen_wifi_btn_3_label, "返回");
    lv_label_set_long_mode(ui->screen_wifi_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_wifi_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_wifi_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_wifi_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_wifi_btn_3, 904, 24);
    lv_obj_set_size(ui->screen_wifi_btn_3, 100, 50);

    //Write style for screen_wifi_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_btn_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_btn_3, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_label_1
    ui->screen_wifi_label_1 = lv_label_create(ui->screen_wifi);
    lv_label_set_text(ui->screen_wifi_label_1, "");
    lv_label_set_long_mode(ui->screen_wifi_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_wifi_label_1, 575, 276);
    lv_obj_set_size(ui->screen_wifi_label_1, 100, 32);

    //Write style for screen_wifi_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_label_1, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_wifi.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_wifi);

    //Init events for screen.
    events_init_screen_wifi(ui);
}
