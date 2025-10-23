#include "im_widgets.h"

#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <imgui_internal.h>

#include <iostream>

static ImWidgetsStyle GImWidgetsStyle = {
    /* Toggle switch styling  */
    10.0f,
    ImVec2(40, 20),
    ImVec4(0.2f, 0.8f, 0.4f, 1),
    ImVec4(0.4f, 0.4f, 0.4f, 1),
    ImVec4(1.0f, 1.0f, 1.0f, 1),
    ImVec4(0.2f, 0.2f, 0.2f, 1),

    /* Radio Button Group */
    10.0f,
    5.f,
    ImVec2(80, 30), 
    ImVec4(0.2f, 0.8f, 0.4f, 1),
    ImVec4(0.4f, 0.4f, 0.4f, 1),
    ImVec4(0.2f, 0.2f, 0.2f, 1),
    ImVec4(0.98f, 0.98f, 0.98f, 1.0f),
    ImVec4(0.75f, 0.75f, 0.75f, 1.0f),
    ImVec4(0.60f, 0.60f, 0.60f, 1.0f),

    /* Progress Bar */
    10.0f,
    20.0f,
    5.0f,
    ImVec2(80, 30),
    ImVec4(0.2f, 0.2f, 0.2f, 1),
    ImVec4(0.2f, 0.8f, 0.4f, 1.0f),
    ImVec4(1.0f, 1.0f, 1.0f, 1.0f),

    10.0f,
    2.0f,
    8.f,
    20,
    ImVec4(0.2f, 0.8f, 0.4f, 1.0f),

    /* Calendar */
    0,
    ImVec2(250, 100),
    ImVec4(0.85f, 0.87f, 0.90f, 1.0f),
    ImVec4(0.35f, 0.35f, 0.45f, 1.0f),
    ImVec4(0.50f, 0.50f, 0.60f, 0.4f),
    ImVec4(0.20f, 0.45f, 0.85f, 0.85f),
    ImVec4(0.25f, 0.55f, 0.95f, 0.9f),
    ImVec4(0.10f, 0.10f, 0.12f, 0.5f)
};

ImWidgetsStyle& ImWidgets::GetStyle() {
    return GImWidgetsStyle;
}

/*
    UTILITIES
*/

std::pair<std::string, bool> TrimTextToWidth(const char* text, float max_width) {
    ImVec2 text_size = ImGui::CalcTextSize(text);
        
    if (text_size.x <= max_width) {
        return {std::string(text), false};
    }
        
    // Text needs trimming
    std::string result;
    const char* ptr = text;
    float current_width = 0.0f;
    
    while (*ptr) {
        // Get the width of the current character
        ImVec2 char_size = ImGui::CalcTextSize(ptr, ptr + 1);
        
        // Check if adding this character would exceed the max width
        if (current_width + char_size.x > max_width) {
            break;
        }
        
        result += *ptr;
        current_width += char_size.x;
        ptr++;
    }
    
    return {result, true};
}

std::string TrimTextWithEllipsis(const char* text, float max_width) {
    static const char* ellipsis = "..";
    ImVec2 ellipsis_size = ImGui::CalcTextSize(ellipsis);

    ImVec2 text_size = ImGui::CalcTextSize(text);
    if (text_size.x <= max_width) {
        return std::string(text);
    }

    float available_width = max_width - ellipsis_size.x;
    if (available_width <= 0.0f) {
        return std::string(ellipsis);
    }
    
    auto [trimmed, was_truncated] = TrimTextToWidth(text, available_width);
    
    if (was_truncated) {
        return trimmed + ellipsis;
    }
    
    return trimmed;
}

void DrawCenteredText(ImDrawList* draw_list, const ImVec2& center, const char* text, ImU32 color) {
    ImVec2 text_size = ImGui::CalcTextSize(text);
    ImVec2 text_pos(
        center.x - text_size.x * 0.5f,
        center.y - text_size.y * 0.5f
    );
    draw_list->AddText(text_pos, color, text);
}

