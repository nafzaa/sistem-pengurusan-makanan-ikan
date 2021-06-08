#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DS3232RTC.h>     
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

DS3232RTC myRTC;

//for ds18b20
#define ONE_WIRE_BUS D4 
#define SensorPin A0         // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback

OneWire oneWire(ONE_WIRE_BUS);      
DallasTemperature sensors(&oneWire); // pass oneWire to DallasTemperature library

time_t t;

float b;
float tempCelsius; 
int buf[10],temp;

int hourssenddata = 0;
int minutessenddata = 0;

String hours;
String minutes;
String minutes1;

String jam;
String minit;
String minit1;

String timer = "";
String timer1 = "";

String phstring = "";
String tempstring = "";

String hari;
String bulan;

String days;
String months;
String years; 
String date;

const char* address;
const char* timerjson;
const char* datejson;

String jsontimer;
String jsondate;

Servo myservo;

int state = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

const char* wifiName = "Poco"; // your wifi name
const char* wifiPass = "nafza9494"; //your wifi password

//Web Server address to read/write from 
const char *host = "http://192.168.202.206/testapi/api.php?id=1";  //change your api url

String host2;

String data2;

void setup() {
  
  Serial.begin(9600);
  sensors.begin();
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
  Readsensor(); 
  getdata();  
  delay(2000);
  
  display(); 
  Serial.print("realtimer: ");
  Serial.print(timer);
  Serial.print(" jsontimer: ");
  Serial.print(jsontimer);
  Serial.print(" date: ");
  Serial.print(date);
  Serial.print(" jsondate: ");
  Serial.println(jsondate);
  Serial.println(hourssenddata);

  if (timer == jsontimer && date == jsondate){     
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

  if (hour(t) == hourssenddata && minute(t) == minutessenddata){
    hourssenddata++;
    senddata();
    }
  
}

void senddata(){
  HTTPClient http2;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host2);
  
  http2.begin(host2);     //Specify request destination
  
  int httpCode2 = http2.GET();            //Send the request
  String payload2 = http2.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode2);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload2);    //Print request response payload
  
  if(httpCode2 == 200)
  {
    Serial.println("send data oky");
  }
  else
  {
    Serial.println("Error in response");
  }

  http2.end();  //Close connection
  }

void getdata(){
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host);
  
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  data2 = payload.substring(1,payload.length()-1);

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

    address = doc["Kolam"];
    timerjson = doc["Masa"];
    datejson = doc["Tarikh"];

    jsontimer = String(timerjson);
    jsondate = String(datejson);
  
  
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(address);
    Serial.println(timerjson);
    Serial.println(datejson);

  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  }


void display()
{
    char buf[40];
    t = myRTC.get();

     days = String(day(t));
     months = String(month(t));
     years = String(year(t));

     
    
    if (state3 == 0){
      hourssenddata = hour(t);
      minutessenddata = minute(t);
      state3 = 1;
      }  

    
    if (hour(t) == 0){
      if(state4 == 0){
        hourssenddata =0;
        state4 = 1;
        }
      }

    if (hour(t) == 1){
      state4 = 0;
      }

    if (hourssenddata == 24){
      hourssenddata = 0;
      }

      
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

    if (day(t) <= 9){
      hari = "0" + days;
      }
    else {
      hari = days;
      }

    if (day(t) <= 9){
      bulan = "0" + months;
      }
    else {
      bulan = months;
      }
    
   
    timer = jam + ":" + minit;
    timer1 = jam + ":" + minit1; 
    date = years + "-" + bulan + "-" + hari;
    //Serial.println (timer);
    
}

void Readsensor(){
  sensors.requestTemperatures();             // send the command to get temperatures
  tempCelsius = sensors.getTempCByIndex(0);  // read temperature in Celsius

  
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(100);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue; 

  Serial.print("Temperature: ");
  Serial.print(tempCelsius);    // print the temperature in Celsius
  Serial.print("°C");
  Serial.print(" ");
  Serial.print("pH: ");  
  Serial.println(phValue,2);

  phstring = String(phValue,2);
  tempstring = String(tempCelsius);

  host2 = "http://192.168.202.206/testapi/insertdata.php?ph=" + phstring + "&temp=" + tempstring; //change your api url
  }
