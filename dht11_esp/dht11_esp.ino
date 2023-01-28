#include <DHT.h>
#include <ESP8266WiFi.h>


#define DHTPIN 2          //DHT11 is connected to GPIO Pin 2
#define LED1 5
#define LED2 0
String apiKey = "DCSU8341011FNILR";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "amigos do pedro";     // Enter your WiFi Network's SSID
const char* pass =  "fluminense1"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";


float humi;
float temp;
float hi;

DHT dht(DHTPIN, DHT11);
WiFiClient client;
 
void setup() 
{
      pinMode(LED1, OUTPUT);  
      pinMode(LED2, OUTPUT);  

       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);

       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED){
            delay(100);
            Serial.print("*");
     }
      Serial.println("");
      Serial.println("***WiFi connected***");
 
}
 
void loop() 
{
  
      humi = dht.readHumidity();
      temp = dht.readTemperature();
      hi =   dht.computeHeatIndex(temp, humi, false);
      if (client.connect(server,80)  )  {  
       String sendData = apiKey+"&field5="+String(temp)+"&field6="+String(humi)+"&field7="+String(hi)+"\r\n\r\n"; 
       Serial.println(sendData);
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(sendData.length());
       client.print("\n\n");
       client.print(sendData);
       Serial.print("Temperature: ");
       Serial.print(temp);
       Serial.print("deg C. Humidity: ");
       Serial.print(humi);
       Serial.print("%. Heat index: ");
       Serial.print(hi);
       Serial.println(" Connecting to Thingspeak.");
       }
      
      client.stop();

      Serial.println("Sending....");

  for (int i=0 ; i<15; i++){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(1000); 
  digitalWrite(LED1, LOW); 
  delay(1000); 
  digitalWrite(LED2, LOW); 
  delay(1000); 
}
}