bool ImWidgets::ToggleSwitch(const char* label, bool* v, ImToggleFlags flags) {
    ImGui::PushID(label);

    ImWidgetsStyle& style = GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    bool changed = false;

    bool disabled = flags & ImToggleFlags_Disabled;
    bool animated = !(flags & ImToggleFlags_NoAnimation);

    ImGui::InvisibleButton("##toggle", style.ImToggleSwitchSize);
    if (!disabled && ImGui::IsItemClicked()) {
        *v = !*v;
        changed = true;
    }

    ImVec2 p = ImGui::GetItemRectMin();

    static std::unordered_map<ImGuiID, float> animStates;
    ImGuiID id = ImGui::GetID("##toggle");
    float& t = animStates[id]; 
    float target = *v ? 1.0f : 0.0f;

    if (!disabled) {
        if (animated)
            t += (target - t) * std::clamp(20.0f * ImGui::GetIO().DeltaTime, 0.0f, 1.0f);
        else
            t = target;        
    }

    ImU32 col_bg = ImGui::GetColorU32(disabled
        ? style.ImToggleSwitchDisabledColor
        : ImLerp(style.ImToggleSwitchOffColor, style.ImToggleSwitchOnColor, t));

    draw_list->AddRectFilled(
        p,
        ImVec2(p.x + style.ImToggleSwitchSize.x, p.y + style.ImToggleSwitchSize.y),
        col_bg,
        style.ImToggleSwitchRadius
    );

    draw_list->AddCircleFilled(
        ImVec2(p.x + style.ImToggleSwitchRadius + t * (style.ImToggleSwitchSize.x - 2*style.ImToggleSwitchRadius),
               p.y + style.ImToggleSwitchRadius),
        style.ImToggleSwitchRadius - 1.5f,
        ImGui::GetColorU32(style.ImToggleSwitchKnobColor)
    );

    ImGui::PopID();

    if (!(flags & ImToggleFlags_NoLabel)) {
        ImGui::SameLine();
        ImVec2 label_size = ImGui::CalcTextSize(label);
        float toggle_height = style.ImToggleSwitchSize.y;
        float vertical_offset = (toggle_height - label_size.y) * 0.5f;
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vertical_offset);
        ImGui::TextUnformatted(label);
    }
    return changed;
}

bool ImWidgets::RadioButtonGroup(const char* label, int* current, const char* const items[], int items_count, ImRadioButtonGroupFlags flags) {
    ImGui::PushID(label);

    ImWidgetsStyle& style = GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    bool changed = false;

    bool horizontal = !(flags & ImRadioButtonGroupFlags_Vertical);
    bool disabled = (flags & ImRadioButtonGroupFlags_Disabled) != 0;

    for (int i = 0; i < items_count; i++) {
        ImGui::PushID(i); 
        ImGui::InvisibleButton("##toggle", style.ImRadioButtonGroupSize); 
        if (!disabled && ImGui::IsItemClicked()) {
            *current = i;
            changed = true;
        }

        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImVec2 rect_size = ImVec2(p_max.x - p_min.x, p_max.y - p_min.y);

        ImVec4 bg_color;
        if (disabled) {
            bg_color = style.ImRadioButtonGroupDisabledColor;
        } else if (*current == i) {
            bg_color = style.ImRadioButtonGroupActiveColor;
        } else {
            bg_color = style.ImRadioButtonGroupInactiveColor;
        }

        draw_list->AddRectFilled(
            p_min,
            p_max,
            ImGui::GetColorU32(bg_color),
            5.0f
        );

        std::string trimmed_text = TrimTextWithEllipsis(items[i], rect_size.x - 2.0f);
        ImVec2 text_size = ImGui::CalcTextSize(trimmed_text.c_str());
        ImVec2 text_pos = ImVec2(
            p_min.x + (rect_size.x - text_size.x) * 0.5f,
            p_min.y + (rect_size.y - text_size.y) * 0.5f
        );

        ImVec4 text_color;
        if (disabled) {
            text_color = style.ImRadioButtonGroupDisabledTextColor;
        } else if (*current == i) {
            text_color = style.ImRadioButtonGroupActiveTextColor;
        } else {
            text_color = style.ImRadioButtonGroupInactiveTextColor;
        }

        draw_list->AddText(
            text_pos, 
            ImGui::GetColorU32(text_color), 
            trimmed_text.c_str()
        );

        ImGui::PopID();
        if (i + 1 < items_count) {
            if (horizontal) {
                ImGui::SameLine(0, style.ImRadioButtonGroupSpacing);
            } else {
                ImGui::Dummy(ImVec2(0.0f, style.ImRadioButtonGroupSpacing));
            }
        }
    }

    ImGui::PopID();
    return changed;
}

