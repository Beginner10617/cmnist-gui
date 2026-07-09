#!/bin/zsh
g++ build/*.o SDL/build/libSDL2.a \
    -framework Cocoa \
    -framework IOKit \
    -framework CoreVideo \
    -framework CoreAudio \
    -framework AudioToolbox \
    -framework AudioUnit \
    -framework ForceFeedback \
    -framework Carbon \
    -framework CoreHaptics \
    -framework GameController \
    -framework Metal \
    -framework QuartzCore \
    -framework CoreGraphics \
    -framework CoreFoundation \
    -framework OpenGL \
    -o build/cmnist-gui
