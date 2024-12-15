import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo CSV
data = pd.read_csv("matrix_times.csv")

# Graficar los resultados
plt.figure(figsize=(10, 6))
plt.plot(data["N"], data["Time"], marker="o", linestyle="-", color="b")
plt.title("Tiempo de ejecución vs Tamaño de matriz")
plt.xlabel("Tamaño de matriz (N)")
plt.ylabel("Tiempo de ejecución (segundos)")
plt.grid(True)
plt.show()