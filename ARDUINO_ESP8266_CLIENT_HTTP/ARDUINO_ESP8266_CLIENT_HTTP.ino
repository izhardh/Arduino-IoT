#include<KRwifi.h>
#include <ArduinoJson.h> //6.17.3

char* ssid    = "iotgateway";
char* pass    = "iotgateway";
char* server  = "192.168.0.102";

void setup() {
  Serial.begin(115200);
  setWifi(ssid, pass);
}
 
void loop() {
   String namadevice="iwancilibur";
   float sensor1=random(27,50);
   float sensor2=random(27,50);
   httpGet(server, "/iot/data-api.php?namadevice=" + String(namadevice) + 
          "&sensor1=" + String(sensor1)+
          "&sensor2=" + String(sensor2), 80);
  Serial.println("Respon: ");
  Serial.println(getData);
  Serial.println();
  //delay(2000);
  
  //AMBIL DATA JSON
  const size_t capacity = JSON_OBJECT_SIZE(6) + 110; //cari dulu nilainya pakai Arduino Json 5 Asisten
  DynamicJsonDocument doc(capacity);
  //StaticJsonDocument<192> doc;
  DeserializationError error = deserializeJson(doc, getData);

  //if (error) {
  //  Serial.print(F("deserializeJson() failed: "));
  //  Serial.println(error.f_str());
  //  return;
  //}
  
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
