import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo CSV
data = pd.read_csv('results.csv')

# Graficar la estimación de PI
plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.plot(data['num_points'], data['pi_estimate'], marker='o')
plt.xlabel('Número de puntos')
plt.ylabel('Estimación de PI')
plt.title('Estimación de PI vs Número de puntos')

# Graficar el tiempo de ejecución
plt.subplot(1, 2, 2)
plt.plot(data['num_points'], data['execution_time'], marker='o')
plt.xlabel('Número de puntos')
plt.ylabel('Tiempo de ejecución (segundos)')
plt.title('Tiempo de ejecución vs Número de puntos')

plt.tight_layout()
plt.show()