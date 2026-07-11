#include <cstddef>
#include <cstdlib>
#include <ios>
extern "C" {
#include "cmnist/neuron_utils.h"
}
#define IMGUI_DEFINE_MATH_OPERATORS
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "imgui_internal.h"
#include <stdio.h>
#include <vector>

struct Stroke {
  std::vector<ImVec2> points;
};

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
  SDL_WindowFlags SDL_WINDOW_FLAGS =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
                        SDL_WINDOW_RESIZABLE);
  int factor = 60;
  SDL_Window *window = SDL_CreateWindow("CMNIST", 50, 50, 16 * factor,
                                        9 * factor, SDL_WINDOW_FLAGS);
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
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // theme/style
  ImGui::StyleColorsDark(); // Dark mode
  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowBorderSize = 0.0f;
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

  // for pencil strokes
  std::vector<Stroke> strokes;
  bool drawing = false;
  ImVec2 current_posn;
  unsigned int current_stroke_index;
  float BRUSH_WIDTH = 2.0f;
  ImVec2 cursor;
  bool first_frame = true;
  // main-loop
  bool running = true;
  bool show_help = false;
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

    // docking
    ImGuiID dockspace_id = ImGui::GetID("My Dockspace");
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
      ImGuiID dock_id_right = 0;
      ImGuiID dock_id_main = dockspace_id;
      ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.50f,
                                  &dock_id_main, &dock_id_right);
      ImGui::DockBuilderDockWindow("Canvas", dock_id_main);
      ImGui::DockBuilderDockWindow("Prediction", dock_id_right);
      ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::DockSpaceOverViewport(dockspace_id, viewport,
                                 ImGuiDockNodeFlags_PassthruCentralNode);

    if (show_help) {
      ImGui::Begin("Help", &show_help, ImGuiWindowFlags_NoCollapse);
      ImGui::Text("Blah blah blah blah blah ... (under construction)");
      ImGui::End();
    }

    ImGui::Begin("Prediction", nullptr);
    ImGui::End();

    // Canvas
    ImGui::Begin("Canvas", nullptr);
    if (ImGui::Button("File")) {
      // load model from text file
    }
    ImGui::SameLine();
    if (ImGui::Button("Help")) {
      // load help etc
      show_help = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      strokes.clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Predict")) {
      // call backend to display prediction
    }
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    cursor = ImGui::GetCursorScreenPos();
    ImVec2 canvas_dimension(15 * factor / 2, 9 * factor * 10 / 12);
    draw_list->AddRectFilled(cursor, cursor + canvas_dimension, IM_COL32_WHITE);
    ImGui::InvisibleButton("Canvas", canvas_dimension);
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive(); // mouse click
    current_posn = ImGui::GetIO().MousePos;
    if (hovered && active) {
      if (!drawing) {
        current_stroke_index = strokes.size();
        strokes.emplace_back();
      }
      drawing = true;
    }
    if (!active)
      drawing = false;

    if (drawing)
      strokes[current_stroke_index].points.push_back(current_posn);
    for (const Stroke &stroke : strokes) {
      for (int i = 1; i < stroke.points.size(); i++)
        draw_list->AddLine(stroke.points[i - 1], stroke.points[i],
                           IM_COL32_BLACK, BRUSH_WIDTH);
    }
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

    // swap window call for sdl
    SDL_GL_SwapWindow(window);
  }
  return 0;
}
