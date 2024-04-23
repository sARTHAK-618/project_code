#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* ssid = "Indu";
const char* password = "seahawks";
const int DHTPin = 4;    // Pin to which the DHT sensor is connected
const int DHTType = DHT22;    // DHT type (DHT11, DHT21, DHT22)
const int waterLevelPin = 5; // Pin to which the water level sensor switch module is connected
const int ledPin = 2; // Pin to which the LED is connected
const int alertLedPin = 26; // Pin to which the additional LED for water level alert is connected
DHT dht(DHTPin, DHTType);
WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  pinMode(alertLedPin, OUTPUT);
  digitalWrite(alertLedPin, LOW); 
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
  int waterLevel = digitalRead(waterLevelPin); 
  if (humidity >= 80.0) {
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW);
  }

  if (waterLevel == HIGH) { 
    digitalWrite(alertLedPin, HIGH); 
  } else {
    digitalWrite(alertLedPin, LOW);
  }
}

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int waterLevel = digitalRead(waterLevelPin); 
  String webpage = "<html><head><title>DHT22 Sensor Data</title>";
  webpage += "<script>function refreshData() { fetch('/data').then(response => response.text()).then(data => { document.getElementById('sensorData').innerHTML = data; }); } setInterval(refreshData, 1000); </script>";
  webpage += "</head><body><h1>DHT22 Sensor Data</h1>";
  webpage += "<div id='sensorData'>";
  webpage += "<p>Humidity: " + String(humidity) + "</p>";
  webpage += "<p>Temperature: " + String(temperature) + "</p>";
  // Display water level information
  webpage += "<p>Water Level: " + String(waterLevel) + "</p>";
  webpage += "</div></body></html>";
  server.send(200, "text/html", webpage);
}

void handleSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int waterLevel = digitalRead(waterLevelPin); 
  String sensorData = "Humidity: " + String(humidity) + "%, Temperature: " + String(temperature) + ", Water Level: " + String(waterLevel);
  server.send(200, "text/plain", sensorData);
}
