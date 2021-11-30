#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h> //6.17.3

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 10);

EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.0.102"; // change to your PC's IP address
String PATH_NAME   = "/iot/data-api.php";
String getData;

void setup() {
  Serial.begin(115200);

  // initialize the Ethernet Setting
  Ethernet.begin(mac, ip);
  Serial.println("Menghubungkan...");
  delay(5000);  
  client.connect(HOST_NAME, HTTP_PORT);
}

void loop() {
  //Baca data
  String namadevice="iwancilibur";
  float sensor1=random(27,50);
  float sensor2=random(27,50);
  
// make a HTTP request:
    // send HTTP header
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + PATH_NAME + 
                   "?namadevice=" + String(namadevice) + 
                   "&sensor1=" + String(sensor1) + 
                   "&sensor2=" + String(sensor2) + 
                   " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    
    while(client.connected()) {
      if(client.available()){
        char endOfHeaders[] = "\r\n\r\n";
        client.find(endOfHeaders);
        getData = client.readString();
        getData.trim();
        Serial.println(getData);

        //AMBIL DATA JSON
        const size_t capacity = JSON_OBJECT_SIZE(6) + 110; //cari dulu nilainya pakai Arduino Json 5 Asisten
        DynamicJsonDocument doc(capacity);
        //StaticJsonDocument<192> doc;
        DeserializationError error = deserializeJson(doc, getData);
      
        const char* waktu_dibaca      = doc["waktu"]; // "2021-10-12 09:18:55"
        const char* namadevice_dibaca = doc["namadevice"]; // "iwancilibur"
        const char* sensor1_dibaca    = doc["sensor1"]; // "44"
        const char* sensor2_dibaca    = doc["sensor2"]; // "40"
        const char* control1_dibaca   = doc["control1"]; // "0"
        const char* control2_dibaca   = doc["control2"]; // "0"
        
       //POST TO SERIAL
       Serial.print("Waktu      = ");Serial.println(waktu_dibaca);
       Serial.print("Nama Device= ");Serial.println(namadevice_dibaca);
       Serial.print("Sensor 1   = ");Serial.println(sensor1_dibaca);
       Serial.print("Sensor 2   = ");Serial.println(sensor2_dibaca);
       Serial.print("Control 1  = ");Serial.println(control1_dibaca);
       Serial.print("Control 2  = ");Serial.println(control2_dibaca);
       Serial.println();
      
       //LOGIKA
       if(String(control1_dibaca)=="1"){
        Serial.println("CONTROL 1 ON");
       }else{
        Serial.println("CONTROL 1 OFF");
       }
      }
      
    }
    delay(1000);
}
