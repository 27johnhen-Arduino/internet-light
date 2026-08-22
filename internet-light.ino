#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "network name";
char pass[] = "network password";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

IPAddress local_IP(192, 168, 1, 225);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  
  server.begin();
  printWifiStatus();
}


void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<!DOCTYPE html>");
            client.print("<html>");
            client.print("<head>");
            client.print("<title>LED control</title>");
            client.print("</head>");
            client.print("<body style='background-image: linear-gradient(to right, rgb(248, 86, 86), rgb(92, 245, 151)); color:rgb(82, 0, 205);'>");
            client.print("<h1>LED control</h1>");
            client.print("<p>");
            client.print("    <a href='on'><button style='border-width:10px;font-size: 3em;background-color:rgb(255, 242, 0);color:rgb(0, 120, 189); border-color:rgb(0, 162, 255)'>Turn LED on!</button></a><br><br>");
            client.print("    <a href='off'><button style='border-width:10px;font-size: 3em;background-color:rgb(255, 242, 0);color:rgb(0, 120, 189); border-color:rgb(0, 162, 255)'>Turn LED off!</button></a><br><br>");
            client.print("</p>");
            client.print("</body>");
            client.print("</html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /on")) {
          digitalWrite(2, HIGH);
        }
        if (currentLine.endsWith("GET /off")) {
          digitalWrite(2, LOW);
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("Turn on and off the LED! Go to http://");
  Serial.println(ip);
}
