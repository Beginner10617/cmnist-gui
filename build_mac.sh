#!/bin/zsh
# Use this script only once to build the entire project
# once dependencies are built, only calling relavent scripts on change is advised
set -e
echo "compiling cmnist..."
source bash/compile_c.sh
echo "building sdl..."
source bash/build_sdl2.sh
echo "building imgui..."
source bash/build_imgui.sh
echo "compiling main..."
source bash/compile_cpp.sh
echo "linking..."
source bash/link_mac.sh
echo "DONE"
