/*........................
   ESW Project
........................*/

#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "ThingSpeak.h"

unsigned long myChannelNumber = 921115;
const char * myWriteAPIKey = "---";


char* ssid = "wifiSSID";
char* password = "wifipassword"; 
WiFiClient  client;

int relay1 = 16;
int relay2 = 17;
int relay3 = 18;
int relay4 = 19;

float readin(){
  float ret=0.00,reading;
  int val;
  for(short i=1; i<35 ; i++)
  {
    val= analogRead(34);
    val = val * 5;
    reading = (float)val / 4096;
    ret+=reading;
  if(i!=1)
  ret/=2;
  }  
  return ret;
}
float readout(){
  float ret=0.00,reading;
  int val;
  for(short i=1; i<35; i++)
  {
    val= analogRead(32);
    val = val * 5;
    reading = (float)val / 4096;
    ret+=reading;
    
    if(i!=1)
        ret/=2;
  }  
  return reading;
}

void setup() {

  Serial.begin(115200);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  ThingSpeak.begin(client);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
   Serial.println(WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
 
  pinMode(32, INPUT); //analog in S01 
  pinMode(34, INPUT); //analog in S02
 }

    bool up = 0, dwn = 0;
    int adcv1, adcv2;
    short count=0;
    float op1, op2;
    short flag = 0;
    long int reset_count = 0;
    short iot_send = 0;
    int loop_stop = 0;

void show(){
      Serial.print("UP:");
      Serial.println(op1);
      Serial.print("DOWN:");
      Serial.println(op2);
      Serial.println("count");
      Serial.println(count);
}

void loop() {
  reset_count++;
  op1 = readin();
  op2 = readout(); 
  loop_stop=0; 

  if(op1>2.00 && op2>2.00)
  goto label;

  //Sensor 02 work down
  if (op2 > 1.30 && op1 < 3.00) {
      loop_stop=0;
      Serial.println("OUT loop");
      show();
  while(op1 < 1.95){
    delay(4);
    loop_stop++;
    
    op1 = readin();
    if(loop_stop > 400)
    goto label;
  }
  loop_stop=0;
  if(count>0)
  count--;
  delay(700);   // Wait time after one change. Experimental value.
  goto label;
  }
  op1 = readin();
  op2 = readout();
  //Sensor 01 work up
  if (op1 > 1.95 && op2 < 0.90) {
          loop_stop=0;
          Serial.println("IN loop");
          show();
    while(op2 < 0.90){
      delay(4);
    loop_stop++;
  
    op2 = readout();
    if(loop_stop > 400)
    goto label;
  }
  loop_stop=0;
  if(count<140)
  count++;
  delay(700);
  }
  
  label:
  show();
   
  iot_send++;

//SENDS DATA EVERY 400*DELAY SECONDS.
    
    if(iot_send == 400){
      iot_send = 0;           //Sending data every 12 seconds
      int httpCode = ThingSpeak.writeField(myChannelNumber, 1, count, myWriteAPIKey);

      if (httpCode == 200) {
        Serial.println("Channel write successful.");
      }
      else {
        Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
      }

    }
    
//Relays are in Normally Closed mode

 if(count<1)
      digitalWrite(relay1, LOW);
   else
      digitalWrite(relay1, HIGH);

   if(count<3)
      digitalWrite(relay2, LOW);
   else
      digitalWrite(relay2, HIGH);

   if(count<5)
      digitalWrite(relay3, LOW);
   else
      digitalWrite(relay3, HIGH);

   if(count<9)
      digitalWrite(relay4, LOW);
   else
      digitalWrite(relay4, HIGH);

  if(reset_count > 9999999) {
    count = 0;      //reset after roughly 90 minutes.
    reset_count = 0;
  }
   
      delay (30);
}

//###----------------END----------------###
