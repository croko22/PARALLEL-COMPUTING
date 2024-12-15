#!/bin/bash

# Compile the programs
gcc -o pi pi.c -fopenmp
gcc -o matrix matrix.c -fopenmp
gcc -o heat heat.c -fopenmp

# Run the programs and measure their execution time
echo "Running pi program..."
./pi.exe

echo "Running matrix program..."
./matrix.exe

echo "Running heat program..."
./heat.exe

echo "All programs have been executed."

# Run the Python script to plot the results
# echo "Plotting results with Python..."
# python plot_results.py