#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DS3232RTC.h>     
DS3232RTC myRTC;
#include <Servo.h>

String hours;
String minutes;
String minutes1;

String (jam);
String (minit);
String minit1;

String timer = "";
String timer1 = "";

const char* address;
const char* timerjson;

Servo myservo;

int state = 0;
int state2 = 0;

const char* wifiName = "Poco";
const char* wifiPass = "nafza9494";

//Web Server address to read/write from 
const char *host = "http://192.168.202.206/testapi/api.php?id=1";

String data2;

void setup() {
  
  Serial.begin(9600);
  myRTC.begin();
  myservo.attach(D6);
  myservo.write(0);
  delay(10);
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
}

void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host);
  
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  data2 = payload.substring(1,53);

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(data2);    //Print request response payload
  
  if(httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, data2);

    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

    address = doc["address"];
    timerjson = doc["timer"];
  
  
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(address);
    Serial.println(timerjson);

  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  
  delay(1000);  //GET Data at every 5 seconds
  display();
  Serial.println(timer);

  if (timer == timerjson){     
        if (state == 0){
          state2 = 0;
          Serial.println("servo on");
          myservo.write(180);
          delay(1000);
          myservo.write(0);
          delay(1000);
          state = 1;  
          }  
      }

  if (timer == timer1){
    Serial.println("update state 0");
    state = 0;
    }

  
}


void display()
{
    char buf[40];
    time_t t = myRTC.get();
//    sprintf(buf, "%.2d:%.2d ",
//        hour(t), minute(t));
    hours = String(hour(t));
    minutes = String (minute(t));

    if(state2 == 0){
      minutes1 = String (minute(t) + 1);
      state2 = 1;
      } 

    if (hour(t) <= 9){
      jam = "0" + hours;
      }
    else {
      jam = hours;
      }

    if (minute(t) <= 9){
      minit = "0" + minutes;
      }
    else {
      minit = minutes;
      }

    if (minute(t) <= 9){
      minit1 = "0" + minutes1;
      }
    else {
      minit1 = minutes1;
      }
   
    timer = jam + ":" + minit;
    timer1 = jam + ":" + minit1; 
    //Serial.println (timer);
    
}
