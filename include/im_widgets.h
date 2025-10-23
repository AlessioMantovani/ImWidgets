#pragma once

#include "imgui.h"

struct ImWidgetsStyle {
    /* Toggle switch styling  */
    float   ImToggleSwitchRadius;
    ImVec2  ImToggleSwitchSize;
    ImVec4  ImToggleSwitchOnColor;  // background when ON
    ImVec4  ImToggleSwitchOffColor; // background when OFF
    ImVec4  ImToggleSwitchKnobColor;// knob color
    ImVec4  ImToggleSwitchDisabledColor; // disabled color    

    /* Radio Button Group */
    float   ImRadioButtonGroupRadius;
    float   ImRadioButtonGroupSpacing;
    ImVec2  ImRadioButtonGroupSize;
    ImVec4  ImRadioButtonGroupActiveColor;
    ImVec4  ImRadioButtonGroupInactiveColor;
    ImVec4  ImRadioButtonGroupDisabledColor;
    ImVec4  ImRadioButtonGroupActiveTextColor;
    ImVec4  ImRadioButtonGroupInactiveTextColor;
    ImVec4  ImRadioButtonGroupDisabledTextColor;

    /* Progress Bar */
    float   ImProgressBarRadius;
    float   ImProgressBarCircleRadius;
    float   ImProgressBarCircleThickness;
    ImVec2  ImProgressBarSize;
    ImVec4  ImProgressBarBgColor;
    ImVec4  ImProgressBarColor;
    ImVec4  ImProgressBarTextColor;

    /* Spinner */
    float   ImSpinnerRadius;
    float   ImSpinnerThickness;
    float   ImSpinnerSpeed;
    int     ImSpinnerSegments;
    ImVec4  ImSpinnerColor;

    /* Calendar */
    float   ImCalendarInnerWidth;
    ImVec2  ImCalendarOuterSize;
    ImVec4  ImCalendarTextColor;
    ImVec4  ImCalendarBorderStrongColor;
    ImVec4  ImCalendarBorderLightColor;
    ImVec4  ImCalendarSelectedCellColor;
    ImVec4  ImCalendarHoveredCellColor;
    ImVec4  ImCalendarActivePressFeedBack;
    ImVec4  ImCalendarBgColor;
};

enum ImToggleFlags_ {
    ImToggleFlags_None                  = 0,
    ImToggleFlags_NoLabel               = 1 << 0,
    ImToggleFlags_Disabled              = 1 << 1,
    ImToggleFlags_NoAnimation           = 1 << 2,
};

enum ImRadioButtonGroupFlags_ {
    ImRadioButtonGroupFlags_None        = 0,
    ImRadioButtonGroupFlags_NoLabel     = 1 << 0,
    ImRadioButtonGroupFlags_Vertical    = 1 << 1,
    ImRadioButtonGroupFlags_Disabled    = 1 << 2,
};

enum ImProgressBarFlags_ {
    ImProgressBarFlags_None             = 0,
    ImProgressBarFLags_NoLabel          = 1 << 0,
    ImProgressBarFlags_PercentageLabel  = 1 << 1,
    ImProgressBarFlags_Circle           = 1 << 2,
};

enum ImSpinnerFlags_ {
    ImSpinnerFlags_None                 = 0,
    ImSpinnerFlags_NoLabel              = 1 << 0,
    ImSpinnerFlags_LabelLeft            = 1 << 1,
    ImSpinnerFlags_LabelRight           = 1 << 2,
};

enum ImCalendarFlags_ {
    ImCalendarFlags_None                 = 0,
    ImCalendarFlags_SelectableDate       = 1 << 0,
};

typedef int ImToggleFlags;
typedef int ImRadioButtonGroupFlags;
typedef int ImProgressBarFLags;
typedef int ImSpinnerFlags;
typedef int ImCalendarFlags;
typedef int ImGuiTabBarFlags;

/*
    TODO:
    - File Picker
    - Collapsible Card
    - Tab Strip with Close Buttons → like browser tabs.
    - Notification Toasts
    - Rich Text
*/

namespace ImWidgets{
    ImWidgetsStyle& GetStyle();
    /* Toggle Switch */
    bool ToggleSwitch(const char* label, bool* v, ImToggleFlags flags = ImToggleFlags_None);

    /* Radio Button Group */
    bool RadioButtonGroup(const char* label, int* current, const char* const items[], int items_count, ImRadioButtonGroupFlags flags = ImRadioButtonGroupFlags_None);

    /* Progress bar */
    void ProgressBar(const char* label, float min, float max, float current, ImToggleFlags flags = ImProgressBarFlags_None);

    /* Spinner */
    void Spinner(const char* label, ImSpinnerFlags flags = ImSpinnerFlags_None);

    /* Calendar */
    /*
        TODO: Finish implementation
    */
    void Calendar(const char* label, int& day, int& month, int& year, ImCalendarFlags flags = ImCalendarFlags_None);

    /* Tab strip */
    void TabStrip(const char* label, ImGuiTabBarFlags flags = ImGuiTabBarFlags_None);
    void Tab(const char* label, bool* p_open = nullptr);
}