#include <ESP8266WiFi.h>
//#include <TridentTD_LineNotify.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define ONE_WIRE_BUS D4              //กําหนดการใช้งานที ขาารการรับส่งข้อมูลของเซ็นเซอร์D2
OneWire oneWire(ONE_WIRE_BUS);       //ประกาสฟังชันการสื อสายแบบเส้นเดียวกับเซ็นเซอร์
DallasTemperature sensors(&oneWire); //ประกาศฟังชันเพื อใช้ตัวแปรสําหรับตอบโต้กับเซ็นเซอร์
// line token ที ใช้ในการส่ง
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
////////// temp
float calibration_value = 21.34 + 5.1;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;

#define LINE_TOKEN "XCVU2Ty3kR5lDu90V6ZDX9CrceeBMBgk4ddTV8Ukeeo"

// Config Firebase

#define FIREBASE_HOST "https://pp1212-6ed93-default-rtdb.asia-southeast1.firebasedatabase.app"

#define FIREBASE_AUTH "IQjvNI2puRUdMnf6Ws9SoH8DN2PtLuAwoNa9frGF"

// ตังค่าวายฟายและฐานข้อมูล firebase ที ต้องการจะเชือมต่อ

// 127

/*
#define WIFI_SSID "Pond"

#define WIFI_PASSWORD "31032543"
*/
#define WIFI_SSID "TELECOM08_2.4G"

#define WIFI_PASSWORD "162404710008"

// Config GoogleSheetURL
const char *host = "script.google.com";
const int httpsPort = 443;
String GoogleSheetURL = "https://script.google.com/macros/s/AKfycbwPM6CDfAZnDzPQHr6KTPmRggVm9XpS-ln8eu7qgasPHyhdRj6dRGlSu6inJZodwL6J/exec";

#include <WiFiClientSecureBearSSL.h>

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
// pp1212-6ed93-default-rtdb.asia-southeast1.firebasedatabase.app  *.asia-southeast1.firebasedatabase.app  — 85:43:74:79:F3:5A:9B:87:73:3E:B8:1E:9D:C2:E3:78:4B:15:14:51
const uint8_t fingerprint[20] = {0x84, 0x6A, 0x0D, 0x58, 0xE4, 0xE0, 0x85, 0xC4, 0x1A, 0x18, 0x57, 0xB7, 0xE7, 0x70, 0x58, 0xBA, 0xA3, 0x1E, 0x8C, 0x52};
WiFiUDP udp; // ประกาศตัวแปร udp เพื อใช้งาน UDPocolProt

NTPClient timeClient(udp, "kr.pool.ntp.org", 3600 * 7); //กําหนดไคลเอ็นต์ NTP เพื อรับวันที และ เวลา

int CurrentWiFiSignal = 0; //ประกาศตัวแปร CurrentWiFiSignal

//======================================
unsigned long last_line = 0;
unsigned long last_firebase = 0;
unsigned long last_sensor = 0;

float C = 0; //ประกาศตัวแปลแบบทศนิยม และเรียกใช้การอ่านค่า อุณหภูมิ
float F = 0; //ประกาศตัวแปลแบทศนิยม และเรียกใช้การอ่านค่าอุณหภูมิ
int adc = 0; //ประกาศตัวแปรแบะกําหนดค่าเริ มตัน ของอนาล็อก
#include "httpclient.hpp";
smarthttp init_http;
//==============================================================================
bool line_notify(String &data)
{
  return init_http.doPost("https://notify-api.line.me/api/notify", "message=" + data, LINE_TOKEN);
}
//===============================================

void setup()
{

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

  // 129
  Serial.println("");

  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP());

  Serial.println("");

  //  ถ้าไม่สามารถเชื อมต่อวายฟายได้ให้แสดง คอนโ"."ในซลีเรียล

  //  ลูปเช็คสถานะวายฟายทุกๆ ครึ งวินาที (500 Millisecound)

  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");

    delay(500);
  }
  WiFi.setAutoReconnect(true);

  timeClient.begin(); //เริมเชื อมต่อ timeClient

  // LINE.setToken(LINE_TOKEN); //ประกาศเรียกใช้line entok

  // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // เริมเชื อมต่อ Firebase

  sensors.begin(); // เริมเชื อมต่อ sensors

  // Serial.println(LINE.getVersion()); //แสดงเวอร์ชั่นของไลน์
  init_http.DEBUG_MODE = true;
}

////-- END CODE BEGIN AND SETUP --////

