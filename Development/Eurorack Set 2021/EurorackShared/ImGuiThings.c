ImFont* pFont;
ImFont* pFontBold;
#include <math.h>
#include <stdio.h>

bool Knob(const char* label, float* value_p, float minv, float maxv)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float line_height = ImGui::GetTextLineHeight();

    ImVec2 p = ImGui::GetCursorScreenPos();
    float sz = 76.0f;
    float radius = sz * 0.5f;
    ImVec2 center = ImVec2(p.x + radius, p.y + radius + line_height);
    float val1 = (value_p[0] - minv) / (maxv - minv);
    char textval[32];
//    sprintf_s(textval, 32, "%04.1f", value_p[0]);
    snprintf(textval, 32, "%04.1f", value_p[0]) ;
    ImVec2 textpos = p;

    float gamma = (float)(3.1415f) / 4.0f;//0 value in knob
    float alpha = (3.1415f - gamma) * val1 * 2 + gamma;


    ImGui::InvisibleButton(label, ImVec2(sz, sz + line_height + style.ItemInnerSpacing.y));

    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();
    bool touched = false;

    if (is_active)
    {
        touched = true;
        ImVec2 mp = ImGui::GetIO().MousePos;
        alpha = (float)atan2f(mp.x - center.x, center.y - mp.y) + 3.1415f;
        alpha = __max(gamma, __min(2.0f * 3.1415f - gamma, alpha));
        float value = 0.5f * (alpha - gamma) / (3.1415f - gamma);
        value_p[0] = value * (maxv - minv) + minv;
    }

    ImU32 col32 = is_active ? ImColor(255, 250, 100) : is_hovered ? ImColor(255, 250, 120) : ImColor(255, 250, 0);
    ImU32 col32line = ImColor(255, 255, 255);
    ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();


    radius *= 0.7;
    float x2 = -sinf(alpha) * radius * 1.15 + center.x;
    float y2 = cosf(alpha) * radius * 1.15 + center.y;


    draw_list->AddCircleFilled(center, radius, IM_COL32(0, 0, 0, 255), 32);
    draw_list->AddCircleFilled(center, radius * 0.9, IM_COL32(10, 10, 10, 255), 32);
    draw_list->AddCircleFilled(center, radius * 0.72, col32, 32);
    draw_list->AddLine(center, ImVec2(x2, y2), col32line, 4);
    //draw_list->AddText(textpos, col32text, textval);

    auto S = ImGui::CalcTextSize(label , NULL, true);
    draw_list->AddText(ImVec2(p.x + sz / 2 - S.x / 2, p.y), col32text, label, strstr(label,"##"));

    return touched;
}

void SetupTheme(float scaling)
{

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
        // Setup style
    ImGui::StyleColorsLight();

    pFont = io.Fonts->AddFontFromFileTTF("Dosis-Medium.ttf", 12.0f * scaling);
    pFontBold = io.Fonts->AddFontFromFileTTF("Dosis-Bold.ttf", 12.0f*scaling);

    ImGui::SFML::UpdateFontTexture(); 

    ImGui::GetStyle().ItemInnerSpacing = ImVec2(5, 5);;
    ImGui::GetStyle().ItemSpacing = ImVec2(5, 5);
    ImGui::GetStyle().FramePadding = ImVec2(5, 5);
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, .800f);


    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui::GetStyle().ItemInnerSpacing = ImVec2(1, 1);
    ImGui::GetStyle().ItemSpacing = ImVec2(1, 1);
    ImGui::GetStyle().ScrollbarSize = 12;
    ImGui::GetStyle().WindowPadding = ImVec2(2, 2);
    ImGui::GetStyle().FramePadding = ImVec2(1, 1);
    ImGui::GetStyle().WindowRounding = 4;
    ImGui::GetStyle().ChildRounding = 0;
    ImGui::GetStyle().FrameRounding = 3;


    colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.87f, 0.91f, 0.91f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.89f, 0.89f, 0.89f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.90f, 0.90f, 0.90f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.79f, 0.79f, 0.79f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.97f, 0.90f, 0.19f, 0.21f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.62f, 0.64f, 0.41f, 0.69f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.46f, 0.02f, 0.83f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.95f, 0.83f, 0.23f, 0.70f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.31f, 0.00f, 0.55f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.80f, 0.80f, 0.85f, 0.80f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.72f, 0.72f, 0.72f, 0.50f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.68f, 0.69f, 0.63f, 0.81f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.80f, 0.40f, 0.86f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.97f, 0.97f, 0.00f, 0.60f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.76f, 0.39f, 0.60f);
    colors[ImGuiCol_Button] = ImVec4(0.78f, 0.78f, 0.78f, 0.62f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.90f, 0.78f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.91f, 0.17f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.09f, 0.47f, 0.18f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.73f, 0.89f, 0.10f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.89f, 0.92f, 0.05f, 0.80f);
    colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.31f, 0.03f, 0.23f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.49f, 0.81f, 0.03f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.79f, 0.90f, 0.70f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.93f, 0.97f, 0.08f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.99f, 0.78f, 0.90f);
    colors[ImGuiCol_Tab] = ImVec4(0.81f, 0.81f, 0.81f, 0.79f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.90f, 0.84f, 0.45f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.89f, 0.12f, 0.84f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.63f, 0.62f, 0.48f, 0.82f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.65f, 0.59f, 0.35f, 0.84f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.53f, 0.80f, 0.20f, 0.79f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.83f, 1.00f, 0.14f, 0.80f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f); 
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.78f, 0.78f, 0.78f, 0.35f);
}
