#include <SPI.h>
#include <Ethernet.h>
int lmPin = A0;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {10,0,0,2};
byte gateway[] = {10,0,0,1};
byte subnet[] = {255, 255, 255, 0};
EthernetServer server(80);
String readString;

void setup(){
  pinMode(6, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  Serial.println("Led and Sensor Test Automation");
}

float tempC(){
  float raw = analogRead(lmPin);
  float percent = raw/1023.0;
  float volts = percent*5.0;
  return 100.0*volts;
}

void loop(){
  EthernetClient client = server.available();
  if(client){
      while(client.connected()){
    if(client.available()){
      char c = client.read();
  
      if(readString.length()<100){
        readString += c;
      }
  
      if(c=='\n'){
        Serial.println(readString);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println("Refresh: 1");
        client.println();
        client.println("<HTML>");
        client.println("<HEAD>");
        client.println("<TITLE>TEST AUTOMATION</TITLE>");
        client.println("</HEAD>");
        client.println("<BODY bgcolor='green'>");
        client.println("<CENTER><H1>TEST LED AND SENSOR AUTOMATION");
        client.println("<hr/>");
        client.println("<br/>");
        client.println("<a href=\"/?lighton\"\">Light ON</a>");
        client.println("<a href=\"/?lightoff\"\">Light OFF</a><br/>");
        client.print("Suhu Ruangan = ");
        client.print(tempC());
        client.print(" C");
  
        client.println("</BODY>");
        client.println("</HTML>");
  
        delay(1);
        client.stop();
  
        if(readString.indexOf("?lighton")>0)
        {
          digitalWrite(6, HIGH);
          Serial.println("LED ON");
        }
  
        else{
          if(readString.indexOf("?lightoff")>0){
          digitalWrite(6, LOW);
          Serial.println("LED Off");
          }
        }
        readString="";
        Serial.println(tempC());
      }
    }
  }
 }
}

