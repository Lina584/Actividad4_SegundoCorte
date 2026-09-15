# Control de iluminación con MediaPipe y ESP32

## Descripción del proyecto

En este proyecto desarrollé un sistema de control de iluminación utilizando reconocimiento de gestos con la mano. Para realizarlo utilicé una cámara, MediaPipe, Python y un ESP32.

La idea principal es controlar tres LEDs realizando diferentes gestos frente a la cámara. MediaPipe se encarga de reconocer el gesto realizado, Python recibe esta información y la envía al ESP32 mediante comunicación serial. Finalmente, el ESP32 interpreta el gesto y controla los LEDs de acuerdo con la acción correspondiente.

Este proyecto fue realizado como parte de la materia de Micros, con el objetivo de integrar el reconocimiento de gestos, la programación en Python, la comunicación serial y el control de salidas mediante un microcontrolador.

---

## Objetivo

Desarrollar un sistema de iluminación controlado mediante gestos de la mano, utilizando MediaPipe para reconocer los gestos, Python para procesar la información y comunicarse con el ESP32, y el ESP32 para controlar físicamente los LEDs.

---

##  Funcionamiento general

El sistema funciona mediante una comunicación entre la cámara, el programa de Python y el ESP32.

El proceso que seguí para realizar el proyecto fue:

1. La cámara del computador captura la imagen de la mano.
2. OpenCV recibe los frames de la cámara.
3. MediaPipe analiza la imagen y reconoce el gesto realizado.
4. Python obtiene el nombre del gesto y su nivel de confianza.
5. Python envía el nombre del gesto al ESP32 mediante comunicación serial.
6. El ESP32 recibe el comando.
7. El ESP32 interpreta el gesto recibido.
8. Dependiendo del gesto, se enciende un LED con una determinada intensidad o se activa un modo de iluminación.

El funcionamiento general se puede representar de la siguiente manera:

```text
               Gesto de la mano
                      ↓
                   Cámara
                      ↓
                   OpenCV
                      ↓
                  MediaPipe
                      ↓
                   Python
                      ↓
             Comunicación Serial
                      ↓
                    COM4
                      ↓
                   ESP32
                      ↓
               Control de LEDs
                      ↓
                Iluminación
```

---

# Gestos utilizados

Para el proyecto utilicé cinco gestos reconocidos por el modelo Gesture Recognizer de MediaPipe.

| Gesto | Nombre en MediaPipe | Acción |
|---|---|---|
| ✊ Puño cerrado | `Closed_Fist` | LED amarillo al 30 % |
| ✌️ Dos dedos / Victoria | `Victory` | LED azul al 70 % |
| 🖐️ Mano abierta | `Open_Palm` | LED rojo al 100 % |
| 👎 Pulgar abajo | `Thumb_Down` | Activa el Modo 1 |
| 👍 Pulgar arriba | `Thumb_Up` | Activa el Modo 2 |

---

# Control de iluminación

Se utilizaron tres LEDs para representar diferentes niveles de iluminación.

| LED | GPIO del ESP32 | Intensidad |
|---|---:|---:|
| 🟡 Amarillo | GPIO 25 | 30 % |
| 🔵 Azul | GPIO 26 | 70 % |
| 🔴 Rojo | GPIO 27 | 100 % |

Para controlar la intensidad de los LEDs se utilizó PWM.

Los valores utilizados fueron:

```text
30 %  → 76 / 255
70 %  → 179 / 255
100 % → 255 / 255
```

De esta manera, el ESP32 puede controlar la intensidad de cada LED dependiendo del gesto reconocido.

---

# Relación entre PWM e intensidad

El PWM permite controlar cuánto tiempo permanece activa una señal durante cada ciclo.

Para este proyecto se utilizó una escala de 0 a 255.

La relación aproximada utilizada fue:

```text
30 %:

255 × 0.30 = 76.5 ≈ 76


70 %:

255 × 0.70 = 178.5 ≈ 179


100 %:

255 × 1.00 = 255
```

Por esta razón se utilizaron los siguientes valores en el programa:

```cpp
analogWrite(LED_AMARILLO, 76);
analogWrite(LED_AZUL, 179);
analogWrite(LED_ROJO, 255);
```

---

# Modos de iluminación

Además del control individual de los LEDs, implementé dos modos de iluminación utilizando los gestos de pulgar.

##  Modo 1 - `Thumb_Down`

Cuando MediaPipe reconoce:

```text
Thumb_Down
```

Python envía este comando al ESP32.

El ESP32 interpreta esta señal y activa el **Modo 1**.

La secuencia implementada es:

```text
  Amarillo
      ↓
    Azul
      ↓
    Rojo
      ↓
  Amarillo
      ↓
     ...
```

Cada estado permanece aproximadamente 500 ms antes de cambiar al siguiente.

El modo continúa ejecutándose hasta que se recibe otro gesto que cambie el estado del sistema.

---

## Modo 2 - `Thumb_Up`

Cuando MediaPipe reconoce:

```text
Thumb_Up
```

Python envía este comando al ESP32.

El ESP32 activa el **Modo 2**.

La secuencia implementada es:

```text
    Amarillo +  Rojo
            ↓
           Azul
            ↓
    Amarillo + Rojo
            ↓
           Azul
            ↓
           ...
```

Cada cambio de estado ocurre aproximadamente cada 500 ms.

---

# Conexión del circuito

Para realizar el montaje físico utilicé un ESP32, tres LEDs, una protoboard y resistencias para limitar la corriente.

