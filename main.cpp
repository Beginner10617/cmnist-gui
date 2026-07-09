#include <cstdlib>
extern "C" {
#include "cmnist/neuron_utils.h"
}
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include <stdio.h>
int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    printf("ERROR: %s\n", SDL_GetError());
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
  SDL_Window *window = SDL_CreateWindow("CMNIST", 50, 50, 192, 108, flags);
  return 0;
}
