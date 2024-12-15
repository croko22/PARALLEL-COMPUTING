import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo CSV
data = pd.read_csv('heat_results.csv')
# data = pd.read_csv('heat_results_dense.csv')

# Graficar el tiempo de ejecución en función de N
plt.figure(figsize=(12, 6))

# Gráfico 1: Tiempo de ejecución vs N para diferentes iteraciones
plt.subplot(1, 2, 1)
for iteration in data['iterations'].unique():
    subset = data[data['iterations'] == iteration]
    plt.plot(subset['N'], subset['execution_time'], marker='o', label=f'Iterations={iteration}')
plt.xlabel('Tamaño de la rejilla (N)')
plt.ylabel('Tiempo de ejecución (segundos)')
plt.title('Tiempo de ejecución vs Tamaño de la rejilla')
plt.legend()
plt.xscale('log')  # Escala logarítmica para N
plt.yscale('log')  # Escala logarítmica para el tiempo

# Gráfico 2: Tiempo de ejecución vs Iteraciones para diferentes N
plt.subplot(1, 2, 2)
for N in data['N'].unique():
    subset = data[data['N'] == N]
    plt.plot(subset['iterations'], subset['execution_time'], marker='o', label=f'N={N}')
plt.xlabel('Número de iteraciones')
plt.ylabel('Tiempo de ejecución (segundos)')
plt.title('Tiempo de ejecución vs Número de iteraciones')
plt.legend()
plt.yscale('log')  # Escala logarítmica para el tiempo

plt.tight_layout()
plt.show()