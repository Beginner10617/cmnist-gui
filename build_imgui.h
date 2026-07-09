g++ -c imgui/imgui.cpp -o build/imgui.o -I./imgui/ -I./SDL/include
g++ -c imgui/imgui_draw.cpp -o build/imgui_draw.o -I./imgui/ -I./SDL/include
g++ -c imgui/imgui_tables.cpp -o build/imgui_tables.o -I./imgui/ -I./SDL/include
g++ -c imgui/imgui_widgets.cpp -o build/imgui_widgets.o -I./imgui/ -I./SDL/include
g++ -c imgui/backends/imgui_impl_sdl2.cpp -o build/imgui_impl_sdl2.o -I./imgui/ -I./SDL/include
g++ -c imgui/backends/imgui_impl_opengl3.cpp -o build/imgui_impl_opengl3.o -I./imgui/ -I./SDL/include
