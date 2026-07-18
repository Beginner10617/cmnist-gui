# CMNIST-GUI

A small graphical frontend for the [**CMNIST**](https://github.com/Beginner10617/CMNIST.git) handwritten digit recognition project, built using **C++**, **Dear ImGui**, **SDL2**, and **OpenGL 3**.

The goal of this project is to provide an interactive interface where users can draw handwritten digits and have them classified using the neural network implementation from the original CMNIST project, which is written entirely in C.

## Features

* Dear ImGui based graphical interface
* Reuses the existing C implementation of the neural network
* Single executable combining C and C++ code

## Project Structure

```text
.
├── build/          # Object files (ignored by git)
├── cmnist/         # Original C implementation
├── imgui/          # Dear ImGui source
├── SDL/            # SDL2 source
├── bash/           # Bash scripts for build
├── main.cpp        # GUI entry point
├── build_mac.sh    # Bash script to build and link to (my local) mac environment
├── FORMAT.txt      # Format description for model parameter files (if u want to add your own)
├── model.txt       # Trained neural network weights and biases
├── DEMO.gif        # A demo of the gui application
└── README.md       # README u r reading rn
```

## Build

To build various components of the project, run the following bash scripts from the root:
```bash
source bash/build_imgui.sh   # Build Dear ImGui
source bash/build_sdl2.sh    # Build SDL2
source bash/compile_c.sh     # Compile the C backend
source bash/compile_cpp.sh   # Compile the C++ frontend
source bash/link_mac.sh      # Link all object files, SDL2, and macOS system frameworks
```

Or instead you can use a single script :
```bash
source build_mac.sh
```

Note that for other platforms, the link script won't be valid

## Demo
<div align="center">
<img src="DEMO.gif" alt="Alt text" width="400" height="250">
</div>
