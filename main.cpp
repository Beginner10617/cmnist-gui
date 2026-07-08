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
  return 0;
}
