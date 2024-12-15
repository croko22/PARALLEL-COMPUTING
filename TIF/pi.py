import pandas as pd
import matplotlib.pyplot as plt
import os

# Crear carpeta para guardar las imágenes
output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def plot_combined(csv_file, title, output_file):
    data = pd.read_csv(csv_file)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 6))

    # Gráfico de tiempo de ejecución
    ax1.plot(data['num_points'], data['execution_time'], marker='o', label=f'{data["threads"][0]} hilos')
    ax1.set_title(f'{title} - Tiempo de Ejecución')
    ax1.set_xlabel('Número de Puntos')
    ax1.set_ylabel('Tiempo de Ejecución (segundos)')
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.legend()
    ax1.grid(True)

    # Gráfico de precisión
    ax2.plot(data['num_points'], data['precision'], marker='o', label=f'{data["threads"][0]} hilos')
    ax2.axhline(y=0, color='r', linestyle='--', label='Valor Real de π')
    ax2.set_title(f'{title} - Precisión')
    ax2.set_xlabel('Número de Puntos')
    ax2.set_ylabel('Precisión')
    ax2.set_xscale('log')
    ax2.legend()
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, output_file))
    plt.close()

# Graficar y guardar resultados combinados para 4 hilos
plot_combined('pi_4_threads.csv', 'Estimación de Pi con 4 Hilos', 'pi_4_threads_combined.png')

# Graficar y guardar resultados combinados para 8 hilos
plot_combined('pi_8_threads.csv', 'Estimación de Pi con 8 Hilos', 'pi_8_threads_combined.png')

# Graficar y guardar resultados combinados para 16 hilos
plot_combined('pi_16_threads.csv', 'Estimación de Pi con 16 Hilos', 'pi_16_threads_combined.png')