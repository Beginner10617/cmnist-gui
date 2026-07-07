# CMNIST-GUI

A small graphical frontend for the [**CMNIST**](https://github.com/Beginner10617/CMNIST.git) handwritten digit recognition project, built using **C++**, **Dear ImGui**, **SDL2**, and **OpenGL 3**.

The goal of this project is to provide an interactive interface where users can draw handwritten digits and have them classified using the neural network implementation from the original CMNIST project, which is written entirely in C.

## Features

* Dear ImGui based graphical interface
* Reuses the existing C implementation of the neural network
* Single executable combining C and C++ code
* Lightweight and cross-platform

## Project Structure

```text
.
├── build/          # Object files (ignored by git)
├── cmnist/         # Original C implementation
├── imgui/          # Dear ImGui source
├── main.cpp        # GUI entry point
├── compile_c.sh    # Compiles the C sources into object files
└── model.txt       # Trained neural network weights and biases
```

## Build

Currently, the C backend is compiled separately into object files using:

```bash
source compile_c.sh
```

The GUI frontend is written in C++ and links against these object files to produce the final executable.

## Status

**Incomplete**

Current focus:

* Setting up the project structure
* Integrating the existing CMNIST backend with Dear ImGui
* Building the drawing canvas for handwritten digit input

Rough sketch for the UI:
```
+-------------------------------------------------------+
| File   Help                         Brush Size [====] |
+---------------------------+---------------------------+
|                           |                           |
|      Drawing Canvas       | Prediction                |
|                           |                           |
|                           | Digit: 7                  |
|                           | Activation: 0.982         |
|                           |                           |
|                           | Top-5                     |
|                           | 7 -> 0.982                |
|                           | 1 -> -0.871               |
|                           | 9 -> 0.004                |
+---------------------------+---------------------------+
| Clear                                         Predict |
+-------------------------------------------------------+
```
