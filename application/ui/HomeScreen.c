﻿/*
 * @Author: totoro huangjian921@outlook.com
 * @Date: 2022-05-23 13:51:24
 * @LastEditors: totoro huangjian921@outlook.com
 * @LastEditTime: 2022-06-03 14:59:38
 * @FilePath: /gui/application/ui/HomeScreen.c
 * @Description: None
 * @other: None
 */
#include <unistd.h>
#include <stdio.h>
#include "HomeScreen.h"
#include "application/windows.h"
//#include "lv_i18n/src/lv_i18n.h"
//#include "application/key/key.h"

lv_obj_t* ui_HomeScreen;
lv_obj_t* ui_Main_Panel;
lv_obj_t* ui_Source_Panel;
//static lv_group_t* Main_Group;
//static lv_group_t* Source_Group;

enum CategoryList {
    UDisk,
    Setting,
    Source,
    IOS,
    Android,
    DLNA,
    CategoryNumber
};

enum SourceList {
    SourceUDisk,
    HDMI,
    SourceListNumber
};

static void event_handler(lv_event_t* event);

/*
static void ui_event_BTN_Source(lv_event_t* event);
static void SetDefaultGroup(lv_group_t* group) {
    lv_group_set_default(group);
    lv_indev_set_group(lv_indev_get_act(), group);
    //lv_indev_set_group(lv_win32_keypad_device_object, group);
}*/

