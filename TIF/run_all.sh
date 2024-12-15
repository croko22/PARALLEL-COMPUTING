#!/bin/bash

# Compile the programs
gcc -o pi pi.c -fopenmp
gcc -o matrix matrix.c -fopenmp
gcc -o heat heat.c -fopenmp

# Run the programs and measure their execution time
echo "Running pi program..."
./pi.exe
py pi.py

echo "Running matrix program..."
./matrix.exe
py matrix.py

echo "Running heat program..."
./heat.exe
py heat.py

echo "All programs have been executed."