import pandas as pd
import matplotlib.pyplot as plt
import os

# Crear carpeta para guardar las imágenes
output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Leer el archivo CSV
data = pd.read_csv('heat_results.csv')

# Crear una figura con dos subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 6))

# Gráfico 1: Tiempo de ejecución vs N para diferentes iteraciones
for iteration in data['iterations'].unique():
    subset = data[data['iterations'] == iteration]
    ax1.plot(subset['N'], subset['execution_time'], marker='o', label=f'Iteraciones={iteration}')
ax1.set_xlabel('Tamaño de la rejilla (N)')
ax1.set_ylabel('Tiempo de ejecución (segundos)')
ax1.set_title('Tiempo de ejecución vs Tamaño de la rejilla')
ax1.legend()
ax1.set_xscale('log')  # Escala logarítmica para N
ax1.set_yscale('log')  # Escala logarítmica para el tiempo
ax1.grid(True)

# Gráfico 2: Tiempo de ejecución vs Iteraciones para diferentes N
for N in data['N'].unique():
    subset = data[data['N'] == N]
    ax2.plot(subset['iterations'], subset['execution_time'], marker='o', label=f'N={N}')
ax2.set_xlabel('Número de iteraciones')
ax2.set_ylabel('Tiempo de ejecución (segundos)')
ax2.set_title('Tiempo de ejecución vs Número de iteraciones')
ax2.legend()
ax2.set_yscale('log')  # Escala logarítmica para el tiempo
ax2.grid(True)

plt.tight_layout()
plt.savefig(os.path.join(output_dir, 'heat_results_combined.png'))
plt.close()