void ImWidgets::ProgressBar(const char* label, float min, float max, float current, ImToggleFlags flags) {
    ImGui::PushID(label);
    ImWidgetsStyle& style = GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    if ((flags & ImProgressBarFLags_NoLabel) && (flags & ImProgressBarFlags_PercentageLabel)) {
        flags &= ~ImProgressBarFlags_PercentageLabel;
    }

    float progress = 0.0f;
    if (max > min) {
        if (current > max) current = max;
        progress = (float)(current - min) / (float)(max - min);
        progress = std::clamp(progress, 0.0f, 1.0f);
    }

    char label_buf[64];
    if (!(flags & ImProgressBarFLags_NoLabel)) {
        if (flags & ImProgressBarFlags_PercentageLabel)
            snprintf(label_buf, sizeof(label_buf), "%.0f%%", progress * 100.0f);
        else
            snprintf(label_buf, sizeof(label_buf), "%f / %f", current, max);
    }

    ImVec2 pos = ImGui::GetCursorScreenPos();

    // --------------------------------------
    // CIRCLE PROGRESS BAR
    // --------------------------------------
    if (flags & ImProgressBarFlags_Circle) {
        ImVec2 center = ImVec2(pos.x + style.ImProgressBarRadius, pos.y + style.ImProgressBarRadius);

        // Background circle
        draw_list->PathClear();
        draw_list->PathArcTo(center, style.ImProgressBarCircleRadius, 0, IM_PI * 2, 64);
        draw_list->PathStroke(ImGui::GetColorU32(style.ImProgressBarBgColor), false, style.ImProgressBarCircleThickness);

        // Progress arc
        float start_angle = -IM_PI * 0.5f;
        float end_angle = start_angle + (IM_PI * 2.0f) * progress;
        draw_list->PathClear();
        draw_list->PathArcTo(center, style.ImProgressBarCircleRadius, start_angle, end_angle, 64);
        draw_list->PathStroke(ImGui::GetColorU32(style.ImProgressBarColor), false, style.ImProgressBarCircleThickness);

        if (!(flags & ImProgressBarFLags_NoLabel))
            DrawCenteredText(draw_list, center, label_buf, ImGui::GetColorU32(style.ImProgressBarTextColor));

        ImVec2 bounds(style.ImProgressBarRadius * 2, style.ImProgressBarRadius * 2);
        ImGui::Dummy(bounds);
    }
    // --------------------------------------
    // RECTANGLE PROGRESS BAR (default)
    // --------------------------------------
    else {
        ImVec2 size = style.ImProgressBarSize;
        ImVec2 p_min = pos;
        ImVec2 p_max(pos.x + size.x, pos.y + size.y);

        draw_list->AddRectFilled(p_min, p_max, ImGui::GetColorU32(style.ImProgressBarBgColor), 5.0f);
        draw_list->AddRectFilled(
            p_min,
            ImVec2(p_min.x + size.x * progress, p_max.y),
            ImGui::GetColorU32(style.ImProgressBarColor),
            5.0f
        );

        if (!(flags & ImProgressBarFLags_NoLabel)) {
            ImVec2 center(p_min.x + size.x * 0.5f, p_min.y + size.y * 0.5f);
            DrawCenteredText(draw_list, center, label_buf, ImGui::GetColorU32(style.ImProgressBarTextColor));
        }

        ImGui::Dummy(size);
    }

    ImGui::PopID();
}

void ImWidgets::Spinner(const char* label, ImSpinnerFlags flags) {
    ImGui::PushID(label);

    ImWidgetsStyle& style = GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float time = static_cast<float>(ImGui::GetTime());
    float rotation = time * style.ImSpinnerSpeed;
    float arc_angle = IM_PI * 1.5f + sinf(time * 3.0f) * 0.5f;

    const float spinner_diameter = style.ImSpinnerRadius * 2.0f;
    const ImVec2 spinner_size(spinner_diameter, spinner_diameter);
    const float text_height = ImGui::GetTextLineHeight();

    if (!(flags & ImSpinnerFlags_NoLabel)) {
        if (flags & ImSpinnerFlags_LabelLeft) {
            ImGui::TextUnformatted(label);
            ImGui::SameLine();
        }
    }

    ImVec2 pos = ImGui::GetCursorScreenPos();
    if (!(flags & ImSpinnerFlags_NoLabel)) {
        float vertical_offset = (text_height - spinner_diameter) * 0.5f;
        pos.y += vertical_offset > 0 ? vertical_offset : 0.0f;
        ImGui::SetCursorScreenPos(pos);
    }

    ImVec2 center = ImVec2(pos.x + style.ImSpinnerRadius, pos.y + style.ImSpinnerRadius);

    draw_list->PathClear();
    draw_list->PathArcTo(center, style.ImSpinnerRadius, rotation, rotation + arc_angle, style.ImSpinnerSegments);
    draw_list->PathStroke(ImGui::GetColorU32(style.ImSpinnerColor), false, style.ImSpinnerThickness);

    ImGui::Dummy(spinner_size);

    if (!(flags & ImSpinnerFlags_NoLabel) && (flags & ImSpinnerFlags_LabelRight)) {
        ImGui::SameLine();
        ImGui::TextUnformatted(label);
    }

    ImGui::PopID();
}

