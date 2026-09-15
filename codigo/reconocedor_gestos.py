import cv2
import mediapipe as mp
import time
import serial

# ==========================================
# CONEXION CON EL ESP32
# ==========================================

puerto = "COM4"
velocidad = 115200

try:
    esp32 = serial.Serial(puerto, velocidad, timeout=1)
    time.sleep(2)
    print("ESP32 conectado correctamente en", puerto)

except:
    print("No se pudo conectar con el ESP32")
    print("Revisa que este conectado y que el puerto sea COM4")
    exit()


# ==========================================
# CONFIGURACION DE MEDIAPIPE
# ==========================================

BaseOptions = mp.tasks.BaseOptions
GestureRecognizer = mp.tasks.vision.GestureRecognizer
GestureRecognizerOptions = mp.tasks.vision.GestureRecognizerOptions
VisionRunningMode = mp.tasks.vision.RunningMode

modelo = "models/gesture_recognizer.task"

opciones = GestureRecognizerOptions(
    base_options=BaseOptions(model_asset_path=modelo),
    running_mode=VisionRunningMode.VIDEO,
    num_hands=1
)


# ==========================================
# RECONOCIMIENTO DE GESTOS
# ==========================================

with GestureRecognizer.create_from_options(opciones) as reconocedor:

    camara = cv2.VideoCapture(0, cv2.CAP_DSHOW)

    camara.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    camara.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    if not camara.isOpened():
        print("No se pudo abrir la camara")
        esp32.close()
        exit()

    print("======================================")
    print("CONTROL DE ILUMINACION")
    print("======================================")
    print("ESP32 conectado")
    print("MediaPipe iniciado")
    print()
    print("Haz un gesto frente a la camara")
    print("Presiona Q para salir")
    print()

    tiempo_inicio = time.time()

    # Guarda el ultimo gesto enviado
    ultimo_gesto = ""

    while True:

        # ==========================================
        # CAPTURAR IMAGEN
        # ==========================================

        ret, frame = camara.read()

        if not ret:
            print("No se pudo obtener la imagen")
            continue

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        imagen_mp = mp.Image(
            image_format=mp.ImageFormat.SRGB,
            data=frame_rgb
        )

        tiempo_actual = int((time.time() - tiempo_inicio) * 1000)

        # ==========================================
        # RECONOCER GESTO
        # ==========================================

        resultado = reconocedor.recognize_for_video(
            imagen_mp,
            tiempo_actual
        )

        if resultado.gestures:

            gesto = resultado.gestures[0][0]

            nombre = gesto.category_name
            confianza = gesto.score

            texto = f"{nombre} - {confianza:.2f}"

            cv2.putText(
                frame,
                texto,
                (20, 50),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (0, 255, 0),
                2
            )

            # ==========================================
            # ENVIAR GESTO AL ESP32
            # ==========================================

            if confianza >= 0.60 and nombre != ultimo_gesto:

                comando = nombre + "\n"

                esp32.write(comando.encode())

                print("Enviado al ESP32:", nombre)

                ultimo_gesto = nombre

        else:

            cv2.putText(
                frame,
                "No se detecta gesto",
                (20, 50),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (0, 0, 255),
                2
            )

            # Permite volver a detectar el mismo gesto
            ultimo_gesto = ""

        # ==========================================
        # MOSTRAR CAMARA
        # ==========================================

        cv2.imshow(
            "MediaPipe - Control de iluminacion",
            frame
        )

        # Salir con Q
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break


# ==========================================
# CERRAR TODO
# ==========================================

camara.release()
cv2.destroyAllWindows()
esp32.close()

print("Programa finalizado")

