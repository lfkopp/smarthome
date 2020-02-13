#include <DHT.h>
#include <ESP8266WiFi.h>


#define DHTPIN 2          // DHT11 is connected to GPIO Pin 2
#define LED 0             // LED D3
#define LED_CONN 5        // LED D1


String apiKey = "DCSU8341011FNILR";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "juju";     // Enter your WiFi Network's SSID
const char* pass =  "pedrinho"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";


float humi;
float temp;

DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
    pinMode(LED_CONN, OUTPUT);
  digitalWrite(LED_CONN, LOW);
  Serial.begin(9600);
  delay(100);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(3000);
    Serial.print("*");

  }
  Serial.println("");
  Serial.println("***WiFi connected***");

}

void loop()
{
  humi = 0;
  temp = 0;
  for (int i = 1; i <= 10; i++) {
    Serial.print(i);
    Serial.print("\t");
    humi += dht.readHumidity() / 10;
    temp += dht.readTemperature() / 10;
    Serial.print(humi / i * 10);
    Serial.print("\t");
    Serial.println(temp / i * 10);
    delay(6000);

  }

  digitalWrite(LED_CONN, LOW);
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String sendData = apiKey + "&field5=" + String(temp) + "&field6=" + String(humi) + "\r\n\r\n";

    Serial.println(sendData);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(sendData.length());
    client.print("\n\n");
    client.print(sendData);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("deg C. Humidity: ");
    Serial.print(humi);
    Serial.println("%. Connecting to Thingspeak.");
  }

  client.stop();

  digitalWrite(LED, HIGH);
  delay(30000);
  digitalWrite(LED, LOW);
  digitalWrite(LED_CONN, HIGH);
}
