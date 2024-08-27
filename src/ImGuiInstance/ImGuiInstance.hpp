#pragma once
#include <glad/glad.h>
//
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <string>

struct ImGuiInstance {
private:
    GLFWwindow* window;

public:
    ImGuiInstance(std::size_t width, std::size_t height, std::string const& windowName) {
        glfwSetErrorCallback([](int error, char const* description) {
            std::fprintf(stderr, "glfw error %d: %s\n", error, description);
            std::terminate();
        });

        if(!glfwInit()) {
            std::fprintf(stderr, "glfwInit failed\n");
            std::terminate();
        }

        // GL 3.0 + GLSL 130
        char const* const glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        window = glfwCreateWindow(
          static_cast<int>(width),
          static_cast<int>(height),
          windowName.c_str(),
          nullptr,
          nullptr);
        if(window == nullptr) {
            std::fprintf(stderr, "glfwCreateWindow failed\n");
            std::terminate();
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::fprintf(stderr, "gladLoadGLLoader failed\n");
            std::terminate();
        }

        int screen_width;
        int screen_height;
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        // Specific Font for OmnAIView 
        font2 = io.Fonts->AddFontFromFileTTF("../fonts/Inter_24pt-Medium.ttf", 24.0f); 
        if (font2 == nullptr) {
                std::fprintf(stderr, "Failed to load font: ../../fonts/Inter_24pt-Medium.ttf\n");
                std::terminate();
        }

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    template<typename FrameFunction>
    bool run(FrameFunction&& frameFunction) {
        if(!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            std::invoke(std::forward<FrameFunction>(frameFunction));

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            return true;
        } else {
            return false;
        }
    }

    ~ImGuiInstance() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ImPlot::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};
