# ImWidgets

A lightweight collection of custom widgets for [Dear ImGui](https://github.com/ocornut/imgui), providing modern UI components with smooth animations and customizable styling.

## Features

ImWidgets extends Dear ImGui with the following components:

- **Toggle Switch** - Animated ON/OFF switch with customizable colors
- **Radio Button Group** - Horizontal or vertical button groups for single selection
- **Progress Bar** - Both rectangular and circular progress indicators
- **Spinner** - Animated loading spinner with configurable placement
- **Calendar** - Interactive date picker with month navigation

All widgets support customization through a unified style system and feature flags for fine-grained control.

## Installation

ImWidgets is header-only style - simply include the files in your project:

1. Copy `im_widgets.h` and `im_widgets.cpp` into your project directory
2. Add both files to your build system (CMake, Makefile, Visual Studio project, etc.)
3. Include the header in your code:

```cpp
#include "im_widgets.h"
```

**Requirements:**
- Dear ImGui (any recent version)
- C++17 or later

## Usage Examples

### Toggle Switch

Create an animated toggle switch for boolean values:

```cpp
static bool enabled = false;
if (ImWidgets::ToggleSwitch("Enable Feature", &enabled)) {
    // Value changed
}

// Without label
ImWidgets::ToggleSwitch("##toggle", &enabled, ImToggleFlags_NoLabel);

// Disabled state
ImWidgets::ToggleSwitch("Disabled", &enabled, ImToggleFlags_Disabled);
```

### Radio Button Group

Display a group of mutually exclusive options:

```cpp
static int selected = 0;
const char* options[] = { "Option 1", "Option 2", "Option 3" };
if (ImWidgets::RadioButtonGroup("Select", &selected, options, 3)) {
    // Selection changed
}

// Vertical layout
ImWidgets::RadioButtonGroup("Vertical", &selected, options, 3, 
    ImRadioButtonGroupFlags_Vertical);
```

### Progress Bar

Show task progress with rectangular or circular styles:

```cpp
static float progress = 0.0f;
progress += 0.001f; // Increment over time

// Rectangular with percentage
ImWidgets::ProgressBar("Loading", 0.0f, 100.0f, progress, 
    ImProgressBarFlags_PercentageLabel);

// Circular progress
ImWidgets::ProgressBar("Upload", 0.0f, 100.0f, progress, 
    ImProgressBarFlags_Circle | ImProgressBarFlags_PercentageLabel);
```

### Spinner

Display an animated loading indicator:

```cpp
// Basic spinner with label
ImWidgets::Spinner("Loading");

// Label on left
ImWidgets::Spinner("Processing", ImSpinnerFlags_LabelLeft);

// No label
ImWidgets::Spinner("##spinner", ImSpinnerFlags_NoLabel);
```

### Calendar

Interactive date picker with month navigation:

```cpp
static int day = 1, month = 1, year = 2025;
ImWidgets::Calendar("Select Date", day, month, year);
// Returns selected date in day, month, year parameters
```

## Demo

This is a dummy demo of what the library can currently achieve

![Demo](https://github.com/AlessioMantovani/ImWidgets/blob/8b28cfe15b76c0f88035d85f89cd0cab53b70da5/assets/demo.mp4)

## Customization
Customize widget appearance through the global style:

```cpp
ImWidgetsStyle& style = ImWidgets::GetStyle();

// Customize toggle switch
style.ImToggleSwitchOnColor = ImVec4(0.0f, 0.8f, 0.3f, 1.0f);
style.ImToggleSwitchSize = ImVec2(50, 25);

// Customize progress bar
style.ImProgressBarColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
style.ImProgressBarSize = ImVec2(200, 30);

// Customize spinner
style.ImSpinnerRadius = 15.0f;
style.ImSpinnerSpeed = 3.0f;
```

See `im_widgets.h` for all available style options.

## API Reference

### Toggle Switch
```cpp
bool ToggleSwitch(const char* label, bool* v, ImToggleFlags flags = ImToggleFlags_None);
```

**Flags:** `ImToggleFlags_NoLabel`, `ImToggleFlags_Disabled`, `ImToggleFlags_NoAnimation`

### Radio Button Group
```cpp
bool RadioButtonGroup(const char* label, int* current, const char* const items[], 
                     int items_count, ImRadioButtonGroupFlags flags = ImRadioButtonGroupFlags_None);
```

**Flags:** `ImRadioButtonGroupFlags_Vertical`, `ImRadioButtonGroupFlags_Disabled`

### Progress Bar
```cpp
void ProgressBar(const char* label, float min, float max, float current, 
                ImToggleFlags flags = ImProgressBarFlags_None);
```

**Flags:** `ImProgressBarFlags_NoLabel`, `ImProgressBarFlags_PercentageLabel`, `ImProgressBarFlags_Circle`

### Spinner
```cpp
void Spinner(const char* label, ImSpinnerFlags flags = ImSpinnerFlags_None);
```

**Flags:** `ImSpinnerFlags_NoLabel`, `ImSpinnerFlags_LabelLeft`, `ImSpinnerFlags_LabelRight`

### Calendar
```cpp
void Calendar(const char* label, int& day, int& month, int& year, 
             ImCalendarFlags flags = ImCalendarFlags_None);
```

## License

This library is provided as-is. Please ensure compliance with Dear ImGui's license when using this code.

## Roadmap

Planned features:
- File picker dialog
- Collapsible card component
- Tab strip with close buttons (browser-style)
- Notification toasts
- Rich text rendering
