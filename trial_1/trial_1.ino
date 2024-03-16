#include<WiFi.h>
#include<ESP32Servo.h>

const char* ssid = "Indu";
const char* password = "seahawks";

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi Connected.");
  Serial.println("IP address---");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("New Client.");
    String currentline = "";
    while(client.connected())
    {
      if(client.available())
      {
        char c = client.read();
        Serial.write(c);
        if (c=="\n")
        {
          if(currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            //html code here
            client.println();

            client.print("");

            break;
          }
          else
          {
            currentline = "";
          }
        }
        else if (c != "/r")
        {
          currentline += c;
        }
        // if else statements here....
        if (currentLine.endsWith("GET /"))
        {

        }
        if (currentLine.endsWith("GET / "))
        {
          
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}