#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

const char ssid[] = "Inudi Harek, Horakhet";
const char password[] = "987654321";

const char endpointTemplate[] = "http://%s:5173/%s";

// -4: the length of the two "%s"
// +12: the maximum length of an IP address
// +1: the null terminator
const int padding = sizeof(endpointTemplate) - 4 + 12 + 1;

char* logEndpoint = new char[padding + sizeof("log")];
char* enrollEndpoint = new char[padding + sizeof("enroll")];
char* verifyEndpoint = new char[padding + sizeof("verify")];

WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Waiting...");

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  sprintf(logEndpoint, endpointTemplate, WiFi.gatewayIP().toString().c_str(), "log");
  sprintf(enrollEndpoint, endpointTemplate, WiFi.gatewayIP().toString().c_str(), "enroll");
  sprintf(verifyEndpoint, endpointTemplate, WiFi.gatewayIP().toString().c_str(), "verify");

  digitalWrite(LED_BUILTIN, HIGH);
}

unsigned long lastLoop = 0;

void loop() {
  // Poll the Arduino for requests
  if (Serial.available()) {
    const String str = Serial.readStringUntil('\n');

    if (str.equalsIgnoreCase("req:new_fingerprint\r")) {
      http.begin(client, enrollEndpoint);
      http.PUT("");
      http.end();
    }
  }

  // Poll the webserver for requests
  if (millis() - lastLoop >= 500) {
    lastLoop = millis();

    http.begin(client, enrollEndpoint);
    if (http.GET() == HTTP_CODE_OK) {
      Serial.println("*");
    };
    http.end();

    http.begin(client, verifyEndpoint);
    if (http.GET() == HTTP_CODE_OK) {
      Serial.println("#");
    }
    http.end();
  }
}
