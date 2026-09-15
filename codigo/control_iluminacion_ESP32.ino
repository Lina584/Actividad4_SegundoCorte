/*
  ACTIVIDAD 4 DE MICROS
  Control de iluminacion con MediaPipe + ESP32
*/

const int LED_AMARILLO = 25;
const int LED_AZUL = 26;
const int LED_ROJO = 27;

// Variables para controlar los modos
int modo = 0;
int paso = 0;

unsigned long tiempoAnterior = 0;

const unsigned long intervalo = 500;


// --------------------------------------------------
// Apagar todos los LEDs
// --------------------------------------------------

void apagarLEDs() {

  analogWrite(LED_AMARILLO, 0);
  analogWrite(LED_AZUL, 0);
  analogWrite(LED_ROJO, 0);
}


// --------------------------------------------------
// Control de iluminacion normal
// --------------------------------------------------

void controlarLuz(String gesto) {

  // Al recibir un gesto normal salimos de cualquier modo
  modo = 0;

  apagarLEDs();

  if (gesto == "Closed_Fist") {

    analogWrite(LED_AMARILLO, 76);

    Serial.println("Gesto: Closed_Fist");
    Serial.println("LED amarillo - 30 %");

  }

  else if (gesto == "Victory") {

    analogWrite(LED_AZUL, 179);

    Serial.println("Gesto: Victory");
    Serial.println("LED azul - 70 %");

  }

  else if (gesto == "Open_Palm") {

    analogWrite(LED_ROJO, 255);

    Serial.println("Gesto: Open_Palm");
    Serial.println("LED rojo - 100 %");

  }

  else {

    Serial.println("Gesto no reconocido");

  }
}


// --------------------------------------------------
// MODO 1
// Thumb_Down
// --------------------------------------------------

void ejecutarModo1() {

  if (millis() - tiempoAnterior >= intervalo) {

    tiempoAnterior = millis();

    apagarLEDs();

    if (paso == 0) {

      analogWrite(LED_AMARILLO, 76);

      Serial.println("MODO 1 -> LED amarillo");

    }

    else if (paso == 1) {

      analogWrite(LED_AZUL, 179);

      Serial.println("MODO 1 -> LED azul");

    }

    else if (paso == 2) {

      analogWrite(LED_ROJO, 255);

      Serial.println("MODO 1 -> LED rojo");

    }

    paso++;

    if (paso > 2) {
      paso = 0;
    }
  }
}


// --------------------------------------------------
// MODO 2
// Thumb_Up
// --------------------------------------------------

void ejecutarModo2() {

  if (millis() - tiempoAnterior >= intervalo) {

    tiempoAnterior = millis();

    apagarLEDs();

    if (paso == 0) {

      analogWrite(LED_AMARILLO, 76);
      analogWrite(LED_ROJO, 255);

      Serial.println("MODO 2 -> Amarillo + Rojo");

    }

    else if (paso == 1) {

      analogWrite(LED_AZUL, 179);

      Serial.println("MODO 2 -> Azul");

    }

    paso++;

    if (paso > 1) {
      paso = 0;
    }
  }
}


// --------------------------------------------------
// Configuracion
// --------------------------------------------------

void setup() {

  Serial.begin(115200);

  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  apagarLEDs();

  Serial.println("--------------------------------");
  Serial.println("SISTEMA DE ILUMINACION");
  Serial.println("--------------------------------");

  Serial.println("Closed_Fist -> 30 %");
  Serial.println("Victory -> 70 %");
  Serial.println("Open_Palm -> 100 %");

  Serial.println("Thumb_Down -> MODO 1");
  Serial.println("Thumb_Up -> MODO 2");
}


// --------------------------------------------------
// Programa principal
// --------------------------------------------------

void loop() {

  // Revisar si llego un comando
  if (Serial.available() > 0) {

    String gesto = Serial.readStringUntil('\n');

    gesto.trim();


    // ----------------------------------------------
    // Interrupcion 1
    // ----------------------------------------------

    if (gesto == "Thumb_Down") {

      modo = 1;
      paso = 0;
      tiempoAnterior = millis();

      Serial.println(">>> INTERRUPCION 1 <<<");
      Serial.println(">>> MODO 1 ACTIVADO <<<");

    }


    // ----------------------------------------------
    // Interrupcion 2
    // ----------------------------------------------

    else if (gesto == "Thumb_Up") {

      modo = 2;
      paso = 0;
      tiempoAnterior = millis();

      Serial.println(">>> INTERRUPCION 2 <<<");
      Serial.println(">>> MODO 2 ACTIVADO <<<");

    }


    // ----------------------------------------------
    // Gestos de iluminacion
    // ----------------------------------------------

    else {

      controlarLuz(gesto);

    }
  }


  // Ejecutar el modo correspondiente

  if (modo == 1) {

    ejecutarModo1();

  }

  else if (modo == 2) {

    ejecutarModo2();

  }
}
