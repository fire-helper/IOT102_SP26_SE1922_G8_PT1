#include <ESP8266WiFi.h>

#define SSID "Inudi Harek, Horakhet"
#define PASSWORD "987654321"

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
}
