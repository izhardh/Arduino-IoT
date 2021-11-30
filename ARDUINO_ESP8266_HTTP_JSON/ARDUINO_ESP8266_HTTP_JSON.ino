#include <WiFiEsp.h>
#include <ArduinoJson.h> //6.17.3
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3); // RX, TX

char ssid[]        = "delameta";            // your network SSID (name)
char pass[]        = "bilano1407";        // your network password
int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.1.12"; // change to your PC's IP address
String PATH_NAME   = "/iotizhardh/data-api.php";
String getData;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  Serial.begin(115200);
  wifiSerial.begin(9600);
  WiFi.init(&wifiSerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield Tidak terdeteksi");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Menghubungkan ke SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("Terhubung ke jaringan!");
  printWifiStatus();
  Serial.println();
  Serial.println("Mengubungkan ke Server...");
  // if you get a connection, report back via serial
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Berhasil terhubung ke Server!");
  }
}

void loop()
{
  //Baca data
  String namadevice="izhardh";
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
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
