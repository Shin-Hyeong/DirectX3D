#include "pch.h"
#include "ImGUIDemo.h"


void ImGUIDemo::Init()
{
 
}

void ImGUIDemo::Update()
{
    // UI
    Test();
}

void ImGUIDemo::Render()
{

}

void ImGUIDemo::Test()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // 테스트를 하기 위한 Gui의 Demo Window 보일지 여부
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        // 동일한 Begin("") 값을 가지면 동일한 창에 추가 됨.
        // 별도의 창을 만들고 싶으면 다른 char 값을 넣어야함
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // show_demo_windwo에 Checkbox 값을 전달 할 수 있음
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        // float 타입으로 0 ~ 1까지 슬라이드 바 생성
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        // 같은 줄에 기능 추가
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 커스텀 Window
    {
        ImGui::Begin("New Window");

        ImGui::Text("GUI can't use Korean");

        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}