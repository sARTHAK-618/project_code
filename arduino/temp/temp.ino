#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* ssid = "TP-Link_8B84";
const char* password = "14195365";
const int DHTPin = 4;          // Pin to which the DHT sensor is connected
const int DHTType = DHT22;     // DHT type (DHT11, DHT21, DHT22)
const int waterLevelPin = 34;  // Pin to which the analog water level sensor is connected
const int ledPin = 2;          // Pin to which the LED is connected
const int relay1Pin = 12;      // Pin to which relay 1 (normally open) is connected
const int relay2Pin = 13;      // Pin to which relay 2 (normally closed) is connected
DHT dht(DHTPin, DHTType);
WebServer server(80);

bool relay1Status = true; // true = relay1 open, false = relay1 closed
bool relay2Status = false; // true = relay2 open, false = relay2 closed

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH); // Relay 1 initially open
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay2Pin, LOW);  // Relay 2 initially closed
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleSensorData);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  float humidity = dht.readHumidity();
  int waterLevel = analogRead(waterLevelPin);

  // When water level reaches 500 or more, turn off relay1 and turn on relay2
  if (waterLevel >= 1500) {
    digitalWrite(relay1Pin, LOW);   // Close relay 1
    digitalWrite(relay2Pin, HIGH);  // Open relay 2
    relay1Status = false;
    relay2Status = true;
  }
  // When water level is between 400 and 500, turn off both relays
  else if (waterLevel >= 1000 && waterLevel < 1500) {
    digitalWrite(relay1Pin, LOW); // Open relay 1
    digitalWrite(relay2Pin, LOW);  // Close relay 2
    relay1Status = false;
    relay2Status = false;
  }
  // When water level drops below 400, turn on relay1 and turn off relay2
  else {
    digitalWrite(relay1Pin, HIGH); // Open relay 1
    digitalWrite(relay2Pin, LOW);  // Close relay 2
    relay1Status = true;
    relay2Status = false;
  }

  // Control LED based on humidity
  if (humidity >= 80.0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int waterLevel = analogRead(waterLevelPin);
  String webpage = "<html><head><title>DHT22 Sensor Data</title>";
  webpage += "<script>function refreshData() { fetch('/data').then(response => response.text()).then(data => { document.getElementById('sensorData').innerHTML = data; }); } setInterval(refreshData, 1000); </script>";
  webpage += "</head><body><h1>DHT22 Sensor Data</h1>";
  webpage += "<div id='sensorData'>";
  webpage += "<p>Humidity: " + String(humidity) + "</p>";
  webpage += "<p>Temperature: " + String(temperature) + "</p>";
  webpage += "<p>Water Level: " + String(waterLevel) + "</p>";
  webpage += "<p>Relay 1 Status: ";
  webpage += relay1Status ? "Open" : "Closed";
  webpage += "</p>";
  webpage += "<p>Relay 2 Status: ";
  webpage += relay2Status ? "Open" : "Closed";
  webpage += "</p>";
  webpage += "</div></body></html>";
  server.send(200, "text/html", webpage);
}

void handleSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int waterLevel = analogRead(waterLevelPin);
  String sensorData = "Humidity: " + String(humidity) + "%, Temperature: " + String(temperature) + ", Water Level: " + String(waterLevel) + ", Relay 1 Status: " + (relay1Status ? "Open" : "Closed") + ", Relay 2 Status: " + (relay2Status ? "Open" : "Closed");
  server.send(200, "text/plain", sensorData);
}
