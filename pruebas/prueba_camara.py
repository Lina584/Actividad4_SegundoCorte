import cv2

# Abrir cámara con DirectShow
camara = cv2.VideoCapture(0, cv2.CAP_DSHOW)

# Configurar resolución
camara.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
camara.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

if not camara.isOpened():
    print("No se pudo abrir la cámara")
    exit()

print("Cámara iniciada correctamente")
print("Presiona Q para salir")

while True:

    ret, frame = camara.read()

    if not ret:
        print("No se pudo obtener la imagen")
        continue

    # Mostrar la imagen
    cv2.imshow("Prueba de camara", frame)

    # Salir con Q
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

camara.release()
cv2.destroyAllWindows() 
