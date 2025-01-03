/*
 * @Author: totoro huangjian921@outlook.com
 * @Date: 2022-08-15 13:36:10
 * @FilePath: /gui/application/setting/NetWork.cpp
 * @Description: None
 * @other: None
 */
#include <string.h>
#include <string>
#include <pthread.h>
#include "NetWork.h"
#include "NetWorkApi.h"

static lv_obj_t* NetWorkPanel;
static lv_obj_t* WiFiPanel;
static lv_obj_t* WiFiControlPanel;
static lv_obj_t* WiFiControlSwitch;
static lv_obj_t* WiFiConnectedPanel;
static lv_obj_t* WiFiAvailablePanel;
static lv_obj_t* WiFiRefreshObj;
static lv_obj_t* WiFiAvailableListPanel;
static lv_obj_t* ConnectPanel;
static lv_obj_t* PwdArea;
static lv_obj_t* CheckboxPanel;
static lv_obj_t* Keyboard;
static lv_group_t* MainGroup;
static lv_coord_t AvailablePanelPos_y;
static lv_coord_t AvailablePanelPos_h;
static char password[21];

enum {
    NetWork_WiFi,
    NetWork_Bluetooth
};

enum {
    WiFiControlPanel_Switch = 0,
    WiFiControlPanel_Refresh,
};

enum {
    WiFiConnectPanel_CodeArea = 1,
    WiFiConnectPanel_Checkbox,
    WiFiConnectPanel_Ok,
    WiFiConnectPanel_Cancel
};

extern pthread_mutex_t lvgl_task_mutex;

void CreateWiFiPanel(lv_obj_t* parent);
void CreateConnectPanel(lv_obj_t* parent, void* wifi_info);
static void WiFiRefreshAvailableList(void);
static void WiFiRefresh(void);