static void CreateMainPanel(lv_obj_t* parent)
{
    static const lv_coord_t btn_area[CategoryNumber][4] = {//{ x, y, w, h}
        { -226, -82, 430, 231},
        {  118, -82, 200, 231},
        {  345,   0, 200, 395},
        { -342, 130, 200, 135},
        { -110, 130, 200, 135},
        {  118, 130, 200, 135}
    };
    static const lv_coord_t lab_area[CategoryNumber][2] = {//{ x, y}
        { -124,  -60},
        {    0,  -60},
        {    0, -142},
        {    0,  -38},
        {    0,  -38},
        {   -8,  -38}
    };
    static const char* str[] =  { "U Disk", "Setting", "Source", "IOS", "Android", "DLNA"};
    static const lv_img_dsc_t* image_src[][2] = {
        {& ui_img_udisk_n_png ,        &ui_img_udisk_f_png},
        {& ui_img_setting_n_png ,      &ui_img_setting_f_png},
        {& ui_img_source_n_png ,       &ui_img_source_f_png},
        {& ui_img_ios_cast_n_png ,     &ui_img_ios_cast_f_png},
        {& ui_img_android_cast_n_png , &ui_img_android_cast_f_png},
        {& ui_img_dlna_cast_n_png ,    &ui_img_dlna_cast_f_png}
    };

    // ui_Main_Panel
    ui_Main_Panel = lv_obj_create(parent);
    lv_obj_set_size(ui_Main_Panel, 1085, 650);
    lv_obj_set_pos(ui_Main_Panel, 0, 0);
    lv_obj_set_align(ui_Main_Panel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(ui_Main_Panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Main_Panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    for (int i = 0; i < CategoryNumber; i++) {
        // ui_btn
        lv_obj_t* ui_btn = lv_btn_create(ui_Main_Panel);
        lv_obj_set_size(ui_btn, btn_area[i][2], btn_area[i][3]);
        lv_obj_set_pos(ui_btn, btn_area[i][0], btn_area[i][1]);
        lv_obj_set_align(ui_btn, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(ui_btn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_opa(ui_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_src(ui_btn, image_src[i][0], LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_src(ui_btn, image_src[i][1], LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_add_event_cb(ui_btn, event_handler, LV_EVENT_KEY, NULL);
        // ui_lab
        lv_obj_t* ui_lab = lv_label_create(ui_btn);
        lv_obj_set_size(ui_lab, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_pos(ui_lab, lab_area[i][0], lab_area[i][1]);
        lv_obj_set_align(ui_lab, LV_ALIGN_CENTER);
        lv_label_set_text(ui_lab, str[i]);
        lv_obj_set_style_text_font(ui_lab, &ui_font_MyFont38, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void CreateSourcePanel(lv_obj_t* parent)
{
    ui_Source_Panel = lv_obj_create(parent);
    lv_obj_set_size(ui_Source_Panel, 1033, 630);
    lv_obj_set_pos(ui_Source_Panel, 0, 0);
    lv_obj_set_align(ui_Source_Panel, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Source_Panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_color(ui_Source_Panel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Source_Panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Source_Panel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Source_Panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_IMG_SourceType
    lv_obj_t* ui_IMG_SourceType = lv_img_create(ui_Source_Panel);
    lv_img_set_src(ui_IMG_SourceType, &ui_img_usb2_big_png);
    lv_obj_set_size(ui_IMG_SourceType, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_IMG_SourceType, 0, -80);
    lv_obj_set_align(ui_IMG_SourceType, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IMG_SourceType, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_IMG_SourceType, LV_OBJ_FLAG_SCROLLABLE);
    // ui_LAB_SourceType
    lv_obj_t* ui_LAB_SourceType = lv_label_create(ui_IMG_SourceType);
    lv_obj_set_size(ui_LAB_SourceType, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_LAB_SourceType, 0, 84);
    lv_obj_set_align(ui_LAB_SourceType, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LAB_SourceType, "U Disk");
    lv_obj_set_style_text_color(ui_LAB_SourceType, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LAB_SourceType, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LAB_SourceType, &ui_font_MyFont38, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_BTN_Source_USB
    lv_obj_t* ui_BTN_Source_USB = lv_btn_create(ui_Source_Panel);
    lv_obj_set_size(ui_BTN_Source_USB, 113, 107);
    lv_obj_set_pos(ui_BTN_Source_USB, -87, 160);
    lv_obj_set_align(ui_BTN_Source_USB, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BTN_Source_USB, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_BTN_Source_USB, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_BTN_Source_USB, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BTN_Source_USB, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BTN_Source_USB, &ui_img_usb2_n_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BTN_Source_USB, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BTN_Source_USB, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BTN_Source_USB, &ui_img_usb2_f_png, LV_PART_MAIN | LV_STATE_FOCUSED);

    // ui_BTN_Source_HDMI
    lv_obj_t* ui_BTN_Source_HDMI = lv_btn_create(ui_Source_Panel);
    lv_obj_set_size(ui_BTN_Source_HDMI, 113, 107);
    lv_obj_set_pos(ui_BTN_Source_HDMI, 87, 160);
    lv_obj_set_align(ui_BTN_Source_HDMI, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BTN_Source_HDMI, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_BTN_Source_HDMI, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_BTN_Source_HDMI, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BTN_Source_HDMI, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BTN_Source_HDMI, &ui_img_hdmi_n_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BTN_Source_HDMI, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BTN_Source_HDMI, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BTN_Source_HDMI, &ui_img_hdmi_f_png, LV_PART_MAIN | LV_STATE_FOCUSED);
}

static void event_handler(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* ta = lv_event_get_target(event);
    lv_obj_t* parent = lv_obj_get_parent(ta);
    if (LV_EVENT_KEY == code) {
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        lv_group_t* group = (lv_group_t*)lv_obj_get_group(ta);
        switch (value)
        {
        case LV_KEY_LEFT:
            lv_group_focus_prev(group);
            break;
        case LV_KEY_RIGHT:
            lv_group_focus_next(group);
            break;
        case LV_KEY_UP:
            switch (lv_obj_get_index(ta))
            {
            case IOS:
            case Android:
                lv_group_focus_obj(lv_obj_get_child(parent, UDisk));
                break;
            case DLNA:
                lv_group_focus_obj(lv_obj_get_child(parent, Setting));
                break;
            default:
                break;
            }
            break;
        case LV_KEY_DOWN:
            switch (lv_obj_get_index(ta))
            {
            case UDisk:
                lv_group_focus_obj(lv_obj_get_child(parent, IOS));
                break;
            case Setting:
                lv_group_focus_obj(lv_obj_get_child(parent, DLNA));
                break;
            default:
                break;
            }
            break;
        case LV_KEY_ENTER:
            switch (lv_obj_get_index(ta))
            {
            case UDisk:
                CurrentScreen = MediaScreen;
                //printf("enter media\r\n");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    else if (LV_EVENT_FOCUSED == code) {
        //lv_img_set_src(ui_LangImage, ta->user_data);
    }
}

static void HomeWait(void)
{
    do {
        usleep(5000);
    } while (CurrentScreen == HomeScreen);
}

static void HomeClose(void)
{
    lv_obj_del(ui_HomeScreen);
}


/*static void ui_event_BTN_Source(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    //lv_obj_t * ta = lv_event_get_target(e);
    if (event == LV_EVENT_CLICKED) {
        //_ui_flag_modify(ui_Source_Panel, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        //_ui_flag_modify(ui_Main_Panel, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}*/

static void HomeInit(lv_obj_t* parent, void *param)
{
    (void)param;

    ui_HomeScreen = lv_obj_create(parent);
    lv_obj_set_size(ui_HomeScreen, 1280, 720);
    lv_obj_set_style_bg_color(ui_HomeScreen, lv_color_hex(0x3200FE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_HomeScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    CreateMainPanel(ui_HomeScreen);
    CreateSourcePanel(ui_HomeScreen);
}

static void LoadHome(void)
{
    lv_disp_load_scr(ui_HomeScreen);
}

window HomeWindow = {
    .ScreenInit = HomeInit,
    .ScreenLoad = LoadHome,
    .ScreenWait = HomeWait,
    .ScreenClose = HomeClose
};