void ImWidgets::Calendar(const char* label, int& day, int& month, int& year, ImCalendarFlags flags)
{
    ImGui::PushID(label);

    ImWidgetsStyle& style = GetStyle();

    time_t t = time(nullptr);
    const tm* now = localtime(&t);

    static int year_ = now->tm_year + 1900;
    static int month_ = now->tm_mon;
    static int day_ = now->tm_mday;

    auto normalize_month = [&]() {
        if (month_ > 11) {
            month_ = 0;
            year_++;
        } else if (month_ < 0) {
            month_ = 11;
            year_--;
        }
    };

    // Month navigation buttons
    ImGui::SetCursorPosX((style.ImCalendarOuterSize.x / 2) - 5);
    if (ImGui::Button("<")) {
        month_--;
        day_ = 1;
        normalize_month();
    }
    ImGui::SameLine();
    ImGui::Text("%d / %d / %d", day_ , month_ + 1, year_);
    ImGui::SameLine();
    if (ImGui::Button(">")) {
        month_++;
        day_ = 1;
        normalize_month();
    }
    // Determine days in month
    int days_in_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if ((year_ % 4 == 0 && year_ % 100 != 0) || year_ % 400 == 0)
        days_in_month[1] = 29;

    // Compute first day of the current month
    tm first_day = {};
    first_day.tm_year = year_ - 1900;
    first_day.tm_mon  = month_;
    first_day.tm_mday = 1;
    mktime(&first_day);
    int start_weekday = first_day.tm_wday;
    int total_rows = (start_weekday + days_in_month[month_] + 6) / 7;

    // Apply styles
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, style.ImCalendarBorderStrongColor);
    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, style.ImCalendarBorderLightColor);
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, style.ImCalendarHoveredCellColor);
    ImGui::PushStyleColor(ImGuiCol_Header, style.ImCalendarSelectedCellColor);
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, style.ImCalendarBgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.ImCalendarTextColor);

    if (ImGui::BeginTable("CalendarTable", 7, ImGuiTableFlags_Borders, style.ImCalendarOuterSize, style.ImCalendarInnerWidth)) {
        const char* weekdays[] = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };
        for (int i = 0; i < 7; i++)
            ImGui::TableNextColumn(), ImGui::Text("%s", weekdays[i]);

        int day_counter = 1;
        for (int week = 0; week < total_rows; week++) {
            for (int col = 0; col < 7; col++) {
                ImGui::TableNextColumn();
                int cell_index = week * 7 + col;
                if (cell_index < start_weekday || day_counter > days_in_month[month_]) {
                    ImGui::Text(" ");
                } else {
                    bool is_today = (day_counter == day_);
                    if (ImGui::Selectable(std::to_string(day_counter).c_str(), is_today, 0, ImVec2(28, 24))) {
                        year = year_;
                        month = month_ + 1;
                        day = day_counter;
                        day_ = day_counter;
                    }
                    day_counter++;
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::PopStyleColor(6);
    ImGui::PopID();
}

void ImWidgets::TabStrip(const char* label, ImGuiTabBarFlags flags) {
    ImGui::PushID(label);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->PathClear();
    ImVec2 p = ImGui::GetCursorScreenPos();
    float width = ImGui::GetContentRegionAvail().x;
    float thickness = 1.0f;

    draw_list->AddLine(
        ImVec2(p.x, p.y),
        ImVec2(p.x + width, p.y),
        IM_COL32(255, 255, 255, 255),
        thickness
    );

    ImGui::Dummy(ImVec2(0.0f, thickness + 2.0f)); 

    ImGui::PopID();
}

void ImWidgets::Tab(const char* label, bool* p_open) {
    
    ImGui::PushID(label);
}