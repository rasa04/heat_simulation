#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "heat_cpu.h"
#include "heat_gpu.h"   // stub for now
#include "utils/utils.h"

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;
static const int HISTORY_SIZE = 100;

int main() {
    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Heat Diffusion CPU vs GPU", nullptr, nullptr);
    if (!window) { std::cerr << "Window creation failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    int gridSize = 256; float heatSourceTemp = 100.f;
    std::vector<float> grid(gridSize*gridSize), nextGrid(gridSize*gridSize);
    initHeat(grid, gridSize, heatSourceTemp);

    std::vector<float> cpuHist, gpuHist;
    cpuHist.reserve(HISTORY_SIZE);
    gpuHist.reserve(HISTORY_SIZE);

    GLuint texID; glGenTextures(1,&texID);
    float cpuTime=0.f, gpuTime=0.f;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI PANEL
        ImGui::Begin("Control Panel");
        if(ImGui::SliderInt("Grid Size", &gridSize, 64, 1024)){
            grid.assign(gridSize*gridSize,0.f);
            nextGrid.assign(gridSize*gridSize,0.f);
            initHeat(grid, gridSize, heatSourceTemp);
            cpuHist.clear(); gpuHist.clear();
        }
        if(ImGui::SliderFloat("Heat Source Temp", &heatSourceTemp, 10.f, 500.f)){
            initHeat(grid, gridSize, heatSourceTemp);
        }
        if(ImGui::Button("Reset")){
            grid.assign(gridSize*gridSize,0.f);
            initHeat(grid, gridSize, heatSourceTemp);
            cpuHist.clear(); gpuHist.clear();
        }
        if(ImGui::Button("Run Step")){
            auto t0=std::chrono::high_resolution_clock::now();
            stepHeatCPU(grid, nextGrid, gridSize);
            auto t1=std::chrono::high_resolution_clock::now();
            cpuTime=std::chrono::duration<float,std::milli>(t1-t0).count();

            auto g0=std::chrono::high_resolution_clock::now();
            stepHeatGPU(grid.data(), nextGrid.data(), gridSize);
            auto g1=std::chrono::high_resolution_clock::now();
            gpuTime=std::chrono::duration<float,std::milli>(g1-g0).count();

            std::swap(grid,nextGrid);
            if(cpuHist.size()==HISTORY_SIZE) cpuHist.erase(cpuHist.begin());
            if(gpuHist.size()==HISTORY_SIZE) gpuHist.erase(gpuHist.begin());
            cpuHist.push_back(cpuTime);
            gpuHist.push_back(gpuTime);
        }
        ImGui::Text("CPU:  %.2f ms", cpuTime);
        ImGui::Text("GPU:  %.2f ms", gpuTime);
        ImGui::Text("Speedup: %.1fx", (cpuTime>0&&gpuTime>0)? cpuTime/gpuTime : 0.f);

        if(!cpuHist.empty()){
            float maxC=*std::max_element(cpuHist.begin(), cpuHist.end());
            ImGui::PlotLines("CPU History", cpuHist.data(), cpuHist.size(),0,nullptr,0.f,maxC, ImVec2(0,80));
        }
        if(!gpuHist.empty()){
            float maxG=*std::max_element(gpuHist.begin(), gpuHist.end());
            ImGui::PlotLines("GPU History", gpuHist.data(), gpuHist.size(),0,nullptr,0.f,maxG, ImVec2(0,80));
        }
        ImGui::End();

        // RENDER TEXTURE
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        updateTexture(texID, grid.data(), gridSize);
        renderTexture(texID, gridSize, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