void CreateNetWorkPanel(lv_obj_t* parent)
{
    auto event_cb = [] (lv_event_t* event) {
        lv_obj_t* target = lv_event_get_current_target(event);
        //lv_obj_t* parent = lv_obj_get_parent(target);
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        lv_group_t* group = get_activity_group();
        int index = lv_obj_get_index(target);
        switch (value)
        {
            case LV_KEY_UP:
                lv_group_focus_prev(group);
                break;
            case LV_KEY_DOWN:
                lv_group_focus_next(group);
                break;
            case LV_KEY_ENTER:
                switch (index)
                {
                case NetWork_WiFi:
                    CreateWiFiPanel(NetWorkPanel);
                    break;
                case NetWork_Bluetooth:
                    
                    break;
                
                default:
                    break;
                }
                break;
            case LV_KEY_ESC:
                delete_group(group);
                lv_obj_del_async(NetWorkPanel);
                break;
            default:
                break;
        }
    };

    NetWorkPanel = lv_obj_create(parent);
    lv_obj_set_size(NetWorkPanel, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(NetWorkPanel);
    lv_obj_clear_flag(NetWorkPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_color(NetWorkPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(NetWorkPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(NetWorkPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    MainGroup = create_new_group();
    set_group_activity(MainGroup);
    lv_obj_t* obj = lv_obj_create(NetWorkPanel);
    lv_obj_set_size(obj, 800, 90);
    lv_obj_set_pos(obj, 10, -119);
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    lv_obj_add_state(obj, LV_STATE_FOCUSED);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x61B09D), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_group_add_obj(MainGroup, obj);
    lv_obj_add_event_cb(obj, event_cb, LV_EVENT_KEY, nullptr);

    lv_obj_t* img = lv_img_create(obj);
    lv_img_set_src(img, &ui_img_wifi_free_64_png);
    lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(img, -200, 0);
    lv_obj_set_align(img, LV_ALIGN_CENTER);
    lv_obj_add_flag(img, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* lab = lv_label_create(obj);
    lv_obj_set_size(lab, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(lab, 250, 0);
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
    lv_label_set_text(lab, _("setting_p_wifi"));
    lv_obj_set_style_text_font(lab, &ui_font_MyFont38, LV_PART_MAIN | LV_STATE_DEFAULT);

    obj = lv_obj_create(NetWorkPanel);
    lv_obj_set_size(obj, 800, 90);
    lv_obj_set_pos(obj, 14, 10);
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x61B09D), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_group_add_obj(MainGroup, obj);
    lv_obj_add_event_cb(obj, event_cb, LV_EVENT_KEY, nullptr);

    img = lv_img_create(obj);
    lv_img_set_src(img, &ui_img_bluetooth_64_png);
    lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(img, -200, 0);
    lv_obj_set_align(img, LV_ALIGN_CENTER);
    lv_obj_add_flag(img, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);

    lab = lv_label_create(obj);
    lv_obj_set_size(lab, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(lab, 250, 0);
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
    lv_label_set_text(lab, _("setting_p_bluetooth"));
    lv_obj_set_style_text_align(lab, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lab, &ui_font_MyFont38, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CreateWiFiPanel(lv_obj_t* parent)
{
    auto event_cb = [] (lv_event_t* event) {
        lv_obj_t* target = lv_event_get_current_target(event);
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        lv_group_t* group = get_activity_group();
        int index = lv_obj_get_index(target);
        switch (value)
        {
            case LV_KEY_UP:
                lv_group_focus_prev(group);
                break;
            case LV_KEY_DOWN:
                lv_group_focus_next(group);
                break;
            case LV_KEY_ENTER:
                switch (index)
                {
                case WiFiControlPanel_Switch:
                    lv_event_send(WiFiControlSwitch, LV_EVENT_RELEASED, nullptr);
                    break;
                case WiFiControlPanel_Refresh:
                    WiFiRefresh();
                    break;
                default:
                    break;
                }
                break;
            case LV_KEY_ESC:
                delete_group(group);
                lv_obj_del_async(WiFiPanel);
                break;
            default:
                break;
        }
    };
    
    WiFiPanel = lv_obj_create(parent);
    lv_obj_set_size(WiFiPanel, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(WiFiPanel, 0, 0);
    lv_obj_set_align(WiFiPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(WiFiPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(WiFiPanel, lv_color_hex(0xE2E2E2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(WiFiPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(WiFiPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(WiFiPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    MainGroup = create_new_group();
    set_group_activity(MainGroup);
    lv_group_set_wrap(MainGroup, false);

    WiFiControlPanel = lv_obj_create(WiFiPanel);
    lv_obj_set_size(WiFiControlPanel, 1020, 150);
    lv_obj_set_pos(WiFiControlPanel, 0, -10);
    lv_obj_set_align(WiFiControlPanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(WiFiControlPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(WiFiControlPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(WiFiControlPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* panel = lv_obj_create(WiFiControlPanel);
    lv_obj_set_size(panel, 980, 65);
    lv_obj_set_pos(panel, 0, -36);
    lv_obj_set_align(panel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(panel, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x87A4C1), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(panel, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_group_add_obj(MainGroup, panel);
    lv_obj_add_event_cb(panel, event_cb, LV_EVENT_KEY, nullptr);

    lv_obj_t* lab = lv_label_create(panel);
    lv_obj_set_size(lab, 600, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_pos(lab, -170, 0);
    lv_obj_set_align(lab, LV_ALIGN_CENTER);
    lv_label_set_text(lab, "WLAN");
    lv_obj_set_style_text_font(lab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    WiFiControlSwitch = lv_switch_create(panel);
    lv_obj_set_size(WiFiControlSwitch, 90, 45);
    lv_obj_set_pos(WiFiControlSwitch, 370, 0);
    lv_obj_add_state(WiFiControlSwitch, LV_STATE_CHECKED);
    lv_obj_set_align(WiFiControlSwitch, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(WiFiControlSwitch, [] (lv_event_t* event) { 
        lv_obj_t* target = lv_event_get_current_target(event);
        if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
            if (WiFiConnectedPanel) lv_obj_clear_flag(WiFiConnectedPanel, LV_OBJ_FLAG_HIDDEN);
            if (WiFiAvailablePanel) lv_obj_clear_flag(WiFiAvailablePanel, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            if (WiFiConnectedPanel) lv_obj_add_flag(WiFiConnectedPanel, LV_OBJ_FLAG_HIDDEN);
            if (WiFiAvailablePanel) lv_obj_add_flag(WiFiAvailablePanel, LV_OBJ_FLAG_HIDDEN);
        }
    }, LV_EVENT_RELEASED, nullptr);

    panel = lv_obj_create(WiFiControlPanel);
    lv_obj_set_size(panel, 980, 65);
    lv_obj_set_pos(panel, 0, 30);
    lv_obj_set_align(panel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(panel, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x87A4C1), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(panel, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_group_add_obj(MainGroup, panel);
    lv_obj_add_event_cb(panel, event_cb, LV_EVENT_KEY, nullptr);

    lab = lv_label_create(panel);
    lv_obj_set_size(lab, 600, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_pos(lab, -170, 0);
    lv_obj_set_align(lab, LV_ALIGN_CENTER);
    lv_label_set_text(lab, _("setting_p_refresh"));
    lv_obj_set_style_text_font(lab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (WiFi_IsConnected()) {
    //if (1) {
        WiFiConnectedPanel = lv_obj_create(WiFiPanel);
        lv_obj_set_size(WiFiConnectedPanel, 1020, 110);
        lv_obj_set_pos(WiFiConnectedPanel, 0, 145);
        lv_obj_set_align(WiFiConnectedPanel, LV_ALIGN_TOP_MID);
        lv_obj_clear_flag(WiFiConnectedPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(WiFiConnectedPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(WiFiConnectedPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lab = lv_label_create(WiFiConnectedPanel);
        lv_obj_set_size(lab, 200, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_pos(lab, -370, -20);
        lv_obj_set_align(lab, LV_ALIGN_TOP_MID);
        lv_label_set_text(lab, _("setting_p_wifi_connected"));
        lv_obj_set_style_text_font(lab, &ui_font_MyFont26, LV_PART_MAIN | LV_STATE_DEFAULT);

        panel = lv_obj_create(WiFiConnectedPanel);
        lv_obj_set_size(panel, 980, 65);
        lv_obj_set_pos(panel, 0, 10);
        lv_obj_set_align(panel, LV_ALIGN_TOP_MID);
        lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(panel, lv_color_hex(0x87A4C1), LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(panel, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_group_add_obj(MainGroup, panel);
        lv_obj_add_event_cb(panel, event_cb, LV_EVENT_KEY, nullptr);

        lab = lv_label_create(panel);
        lv_obj_set_size(lab, 600, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_pos(lab, -170, 0);
        lv_obj_set_align(lab, LV_ALIGN_CENTER);
        lv_label_set_text(lab, "HONOR");
        lv_obj_set_style_text_color(lab, lv_color_hex(0x2565B0), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(lab, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* img = lv_img_create(panel);
        lv_img_set_src(img, &ui_img_wifi_64_png);
        lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);    /// 64
        lv_obj_set_pos(img, 430, 0);
        lv_obj_set_align(img, LV_ALIGN_CENTER);
        lv_obj_add_flag(img, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
        lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        AvailablePanelPos_y = 260;
        AvailablePanelPos_h = 425;
    }
    else {
        WiFiConnectedPanel = nullptr;
        AvailablePanelPos_y = 145;
        AvailablePanelPos_h = 555;
    }
    WiFiAvailablePanel = lv_obj_create(WiFiPanel);
    lv_obj_set_size(WiFiAvailablePanel, 1020, AvailablePanelPos_h);
    lv_obj_set_pos(WiFiAvailablePanel, 0, AvailablePanelPos_y);
    lv_obj_set_align(WiFiAvailablePanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(WiFiAvailablePanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_opa(WiFiAvailablePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lab = lv_label_create(WiFiAvailablePanel);
    lv_obj_set_size(lab, 200, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_pos(lab, -370, -20);
    lv_obj_set_align(lab, LV_ALIGN_TOP_MID);
    lv_label_set_text(lab, _("setting_p_wifi_available"));
    lv_obj_set_style_text_font(lab, &ui_font_MyFont26, LV_PART_MAIN | LV_STATE_DEFAULT);

    WiFiRefreshObj = lv_img_create(WiFiAvailablePanel);
    lv_img_set_src(WiFiRefreshObj, &ui_img_refresh32_png);
    lv_obj_set_size(WiFiRefreshObj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);    /// 48
    lv_obj_set_pos(WiFiRefreshObj, 430, -20);
    lv_obj_set_align(WiFiRefreshObj, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(WiFiRefreshObj, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(WiFiRefreshObj, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    WiFiAvailableListPanel = lv_obj_create(WiFiAvailablePanel);
    lv_obj_remove_style_all(WiFiAvailableListPanel);
    lv_obj_set_size(WiFiAvailableListPanel, 1000, AvailablePanelPos_h - 35);
    lv_obj_set_pos(WiFiAvailableListPanel, 0, 10);
    lv_obj_set_align(WiFiAvailableListPanel, LV_ALIGN_TOP_MID);
    lv_obj_set_scrollbar_mode(WiFiAvailableListPanel,LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(WiFiAvailableListPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(WiFiAvailableListPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(WiFiAvailableListPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(WiFiAvailableListPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_msg_subscribe_obj(MSG_SHOW_WIFI_LIST, WiFiAvailableListPanel, nullptr);
    NetWorkInit();
    lv_obj_add_event_cb(WiFiAvailableListPanel, [] (lv_event_t* event) {
        WiFiRefreshAvailableList();
    }, LV_EVENT_MSG_RECEIVED, NULL);
    WiFiRefresh();
}

void CreateInputCodePanel(lv_obj_t* parent)
{
    MainGroup = create_new_group();
    set_group_activity(MainGroup);
    Keyboard = lv_keyboard_create(parent);
    lv_keyboard_set_mode(Keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_set_size(Keyboard, 1130, 410);
    lv_obj_set_pos(Keyboard, 0, 120);
    lv_obj_set_align(Keyboard, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(Keyboard, &lv_font_montserrat_30, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_group_add_obj(MainGroup, Keyboard);
    lv_obj_add_event_cb(Keyboard, [] (lv_event_t* event) {
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        if (value == LV_KEY_ESC) {
            delete_group(MainGroup);
            lv_obj_del_async(Keyboard);
            lv_obj_set_y(ConnectPanel, 0);//-150
        }
    }, LV_EVENT_KEY, nullptr);
    lv_keyboard_set_textarea(Keyboard, PwdArea);
}

void CreateConnectPanel(lv_obj_t* parent, void* wifi_info)
{
    auto event_cb = [] (lv_event_t* event) {
        lv_obj_t* target = lv_event_get_current_target(event);
        lv_obj_t* l_parent = lv_obj_get_parent(target);
        lv_group_t* group = get_activity_group();
        int index = lv_obj_get_index(target);
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        switch (value)
        {
            case LV_KEY_UP:
                if (target != lv_obj_get_child(ConnectPanel, 1))
                    lv_group_focus_prev(group);
                break;
            case LV_KEY_DOWN:
                if (target != lv_obj_get_child(ConnectPanel, lv_obj_get_child_cnt(ConnectPanel) - 1))
                    lv_group_focus_next(group);
                break;
            case LV_KEY_LEFT:
                if (target == lv_obj_get_child(ConnectPanel, lv_obj_get_child_cnt(ConnectPanel) - 1))
                    lv_group_focus_prev(group);
                break;
            case LV_KEY_RIGHT:
                if (target == lv_obj_get_child(ConnectPanel, lv_obj_get_child_cnt(ConnectPanel) - 2))
                    lv_group_focus_next(group);
                break;
            case LV_KEY_ENTER:
                switch (index)
                {
                case WiFiConnectPanel_CodeArea:
                    lv_obj_set_y(ConnectPanel, -150);
                    CreateInputCodePanel(WiFiPanel);
                    break;
                case WiFiConnectPanel_Checkbox:
                    lv_event_send(lv_obj_get_child(target, 0), LV_EVENT_RELEASED, nullptr);
                    break;
                case WiFiConnectPanel_Ok:
                    memcpy(password, lv_textarea_get_text(PwdArea),20);
                    // WiFi_Connect(static_cast<wifi_ap_info_t *>(l_parent->user_data));

                    break;
                case WiFiConnectPanel_Cancel:
                    delete_group(group);
                    lv_obj_del_async(ConnectPanel);
                    break;
                default:
                    break;
                }
                break;
            case LV_KEY_ESC:
                delete_group(group);
                lv_obj_del_async(ConnectPanel);
                break;
            default:
                break;
        }
    };

    MainGroup = create_new_group();
    set_group_activity(MainGroup);

    ConnectPanel = lv_obj_create(parent);
    lv_obj_set_size(ConnectPanel, 900, 400);
    lv_obj_set_pos(ConnectPanel, 0, 0);
    lv_obj_set_align(ConnectPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ConnectPanel, LV_OBJ_FLAG_SCROLLABLE);
    ConnectPanel->user_data = wifi_info;

    lv_obj_t* WiFiName = lv_label_create(ConnectPanel);
    lv_obj_set_size(WiFiName, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(WiFiName, 0, -135);
    lv_obj_set_align(WiFiName, LV_ALIGN_CENTER);
    // lv_label_set_text(WiFiName, (static_cast<wifi_ap_info_t *>(wifi_info))->ssid);
    lv_obj_set_style_text_font(WiFiName, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    PwdArea = lv_textarea_create(ConnectPanel);
    lv_obj_set_size(PwdArea, 600, 50);
    lv_obj_set_pos(PwdArea, 0, -50);
    lv_obj_set_align(PwdArea, LV_ALIGN_CENTER);
    lv_textarea_set_text(PwdArea, "");
    lv_textarea_set_password_mode(PwdArea, true);
    lv_textarea_set_one_line(PwdArea, true);
    lv_textarea_set_placeholder_text(PwdArea, _("setting_p_code"));
    lv_obj_set_style_text_font(PwdArea, &ui_font_MyFont26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_group_add_obj(MainGroup, PwdArea);
    lv_obj_add_event_cb(PwdArea, event_cb, LV_EVENT_KEY, nullptr);
    lv_obj_add_event_cb(PwdArea, [] (lv_event_t* event) {
        lv_obj_t* target = lv_event_get_current_target(event);
        lv_obj_t* parent = lv_obj_get_parent(target);
        if (lv_obj_is_valid(Keyboard)) {
            memcpy(password, lv_textarea_get_text(target),20);
            // WiFi_Connect(static_cast<wifi_ap_info_t *>(parent->user_data));
            MainGroup = delete_group(MainGroup);
            lv_obj_del_async(Keyboard);
            lv_obj_set_y(ConnectPanel, 0);//-150
        }
    }, LV_EVENT_READY, nullptr);


    CheckboxPanel = lv_obj_create(ConnectPanel);
    lv_obj_set_size(CheckboxPanel, 200, 50);
    lv_obj_set_pos(CheckboxPanel, -200, 10);
    lv_obj_set_align(CheckboxPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(CheckboxPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_color(CheckboxPanel, lv_color_hex(0x90CAF9), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(CheckboxPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(CheckboxPanel, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_group_add_obj(MainGroup, CheckboxPanel);
    lv_obj_add_event_cb(CheckboxPanel, event_cb, LV_EVENT_KEY, nullptr);

    lv_obj_t* Checkbox = lv_checkbox_create(CheckboxPanel);
    lv_checkbox_set_text(Checkbox, _("setting_p_show_code"));
    lv_obj_set_size(Checkbox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(Checkbox, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(Checkbox, &ui_font_MyFont26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(Checkbox, [] (lv_event_t* event) { 
        lv_obj_t* target = lv_event_get_current_target(event);
        bool show_code = lv_obj_get_state(target) & LV_STATE_CHECKED ? false : true;
        lv_textarea_set_password_mode(PwdArea, show_code);
    }, LV_EVENT_RELEASED, nullptr);

    lv_obj_t* ConnectButton = lv_btn_create(ConnectPanel);
    lv_obj_set_size(ConnectButton, 130, 50);
    lv_obj_set_pos(ConnectButton, -180, 125);
    lv_obj_set_align(ConnectButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ConnectButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ConnectButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_group_add_obj(MainGroup, ConnectButton);
    lv_obj_add_event_cb(ConnectButton, event_cb, LV_EVENT_KEY, nullptr);

    lv_obj_t* ConnectLab = lv_label_create(ConnectButton);
    lv_obj_set_size(ConnectLab, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ConnectLab, 0, 0);
    lv_obj_set_align(ConnectLab, LV_ALIGN_CENTER);
    lv_label_set_text(ConnectLab, _("setting_p_connect"));
    lv_obj_set_style_text_font(ConnectLab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* CancelButton = lv_btn_create(ConnectPanel);
    lv_obj_set_size(CancelButton, 130, 50);
    lv_obj_set_pos(CancelButton, 180, 125);
    lv_obj_set_align(CancelButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(CancelButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(CancelButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_group_add_obj(MainGroup, CancelButton);
    lv_obj_add_event_cb(CancelButton, event_cb, LV_EVENT_KEY, nullptr);

    lv_obj_t* CancelLab = lv_label_create(CancelButton);
    lv_obj_set_size(CancelLab, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(CancelLab, 0, 0);
    lv_obj_set_align(CancelLab, LV_ALIGN_CENTER);
    lv_label_set_text(CancelLab, _("setting_p_cancel"));
    lv_obj_set_style_text_font(CancelLab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/* 本文件内函数 */
static void WiFiRefreshAvailableList(void)
{
    auto event_cb = [] (lv_event_t* event) {
        lv_obj_t* target = lv_event_get_current_target(event);
        uint32_t value = lv_indev_get_key(lv_indev_get_act());
        lv_group_t* group = get_activity_group();
        switch (value)
        {
            case LV_KEY_UP:
                lv_group_focus_prev(group);
                break;
            case LV_KEY_DOWN:
                lv_group_focus_next(group);
                break;
            case LV_KEY_ENTER:
                CreateConnectPanel(WiFiPanel, target->user_data);
                break;
            case LV_KEY_ESC:
                delete_group(group);
                lv_obj_del_async(WiFiPanel);
                break;
            default:
                break;
        }
    };
    int ap_count = 0;
    // wifi_ap_info_t *wifi_list = WiFi_GetAPList();
    // ap_count = WiFi_GetAPCount();
    // if (wifi_list == nullptr || ap_count == 0) return;
    lv_obj_t* child = NULL;
    int i;
    for(i = 0; i < ap_count; ++i) {
        child = lv_obj_get_child(WiFiAvailableListPanel, i);
        if (!child) {
            child = lv_obj_create(WiFiAvailableListPanel);
            lv_obj_set_size(child, 980, 65);
            lv_obj_set_align(child, LV_ALIGN_TOP_MID);
            lv_obj_clear_flag(child, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_flag(child, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            lv_obj_set_style_bg_color(child, lv_color_hex(0x87A4C1), LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_bg_opa(child, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_group_add_obj(MainGroup, child);
            lv_obj_add_event_cb(child, event_cb, LV_EVENT_KEY, nullptr);

            lv_obj_t* lab = lv_label_create(child);
            lv_obj_set_size(lab, 600, LV_SIZE_CONTENT);    /// 1
            lv_obj_set_pos(lab, -170, 0);
            lv_obj_set_align(lab, LV_ALIGN_CENTER);
            lv_obj_set_style_text_font(lab, &ui_font_MyFont30, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_t* img = lv_img_create(child);
            lv_img_set_src(img, &ui_img_wifi_64_png);
            lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);    /// 64
            lv_obj_set_pos(img, 430, 0);
            lv_obj_set_align(img, LV_ALIGN_CENTER);
            lv_obj_add_flag(img, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
            lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        }
        // child->user_data = static_cast<void*>(&wifi_list[i]);
        // lv_label_set_text(child->spec_attr->children[0], wifi_list[i].ssid);
    }
    uint16_t ChildCnt = lv_obj_get_child_cnt(WiFiAvailableListPanel);
    for ( ; i < ChildCnt; ++i) {
        lv_obj_del_async(lv_obj_get_child(WiFiAvailableListPanel, i));
    }
    lv_obj_add_flag(WiFiRefreshObj,LV_OBJ_FLAG_HIDDEN);
    lv_anim_del(WiFiRefreshObj, nullptr);
    NetWorkDeInit();
}

static void WiFiRefresh(void)
{
    WiFi_Scan([] () { 
        pthread_mutex_lock(&lvgl_task_mutex);
        lv_msg_send(MSG_SHOW_WIFI_LIST, nullptr);
        pthread_mutex_unlock(&lvgl_task_mutex);
    });
    lv_obj_clear_flag(WiFiRefreshObj,LV_OBJ_FLAG_HIDDEN);
    SpinAnimation(WiFiRefreshObj, LV_ANIM_PLAYTIME_INFINITE, 1000);
}
