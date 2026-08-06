/*
 * Six-Wavelength LED Absorbance Colorimeter
 * ------------------------------------------
 * MCU        : ATmega328P (Arduino Uno core), 16 MHz, 5 V
 * Detector   : GL5516 CdS photoresistor (LDR) in a voltage divider on ADC2 (pin A2)
 * Emitters   : 6 LEDs on digital pins 2-7, each via a 1k series resistor
 * ADC        : 10-bit, default 5 V reference (0-1023 counts)
 * Serial     : 9600 baud
 *
 * Wavelengths / pin map:
 *   Pin 2 -> 665 nm (Red)     Pin 5 -> 550 nm (Green)
 *   Pin 3 -> 630 nm (Orange)  Pin 6 -> 470 nm (Blue)
 *   Pin 4 -> 600 nm (Yellow)  Pin 7 -> 400 nm (Violet)
 *
 * Commands (newline-terminated):
 *   CALIBRATE  - lights each LED against the blank cuvette, stores I0 per channel
 *   READ_<nm>  - measures one wavelength, prints "<nm>,<absorbance>"
 *   SWEEP      - measures all six in ROYGBIV order, prints one CSV line
 *
 * Absorbance is A = -log10(I / I0), where I is the current LDR reading and
 * I0 is the calibration (blank) reading for that channel.
 */

#include <Wire.h>
#include <math.h>

// Calibration storage
int d2CalibrationValue = 0;
int d3CalibrationValue = 0;
int d4CalibrationValue = 0;
int d5CalibrationValue = 0;
int d6CalibrationValue = 0;
int d7CalibrationValue = 0;

// wavelength + pins
struct WavelengthInfo {
  int pin;
  int wavelength;
};

WavelengthInfo wlMap[] = {
  {2, 665},
  {3, 630},
  {4, 600},
  {5, 550},   // 550 nm on pin 5
  {6, 470},
  {7, 400}    // 400 nm on pin 7
};

int getCalibrationForPin(int pin) {
  switch (pin) {
    case 2: return d2CalibrationValue;
    case 3: return d3CalibrationValue;
    case 4: return d4CalibrationValue;
    case 5: return d5CalibrationValue;
    case 6: return d6CalibrationValue;
    case 7: return d7CalibrationValue;
  }
  return 1;
}

// Perform full calibration with live status updates
void runCalibration() {

  Serial.println("CAL_START");

  for (int i = 0; i < 6; i++) {

    int pin = wlMap[i].pin;
    int wl  = wlMap[i].wavelength;

    Serial.print("CAL_STEP ");
    Serial.println(wl);

    digitalWrite(pin, HIGH);
    delay(2500);

    int sensorValue = analogRead(A2);

    switch (pin) {
      case 2: d2CalibrationValue = sensorValue; break;
      case 3: d3CalibrationValue = sensorValue; break;
      case 4: d4CalibrationValue = sensorValue; break;
      case 5: d5CalibrationValue = sensorValue; break; // now 550 nm
      case 6: d6CalibrationValue = sensorValue; break;
      case 7: d7CalibrationValue = sensorValue; break; // now 400 nm
    }

    digitalWrite(pin, LOW);
  }

  Serial.println("CAL_DONE");
}

// Read one wavelength only
void readSingle(int pin, int wavelength) {
  digitalWrite(pin, HIGH);
  delay(2500);

  int sensorValue = analogRead(A2);
  int cal = getCalibrationForPin(pin);

  float absorbance = -log10((float)sensorValue / (float)cal);

  Serial.print(wavelength);
  Serial.print(",");
  Serial.println(absorbance, 4);

  digitalWrite(pin, LOW);

  Serial.println("DONE");
}

// Full sweep in ROYGBIV order -> single CSV line of absorbance values
void runSweep() {
  String csvData = "";
  for (int i = 0; i < 6; i++) {
    int pin = wlMap[i].pin;

    digitalWrite(pin, HIGH);
    delay(2500);

    int sensorValue = analogRead(A2);
    int cal = getCalibrationForPin(pin);
    float absorbance = -log10((float)sensorValue / (float)cal);

    digitalWrite(pin, LOW);

    csvData += String(absorbance, 4);
    if (i < 5) csvData += ",";
  }
  Serial.println(csvData);
}

void setup() {
  Serial.begin(9600);

  for (int pin = 2; pin <= 7; pin++) {
    pinMode(pin, OUTPUT);
  }

  delay(500);
}

String incoming = "";

void loop() {

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {

      if (incoming == "CALIBRATE") {
        runCalibration();
      }

      else if (incoming == "SWEEP") {
        runSweep();
      }

      else if (incoming.startsWith("READ_")) {
        int wl = incoming.substring(5).toInt();

        for (int i = 0; i < 6; i++) {
          if (wlMap[i].wavelength == wl) {
            readSingle(wlMap[i].pin, wl);
            break;
          }
        }
      }

      incoming = "";
    }
    else {
      incoming += c;
    }
  }
}
