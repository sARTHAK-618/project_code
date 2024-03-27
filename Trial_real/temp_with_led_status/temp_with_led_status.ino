#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* ssid = "Indu";
const char* password = "seahawks";

const int DHTPin = 4;  // Pin to which the DHT sensor is connected
const int DHTType = DHT22;  // DHT type (DHT11, DHT21, DHT22)

const int ledPin = 2; // Pin to which the LED is connected

DHT dht(DHTPin, DHTType);

WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is initially off

  // Connect to Wi-Fi
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

  // Route for root / web page
  server.on("/", HTTP_GET, handleRoot);

  // Route for sensor data
  server.on("/data", HTTP_GET, handleSensorData);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  float humidity = dht.readHumidity();

  if (humidity >= 80.0) {
    digitalWrite(ledPin, HIGH); // Turn on LED
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }
}

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String webpage = "<html><head><title>DHT22 Sensor Data</title>";
  webpage += "<script>function refreshData() { fetch('/data').then(response => response.text()).then(data => { document.getElementById('sensorData').innerHTML = data; }); } setInterval(refreshData, 1000); </script>";
  webpage += "</head><body><h1>DHT22 Sensor Data</h1>";
  webpage += "<div id='sensorData'>";
  webpage += "<p>Humidity: " + String(humidity) + "%</p>";
  webpage += "<p>Temperature: " + String(temperature) + "</p>";
  webpage += "<p>LED Status: " + String(digitalRead(ledPin)) + "</p>"; // Include LED status
  webpage += "</div></body></html>";

  server.send(200, "text/html", webpage);
}

void handleSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String ledStatus = digitalRead(ledPin) ? "ON" : "OFF"; // Convert LED status to string
  String sensorData = "Humidity: " + String(humidity) + "%, Temperature: " + String(temperature) + ", LED Status: " + ledStatus;
  server.send(200, "text/plain", sensorData);
}