Las conexiones utilizadas fueron:

```text
ESP32 GPIO 25 → Resistencia → LED amarillo → GND

ESP32 GPIO 26 → Resistencia → LED azul → GND

ESP32 GPIO 27 → Resistencia → LED rojo → GND
```

Las resistencias utilizadas fueron aproximadamente de:

```text
220 Ω - 330 Ω
```

Las resistencias se conectan en serie con los LEDs para limitar la corriente y evitar dañarlos.

---

# Materiales utilizados

Los principales materiales utilizados para realizar el proyecto fueron:

- ESP32
- Protoboard
- LED amarillo
- LED azul
- LED rojo
- Resistencias de 220 Ω o 330 Ω
- Cable USB para conectar el ESP32
- Cables jumper
- Computador con cámara

---

# 💻 Software utilizado

Para desarrollar el proyecto utilicé:

- Python
- MediaPipe
- OpenCV
- PySerial
- Arduino IDE
- Visual Studio Code

---

#  MediaPipe

MediaPipe fue utilizado para realizar el reconocimiento de los gestos de la mano mediante la cámara.

El proyecto utiliza el **Gesture Recognizer**, que permite identificar diferentes gestos y obtener información sobre la confianza del reconocimiento.

Los cinco gestos utilizados fueron:

```text
Closed_Fist
Victory
Open_Palm
Thumb_Down
Thumb_Up
```

El programa de Python analiza continuamente las imágenes obtenidas de la cámara y obtiene el gesto reconocido.

Además, se utiliza un valor de confianza para evitar enviar al ESP32 resultados cuando el reconocimiento no es suficientemente confiable.

En este proyecto se estableció como referencia una confianza mínima de:

```text
0.60
```

---


#  Comunicación serial

La comunicación entre Python y el ESP32 se realizó mediante el puerto USB del computador.

En este proyecto el ESP32 fue reconocido como:

```text
COM4
```

La velocidad utilizada fue:

```text
115200
```

Python envía el nombre del gesto como una cadena de texto.

Por ejemplo:

```text
Closed_Fist
```

```text
Victory
```

```text
Open_Palm
```

```text
Thumb_Down
```

```text
Thumb_Up
```

Cada comando se envía acompañado de un salto de línea:

```text
Closed_Fist\n
```

Esto permite que el ESP32 pueda identificar el final de cada comando utilizando la lectura serial.

---


# 🔧 Programa del ESP32

El archivo:

```text
codigo/control_iluminacion_ESP32.ino
```

contiene el programa que controla físicamente los LEDs.

El ESP32 recibe los comandos enviados desde Python y decide qué acción ejecutar.

El programa contiene funciones para:

- Apagar los LEDs.
- Controlar el LED amarillo.
- Controlar el LED azul.
- Controlar el LED rojo.
- Ejecutar el Modo 1.
- Ejecutar el Modo 2.
- Recibir comandos mediante comunicación serial.

---

# ⚙️ Funcionamiento del ESP32

Cuando el ESP32 recibe un comando normal, realiza directamente la acción correspondiente.

Por ejemplo:

```text
Closed_Fist → LED amarillo al 30 %
```

```text
Victory → LED azul al 70 %
```

```text
Open_Palm → LED rojo al 100 %
```

Cuando recibe:

```text
Thumb_Down
```

activa el Modo 1.

Cuando recibe:

```text
Thumb_Up
```

activa el Modo 2.

Los gestos `Thumb_Down` y `Thumb_Up` se utilizan como señales de evento para activar las rutinas correspondientes a los dos modos de iluminación.

---

# Resultados

El resultado final fue un sistema capaz de controlar físicamente un conjunto de LEDs utilizando gestos realizados frente a una cámara.

Los tres gestos principales permiten controlar diferentes niveles de iluminación:

```text
 Closed_Fist
→ LED amarillo
→ 30 %
```

```text
 Victory
→ LED azul
→ 70 %
```

```text
 Open_Palm
→ LED rojo
→ 100 %
```

Además:

```text
 Thumb_Down
→ Modo 1
→ Amarillo → Azul → Rojo
```

y:

```text
 Thumb_Up
→ Modo 2
→ Amarillo + Rojo → Azul
```

La comunicación entre Python y el ESP32 funcionó correctamente mediante el puerto serial.

---



# Conclusión

El proyecto cumplió con el objetivo de desarrollar un sistema de control de iluminación mediante gestos de la mano.

La utilización de MediaPipe permitió reconocer los gestos en tiempo real mediante la cámara. Python funcionó como intermediario para procesar la información y enviarla al ESP32 mediante comunicación serial.

Por su parte, el ESP32 se encargó de interpretar los comandos y controlar físicamente los LEDs utilizando diferentes niveles de PWM y secuencias de iluminación.

Uno de los aspectos más importantes del proyecto fue la integración de todas las partes, ya que fue necesario comprobar primero la cámara, después el reconocimiento de gestos, posteriormente los LEDs y finalmente la comunicación entre Python y el ESP32.

El resultado final fue un sistema en el que se puede controlar la iluminación simplemente realizando diferentes gestos frente a la cámara, sin necesidad de utilizar botones físicos.

---
## Evidencia del montaje

En la siguiente imagen se muestra el montaje físico realizado con el ESP32 y los tres LEDs:

![Montaje del circuito](ImagenCircuito.jpeg)

#  Autora

**Lina Maria Moreno Ospina**

Ingeniería Mecatrónica, Universidad Militar Nueva Granada
