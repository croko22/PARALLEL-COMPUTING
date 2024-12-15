import pandas as pd
import matplotlib.pyplot as plt
import os

# Crear carpeta para guardar las imágenes
output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def plot_results(csv_file, title, x_label, y_label, output_file):
    data = pd.read_csv(csv_file)
    plt.figure(figsize=(10, 6))
    plt.plot(data['N'], data['Time'], marker='o', label=f'{data["Threads"][0]} hilos')
    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, output_file))
    plt.close()

# Graficar y guardar resultados para 4 hilos
plot_results('matrix_times_4_threads.csv', 'Multiplicación de Matrices con 4 Hilos', 'Tamaño de la Matriz (N)', 'Tiempo de Ejecución (segundos)', 'matrix_times_4_threads.png')

# Graficar y guardar resultados para 8 hilos
plot_results('matrix_times_8_threads.csv', 'Multiplicación de Matrices con 8 Hilos', 'Tamaño de la Matriz (N)', 'Tiempo de Ejecución (segundos)', 'matrix_times_8_threads.png')