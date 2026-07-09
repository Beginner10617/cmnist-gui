#!/bin/zsh
gcc -c cmnist/fileSystem.c -o build/fileSystem.o
gcc -c cmnist/neuron_utils.c -o build/neuron_utils.o
gcc -c cmnist/neuron.c -o build/neuron.o
gcc -c cmnist/utils.c -o build/utils.o
