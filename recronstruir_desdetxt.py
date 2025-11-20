import cv2
import numpy as np
import re

# === 1. Leer el archivo ===
archivo="imagen_rgb.txt"
#archivo="imagen_rgb_expandida.txt"
#archivo = "imagen_rgb_suavizada.txt"
#archivo = "imagen_rgb_suavizada_vecinos.txt"

with open(archivo, "r") as f:
    lineas = f.readlines()

# === 2. Procesar cada línea ===
pixeles = []
for linea in lineas:
    # Extrae todos los tripletes (R,G,B) con expresiones regulares
    tripletas = re.findall(r'\((\d+),(\d+),(\d+)\)', linea)
    fila = [(int(r), int(g), int(b)) for r, g, b in tripletas]
    pixeles.append(fila)

# === 3. Convertir a arreglo NumPy ===
img = np.array(pixeles, dtype=np.uint8)

# 🔁 Convertir RGB → BGR (porque imshow espera BGR)
img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

# === 4. Mostrar información ===
print(f"✅ Imagen reconstruida con tamaño: {img.shape[1]}x{img.shape[0]}, canales: {img.shape[2]}")

# === 5. Mostrar la imagen ===
cv2.imshow("Imagen reconstruida", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
