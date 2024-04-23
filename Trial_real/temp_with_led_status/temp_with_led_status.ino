#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* ssid = "Indu";
const char* password = "seahawks";

const int DHTPin = 4;  
const int DHTType = DHT22;  

const int ledPin = 2;

DHT dht(DHTPin, DHTType);

WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 

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

  server.on("/", HTTP_GET, handleRoot);


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
  webpage += "<p>LED Status: " + String(digitalRead(ledPin)) + "</p>";
  webpage += "</div></body></html>";

  server.send(200, "text/html", webpage);
}

void handleSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String ledStatus = digitalRead(ledPin) ? "ON" : "OFF"; 
  String sensorData = "Humidity: " + String(humidity) + "%, Temperature: " + String(temperature) + ", LED Status: " + ledStatus;
  server.send(200, "text/plain", sensorData);
}
