#include <ESP8266WiFi.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


#define ONE_WIRE_BUS D4 //กําหนดการใช้งานที ขาารการรับส่งข้อมูลของเซ็นเซอร์D2
OneWire oneWire(ONE_WIRE_BUS); //ประกาสฟังชันการสื อสายแบบเส้นเดียวกับเซ็นเซอร์ 
DallasTemperature sensors(&oneWire); //ประกาศฟังชันเพื อใช้ตัวแปรสําหรับตอบโต้กับเซ็นเซอร์ 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); 

////////// PH
float calibration_value = 21.34 + 3.75 ;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;


#define WIFI_SSID "TELECOM08_2.4G"

#define WIFI_PASSWORD "162404710008"

#include <WiFiClientSecureBearSSL.h>


float C = 0; //ประกาศตัวแปลแบบทศนิยม และเรียกใช้การอ่านค่า อุณหภูมิ
float F = 0; //ประกาศตัวแปลแบทศนิยม และเรียกใช้การอ่านค่าอุณหภูมิ 

#include "httpclient.hpp";
smarthttp init_http;
//==============================================================================

//===============================================

void setup() {
  
  lcd.begin();
  lcd.backlight();
  sensors.begin();
  
  Serial.begin(115200); // เรียกใช้งานซีเรียลคอนโซล
  
  Serial.setDebugOutput(true);
  
  //แสดงข้อความใน Serial Monitor
  
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // เริมเชื อมต่อวายฟาย
   
  Serial.print("Connecting to "); // แสดงคําว่า "Connecting to" ในซีเรียลคอนโซล 
  // แสดงคําว่า "WiFi Connected:" และหมายเลข IP ที ได้รับในซีเรียลคอนโซล129
  Serial.println("");
  
  Serial.println("WiFi connected");
  
  Serial.println(WiFi.localIP());
  
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
  
  Serial.print(".");
  
  delay(500);
  
  }
  WiFi.setAutoReconnect(true);
  
  
  sensors.begin(); // เริมเชื อมต่อ sensors
}

void loop() {

  {
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*3.3/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
  

 }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    delay(2000);
    return;
  }
  
  if (isnan(C) || isnan(F)) //กําหนดเงื่อนไขค่า C,F
  
  {
  
  Serial.println("Error reading DHT!");
  
  delay(1000);
  
  return;
  
  }

    sensors.requestTemperatures(); //สังอ่านค่าอุณหภูมิ
    C = sensors.getTempCByIndex(0); //ประกาศตัวแปลแบบทศนิยม และเรียกใช้การอ่านค่า อุณหภูมิ
    F = sensors.toFahrenheit(C); //ประกาศตัวแปลแบทศนิยม และเรียกใช้การอ่านค่าอุณหภูมิ 
    Serial.print("Sensor : 1 ");
    
    Serial.print("\t");
    
    Serial.print("Celsius: ");
    
    Serial.print(C);
    
    Serial.print("\t");
    
    Serial.print("Fahrenheit: ");
    
    Serial.print(F);
    
    Serial.print("\t");
    
    Serial.print("PH value: ");
    
    Serial.println(ph_act);
    
    Serial.print("\t");
    
    Serial.println("..........................................................................");
  
}