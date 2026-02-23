#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

char hexaKeys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' },
};

byte rowPins[4] = { 13, 12, 11, 10 };
byte colPins[4] = { 9, 8, 7, 6 };

Keypad keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4);
SoftwareSerial fingerprintSerial(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_Fingerprint fingerSensor(&fingerprintSerial);

#define BUZZER_PIN 4
#define POT_DATA_PIN A0
#define LED_PWM_PIN 5

void setup() {
  Serial.begin(9600);

  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  fingerSensor.begin(57600);
}

// 0: waiting for keypad input
// 1: enrolling fingerprint
// 2: verifying fingerprint
// 3: sound buzzer and blink LED for a few seconds
uint8_t currentProcess = 0;
uint8_t lastProcess = currentProcess;

uint8_t enrollStep = 0;
uint8_t lastEnrollStep = enrollStep;

uint8_t verifyStep = 0;
uint8_t lastVerifyStep = verifyStep;

unsigned long alarmTime = 0;

void reset() {
  lcd.clear();
  noTone(BUZZER_PIN);
  analogWrite(LED_PWM_PIN, 0);

  currentProcess = 0;
  lastProcess = currentProcess;

  enrollStep = 0;
  lastEnrollStep = enrollStep;

  verifyStep = 0;
  lastVerifyStep = verifyStep;

  alarmTime = 0;
}

void loop() {
  if (lastProcess != currentProcess) {
    lcd.clear();
    lastProcess = currentProcess;
  }

  if (currentProcess == 0) {
    lcd.setCursor(0, 0);
    lcd.print("*: Enroll");
    lcd.setCursor(0, 1);
    lcd.print("#: Verify");

    char key = keypad.getKey();
    if (key == '*') {
      currentProcess = 1;
    } else if (key == '#') {
      currentProcess = 2;
    }

    if (Serial.available() > 0) {
      String str = Serial.readStringUntil('\n');
      Serial.println(str);

      if (str.equalsIgnoreCase("*\r")) {
        currentProcess = 1;
      } else if (str.equalsIgnoreCase("#\r")) {
        currentProcess = 2;
      }
    }

    return;
  } else if (currentProcess == 1) {
    return stepFingerprintEnrollProcess();
  } else if (currentProcess == 2) {
    return stepFingerprintVerifyProcess();
  } else if (currentProcess == 3) {
    if (alarmTime == 0) {
      alarmTime = millis();
      tone(BUZZER_PIN, 200);
    }

    int ledBrightness = analogRead(POT_DATA_PIN) / 4;
    analogWrite(LED_PWM_PIN, ledBrightness);

    if (millis() - alarmTime >= 200) {
      reset();
    }
  }
}

void stepFingerprintEnrollProcess() {
  if (lastEnrollStep != enrollStep) {
    lcd.clear();
    lastEnrollStep = enrollStep;
  }

  lcd.setCursor(0, 0);

  if (enrollStep == 0) {
    lcd.print("Touch sensor");

    uint8_t status = fingerSensor.getImage();
    if (status != FINGERPRINT_OK) {
      if (status != FINGERPRINT_NOFINGER) {
        abortFingerprintProcess(status);
      }
    } else {
      enrollStep = 1;
    }

    return;
  }

  if (enrollStep == 1) {
    lcd.print("Converting image");

    uint8_t status = fingerSensor.image2Tz(1);
    if (status != FINGERPRINT_OK) {
      abortFingerprintProcess(status);
    } else {
      enrollStep = 2;
    }

    return;
  }

  if (enrollStep == 2) {
    lcd.print("Touch sensor");
    lcd.setCursor(0, 1);
    lcd.print("again");

    uint8_t status = fingerSensor.getImage();
    if (status != FINGERPRINT_OK) {
      if (status != FINGERPRINT_NOFINGER) {
        abortFingerprintProcess(status);
      }
    } else {
      enrollStep = 3;
    }

    return;
  }

  if (enrollStep == 3) {
    lcd.print("Converting image");

    uint8_t status = fingerSensor.image2Tz(2);
    if (status != FINGERPRINT_OK) {
      abortFingerprintProcess(status);
    } else {
      enrollStep = 4;
    }

    return;
  }

  if (enrollStep == 4) {
    lcd.print("Creating model");

    uint8_t status = fingerSensor.createModel();
    if (status != FINGERPRINT_OK) {
      abortFingerprintProcess(status);
    } else {
      enrollStep = 5;
    }

    return;
  }

  if (enrollStep == 5) {
    lcd.print("Saving model");

    // TODO: don't hardcode as ID 1
    uint8_t status = fingerSensor.storeModel(1);
    if (status != FINGERPRINT_OK) {
      abortFingerprintProcess(status);
      return;
    }

    Serial.println("req:new_fingerprint");
    Serial.flush();

    reset();
    lcd.print("Done!");

    delay(3000);
  }
}

void stepFingerprintVerifyProcess() {
  if (lastVerifyStep != verifyStep) {
    lcd.clear();
    lastVerifyStep = verifyStep;
  }

  lcd.setCursor(0, 0);

  if (verifyStep == 0) {
    lcd.print("Touch sensor");

    uint8_t status = fingerSensor.getImage();
    if (status != FINGERPRINT_OK) {
      if (status != FINGERPRINT_NOFINGER) {
        abortFingerprintProcess(status);
      }
    } else {
      verifyStep = 1;
    }

    return;
  }

  if (verifyStep == 1) {
    lcd.print("Converting image");

    uint8_t status = fingerSensor.image2Tz(1);
    if (status != FINGERPRINT_OK) {
      abortFingerprintProcess(status);
    } else {
      verifyStep = 2;
    }

    return;
  }

  if (verifyStep == 2) {
    lcd.print("Searching");
    lcd.setCursor(0, 1);
    lcd.print("fingerprint");

    uint8_t status = fingerSensor.fingerSearch();
    if (status != FINGERPRINT_OK) {
      if (status == FINGERPRINT_NOTFOUND) {
        // sound buzzer and blink LED here
        currentProcess = 3;
      } else {
        abortFingerprintProcess(status);
      }

      return;
    }

    reset();
    lcd.print("Matched!");
    delay(3000);

    return;
  }
}

// Abort fingerprint enroll/verify sequences: reset data, display error message
// to LCD, then return. If given FINGERPRINT_OK, does nothing.
void abortFingerprintProcess(uint8_t errorCode) {
  if (errorCode == FINGERPRINT_OK) return;

  reset();

  switch (errorCode) {
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication");
      lcd.setCursor(0, 1);
      lcd.print("error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.print("Imaging error");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.print("Image too messy");
      break;
    case FINGERPRINT_ENROLLMISMATCH:
      lcd.print("Mismatched");
      lcd.setCursor(0, 1);
      lcd.print("images");
      break;
    case FINGERPRINT_FLASHERR:
      lcd.print("Failed to write");
      lcd.setCursor(0, 1);
      lcd.print("model");
      break;
    case FINGERPRINT_NOTFOUND:
      lcd.print("No matching");
      lcd.setCursor(0, 1);
      lcd.print("fingerprint");
      break;
    default:
      lcd.print("Unknown error");
      break;
  }

  delay(3000);
}
