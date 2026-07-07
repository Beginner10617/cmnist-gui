#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include "stdio.h"
#include "neuron.h"

// Save MLP parameters in a textfile
void saveMLP(MLP* mlp, char* Fname);

// Load MLP from parameters specified in a textfile
MLP* loadMLP(char* Fname);

#endif
