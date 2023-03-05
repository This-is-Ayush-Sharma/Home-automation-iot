#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OPPO Reno2 Z";
const char* password = "np8ziu78";
const char* serverUrl = "http://52.158.43.115:5000/api/v1/getState";
const int relayPin = 23;

HTTPClient http;
void setup() {
  Serial.begin(115200);  
  pinMode(relayPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Serial.println("Connecting to WiFi...");
  }
  // Serial.println("Connected to WiFi");
}
void loop() {

  // Wait for wifi to be connected
  uint32_t notConnectedCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      // Serial.println("Wifi connecting...");
      notConnectedCounter++;
      if(notConnectedCounter > 150) { // Reset board if not connected after 5s
          // Serial.println("Resetting due to Wifi not connecting...");
          ESP.restart();
      }
  }

  delay(500);
  // Serial.println(serverUrl);
  http.begin(serverUrl);

  // Make GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String response = http.getString();
    if(response == "0")
    {
      // Serial.println("turning off");
      digitalWrite(relayPin, 1);
    }
    else
    {
      // Serial.println("turning on");
      digitalWrite(relayPin, 0);
    }
    // Serial.println(response);
  } else {
    // Serial.printf("HTTP error: %d\n", httpResponseCode);
  }
  http.end();
}