void loop()
{

  {
    for (int i = 0; i < 10; i++)
    {
      buffer_arr[i] = analogRead(A0);
      delay(30);
    }
    for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buffer_arr[i] > buffer_arr[j])
        {
          temp = buffer_arr[i];
          buffer_arr[i] = buffer_arr[j];
          buffer_arr[j] = temp;
        }
      }
    }
    avgval = 0;
    for (int i = 2; i < 8; i++)
      avgval += buffer_arr[i];
    float volt = (float)avgval * 3.3 / 1024 / 6;
    ph_act = -5.70 * volt + calibration_value;

    // phValue=14.2-phValue;
    // float phValue = -3.0 * pHVol+17.5;
    // Serial.println("pH Val: ");
    // Serial.print(ph_act);
    // lcd.clear();
  }

  {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("Sensor 1");
    lcd.setCursor(0, 1);
    lcd.print("Temp     : ");
    lcd.setCursor(11, 1);
    lcd.print(C);
    lcd.setCursor(16, 1);
    lcd.print(" C");
    lcd.setCursor(0, 2);
    lcd.print("ph Value : ");
    lcd.setCursor(12, 2);
    lcd.print(ph_act);

    // lcd.setCursor(10,2);
    // lcd.print("=");
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Reconnecting to WiFi...");
    delay(2000);
    return;
  }
  unsigned long last_time = millis();

  if (isnan(C) || isnan(F)) //กําหนดเงื่อนไขค่า C,F

  {

    Serial.println("Error reading DHT!");

    delay(1000);

    return;
  }

  //หาค่าสัญญาณ RSSI ของ WIFI แล้ว MAP ให้อยู่ในรูปของเป์เซ็นต์ int CurrentWiFiSignal = WiFi.RSSI();

  if (CurrentWiFiSignal > -30)
    CurrentWiFiSignal = -30;

  if (CurrentWiFiSignal < -90)
    CurrentWiFiSignal = -90;

  int WifiSignal = map(CurrentWiFiSignal, -90, -30, 0, 100);

  timeClient.update();

  //ประกาศตัวแปรและกําหนดค่า เวลาวโมงชั.นาที.วินาที

  int hour = timeClient.getHours();

  int minute = timeClient.getMinutes();

  int second = timeClient.getSeconds();

  unsigned long epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime((time_t *)&epochTime); //สรร้างโครงสร้างของtm *ptm เพื อรับข้อมูล วัน เดือน ปี

  // 131

  //ประกาศตัวแปรและกําหนดค่า วัน เดือน ปี
  int currentMonth = ptm->tm_mon + 1;
  int monthDay = ptm->tm_mday;

  int currentYear = ptm->tm_year + 1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

  String currentTime = timeClient.getFormattedTime();
  String currentDateTime = currentDate + " " + currentTime;

  if (last_time - last_sensor >= 2000)
  {
    sensors.requestTemperatures(); //สังอ่านค่าอุณหภูมิ
    last_sensor = last_time;
    C = sensors.getTempCByIndex(0); //ประกาศตัวแปลแบบทศนิยม และเรียกใช้การอ่านค่า อุณหภูมิ
    F = sensors.toFahrenheit(C);    //ประกาศตัวแปลแบทศนิยม และเรียกใช้การอ่านค่าอุณหภูมิ
    // adc = analogRead(A0); //ประกาศตัวแปรแบะกําหนดค่าเริ มตัน ของอนาล็อก
    Serial.print("Analog value: 1  =  ");

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

    //    Serial.print("Time:");
    //
    //    Serial.print(currentDateTime);
    //
    //    Serial.print("\t");
    //
    //    Serial.print(" RSSI: ");
    //
    //    Serial.print(CurrentWiFiSignal);
    //
    //    Serial.print("\t");
    //
    //    Serial.print("WIFI:");
    //
    //    Serial.println(WifiSignal);

    Serial.println("..........................................................................");
  }

  if (last_time - last_line >= 30000)
  {

    if (C > 30)
    {

      String LineText;
      String string1 = "เซ็นเซอร์วัดอุณหภูมิตัวที่1 สูงกว่ากำหนด";
      String string2 = " C";
      LineText = string1 + C + string2;
      Serial.print("Line ");
      Serial.println(LineText);

      line_notify(LineText);
    }

    if (ph_act > 9)
    {

      String LineText;
      String string10 = "เซ็นเซอร์วัดค่า PH ตัวที่1 สูงกว่ากำหนด";
      String string11 = " PH";
      LineText = string10 + ph_act + string11;
      Serial.print("Line ");
      Serial.println(LineText);

      line_notify(LineText);
    }

    if (ph_act <= 6)
    {

      String LineText;
      String string12 = "เซ็นเซอร์วัดค่า PH ตัวที่1 ต่ำกว่ากำหนด";
      String string13 = " PH";
      LineText = string12 + ph_act + string13;
      Serial.print("Line ");
      Serial.println(LineText);

      line_notify(LineText);
    }

    if (C <= 26)
    {
      String LineText;
      String string1 = "เซ็นเซอร์วัดอุณหภูมิตัวที่1 ต่ำกว่ากำหนด";
      String string2 = " C";
      LineText = string1 + C + string2;
      Serial.print("Line ");
      Serial.println(LineText);
      line_notify(LineText);
    }

    //เซ็ตเวลาสําหรับแจ้งเตือนอุณหภูมิ

    if (hour == 7 && minute == 0)
    {

      String string20 = " และ ";

      String string21 = " ph";

      String string3 = "เซ็นเซอร์วัดอุณหภูมิและเซ็นเซอร์วัดค่า PH ตัวที่ 1 ณ เวลา 07.00   = ";

      String string4 = " C";

      String LineText1 = string3 + C + string4 + string20 + ph_act + string21;

      line_notify(LineText1);
    }

    if (hour == 12 && minute == 0)
    {

      String string22 = " และ ";

      String string23 = " ph";

      String string5 = "เซ็นเซอร์วัดอุณหภูมิและเซ็นเซอร์วัดค่า PH ตัวที่ 1 ณ เวลา 12.00   = ";

      String string6 = " C";

      String LineText2 = string5 + C + string6 + string22 + ph_act + string23;

      line_notify(LineText2);
    }

    if (hour == 18 && minute == 0)
    {

      String string24 = " และ ";

      String string25 = " ph";

      String string7 = "เซ็นเซอร์วัดอุณหภูมิและเซ็นเซอร์วัดค่า PH ตัวที่ 1 ณ เวลา 18.00   = ";

      String string8 = " C";

      String LineText3 = string7 + C + string8 + string24 + ph_act + string25;

      line_notify(LineText3);
    }
    last_line = last_time;
  }

  if (last_time - last_firebase >= 10000)
  {
    //สร้างข้อมูลให้ Firebase ในรูปแบบ JSON

    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &root_main = jsonBuffer.createObject();
    JsonObject &root = root_main.createNestedObject("Log1");

    JsonObject &humi = root.createNestedObject("Humidity");
    JsonObject &temper = root.createNestedObject("Temperature");
    JsonObject &phh = root.createNestedObject("PH value");

    humi["Data"] = F;
    temper["Data"] = C;
    phh["Data"] = ph_act;

    // root["analog"] = adc;

    // root["Celsius"] = C;

    // root["Fahrenheit"] = F;

    // root["time"] = currentDateTime;

    // 136

    // root["WIFI"] = WifiSignal;

    // root["RSSI"] = CurrentWiFiSignal;

    //  ส่งข้อมูลไปเก็บใน Firebase /logDHT3
    // string name = Firebase.push("logDHT3", root_main);
    String jsonStr;
    root_main.printTo(jsonStr);
    init_http.Firebase_Set(FIREBASE_HOST, FIREBASE_AUTH, "logDHT1", jsonStr);
    Serial.print("JSON_OBJ:");
    root_main.printTo(Serial);

    //    Serial.print(" Time: ");
    //    Serial.print(currentDateTime);
    //    Serial.print("\t");
    //    Serial.print("pushed: /logDHT3/");
    // Serial.println(name);

    // DEBUG เพื่อตรวจสอบ Memory Leaks ESP.getFreeHeap
    //หากลดจนหมด หรือ เกินความจำเป็น = Code Memory Leaks
    Serial.print("ESP.getBootMode(); ");
    Serial.println(ESP.getBootMode());
    Serial.print("ESP.getSdkVersion(); ");
    Serial.println(ESP.getSdkVersion());
    Serial.print("ESP.getBootVersion(); ");
    Serial.println(ESP.getBootVersion());
    Serial.print("ESP.getChipId(); ");
    Serial.println(ESP.getChipId());
    Serial.print("ESP.getFlashChipSize(); ");
    Serial.println(ESP.getFlashChipSize());
    Serial.print("ESP.getFlashChipRealSize(); ");
    Serial.println(ESP.getFlashChipRealSize());
    Serial.print("ESP.getFlashChipSizeByChipId(); ");
    Serial.println(ESP.getFlashChipSizeByChipId());
    Serial.print("ESP.getFlashChipId(); ");
    Serial.println(ESP.getFlashChipId());
    Serial.print("ESP.getFreeHeap(); ");
    Serial.println(ESP.getFreeHeap());
    last_firebase = last_time;
  }
}
