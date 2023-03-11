#include <ArduinoWebsockets.h>
#include <WiFi.h>
using namespace websockets;

const char* ssid = "PROJECT";
const char* password = "project#86";
const char* websockets_server = "http://5x.x5x.xx.115:4000"; //server adress and port
const int relayPin = 23;
const int light_pin = 2;
uint32_t notConnectedCounter = 0;


IPAddress staticIP(172, 32, 0, 8);
IPAddress gateway(172, 32, 0, 1);    // Replace this with your gateway IP Addess
IPAddress subnet(255, 255, 255, 0);  // Replace this with your Subnet Mask
IPAddress dns(172, 32, 0, 1);        // Replace this with your DNS

void onMessageCallback(WebsocketsMessage message) {
    // Serial.print("Got Message: ");
    Serial.println(message.data());
    if(message.data() == "1")
    {
      digitalWrite(relayPin, 1);
    }
    else
    {
      digitalWrite(relayPin, 0);
    }
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    pinMode(relayPin, OUTPUT);
    pinMode(light_pin, OUTPUT);
    if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
    Serial.println("Configuration failed.");
    }
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    // client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(websockets_server);

    // Send a message
    client.send("Hi Server!");
    // Send a ping
    client.ping();
    digitalWrite(light_pin, HIGH);
}

void loop() {
    client.poll();
    notConnectedCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(light_pin, LOW);
    // Serial.println("Wifi connecting...");
    notConnectedCounter++;
    if (notConnectedCounter > 150) {  // Reset board if not connected after 5s
      // Serial.println("Resetting due to Wifi not connecting...");
      ESP.restart();
    }
  }
}
