#include <cstddef>
#include <cstdlib>
extern "C" {
#include "cmnist/neuron_utils.h"
}
#define IMGUI_DEFINE_MATH_OPERATORS
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include <stdio.h>
int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    printf("Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  const char *glsl_version = "#version 330";

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // Version 4.1
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  float content_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
  SDL_WindowFlags flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  int factor = 50;
  SDL_Window *window =
      SDL_CreateWindow("CMNIST", 50, 50, 16 * factor, 9 * factor, flags);
  if (window == nullptr) {
    printf("Error while calling SDL_CreateWindow: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_GLContext opengl_context = SDL_GL_CreateContext(window);
  if (opengl_context == nullptr) {
    printf("Error while calling SDL_GL_CreateContext: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_GL_MakeCurrent(window, opengl_context); // explicit
  SDL_GL_SetSwapInterval(1);                  // enable vsync

  // setup Dear Imgui context
  IMGUI_CHECKVERSION(); // verifying vrsion compatibility
  ImGui::CreateContext();

  // io config
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // theme/style
  ImGui::StyleColorsDark(); // Dark mode
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(content_scale);
  style.FontScaleDpi = content_scale;

  io.ConfigDpiScaleFonts = true;
  io.ConfigDpiScaleViewports = true;

  // needed for multi-viewports
  if (io.ConfigFlags & io.ConfigDpiScaleViewports) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // setting up rendering backends
  ImGui_ImplSDL2_InitForOpenGL(window, opengl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // custom fonts can be loaded here (default will be used for now)

  ImVec4 backgrnd_color = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  io.IniFilename = nullptr; // no ui persistence needed

  // main-loop
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        running = false;
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window))
        // close if the main window is closed
        running = false;
    }
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10); // 10ms wait if minimised
      continue;
    }

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Add widgets and windows
    ImGui::SetNextWindowPos(ImVec2(60, 100), ImGuiCond_Once);
    ImGui::Begin("Canvas");
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImVec2 canvas_dimension(7 * factor, 8 * factor * 10 / 12);
    draw_list->AddRectFilled(cursor, cursor + canvas_dimension,
                             IM_COL32(255, 255, 255, 255));
    ImGui::InvisibleButton("Canvas", canvas_dimension);
    ImGui::End();

    // rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(backgrnd_color.x * backgrnd_color.w,
                 backgrnd_color.y * backgrnd_color.w,
                 backgrnd_color.z * backgrnd_color.w, backgrnd_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // rendering other non-main viewports (required for multi-viewports)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      SDL_Window *og_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext og_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(og_window, og_context);
    }

    // swap window call for sdl
    SDL_GL_SwapWindow(window);
  }
  return 0;
}
