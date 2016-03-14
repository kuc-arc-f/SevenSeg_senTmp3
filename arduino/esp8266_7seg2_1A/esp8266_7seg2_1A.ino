#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

const char* host = "api.thingspeak.com";
String mTimeStr="";
static uint32_t mTimerTmp;
String mAPI_KEY="your-API-key";

//
void setup() {
  Serial.begin( 9600 );
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
}

//
void proc_http(String sTemp){
//Serial.print("connecting to ");
//Serial.println(host);  
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      String url = "/update?key="+ mAPI_KEY + "&field1="+ sTemp;        
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" + 
        "Connection: close\r\n\r\n");
      delay(10);      
      int iSt=0;
      while(client.available()){
          String line = client.readStringUntil('\r');
Serial.print(line);
      }    
}

//
void loop() {
  delay( 100 );
  String sRecv="";
  while( Serial.available()){
      char c = Serial.read();
      sRecv.concat(c);
      if(  sRecv.length() >= 6 ){
          if (millis() > mTimerTmp) {
              mTimerTmp = millis()+ 30000;
              String sHead= sRecv.substring(0,4);
              if(sHead =="tmp="){
                  String sBuff = sRecv.substring( 4, 6 );
  Serial.print("sBuff=");
  Serial.println( sBuff );       
                  proc_http(sBuff);     
                  delay(1000);    
              }               
          }
      }
  } //end_while    
    
}


