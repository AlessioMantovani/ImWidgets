#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "im_widgets.h"
#include "../imgui/imgui_internal.h"

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1000, 700, "ImWidgets Demo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Demo state
    static bool enabled = false;
    static int selected = 1;
    const char* options[] = { "Low", "Medium", "High" };
    static float progress = 0.0f;
    static int day = 0, month = 0, year = 0;

    // Customize styles
    ImWidgetsStyle& style = ImWidgets::GetStyle();
    style.ImProgressBarSize = ImVec2(250, 24);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        progress += 0.4f;
        if (progress > 100.0f) progress = 0.0f;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Demo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::Text("ImWidgets Demo Gallery");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 15));

        // Toggle Switch
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Toggle Switch");
        ImWidgets::ToggleSwitch("Enable Feature", &enabled);
        ImGui::Text("Feature is %s", enabled ? "Enabled" : "Disabled");
        ImGui::Dummy(ImVec2(0, 20));

        // Radio Button Group
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Radio Button Group");
        ImWidgets::RadioButtonGroup("Quality", &selected, options, 3);
        ImGui::Text("Selected: %s", options[selected]);
        ImGui::Dummy(ImVec2(0, 20));

        // Progress Bars
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Progress Bar");
        ImWidgets::ProgressBar("Loading", 0, 100, progress, ImProgressBarFlags_PercentageLabel);
        
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Dummy(ImVec2(20, 0));
        ImGui::SameLine();
        ImWidgets::ProgressBar("Circular", 0, 100, progress, ImProgressBarFlags_Circle | ImProgressBarFlags_PercentageLabel);
        ImGui::Dummy(ImVec2(0, 20));

        // Spinner
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Spinner");
        ImWidgets::Spinner("Processing...", ImSpinnerFlags_LabelRight);
        ImGui::Dummy(ImVec2(0, 20));

        // Calendar
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Calendar");
        ImWidgets::Calendar("Date Picker", day, month, year);
        ImGui::Text("Selected Date: %02d/%02d/%04d", day, month, year);

        ImGui::End();

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}