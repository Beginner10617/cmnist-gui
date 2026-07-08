extern "C" {
#include "cmnist/neuron_utils.h"
}
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
int main() {
  Value *v = createNewValue(1.2f, "hello");
  return 0;
}
