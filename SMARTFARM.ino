#include <Blynk.h>

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <TridentTD_LineNotify.h>

WidgetLED ALED(V10);
WidgetLED BLED(V11);
WidgetLED SolLED(V12);
WidgetLED AutoLED(V13);

char auth[] = "oXY0KoHTbbIjduBMIhS0zmzCxrjGeVPN";
char ssid[] = "KKK";
char pass[] = "012345678";
#define ledPin26 26
#define ledPin23 23
int fertAVal;
int fertBMan;
int solVal;
int autoStatus;
int totalDate;
int startDate;
#define RXD2 16
#define TXD2 17
const int potPin = 32;
WidgetRTC rtc;
BlynkTimer timer;
int potValue;
float moistPercent;
int configMoist;
int configFerA;
int configFerB;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;

  BLYNK_CONNECTED()
  {
    rtc.begin();
  }
   BLYNK_WRITE(V5){
    fertAVal = param.asInt();
    Serial.print("FertAVal = ");
    Serial.println(fertAVal);


    Serial2.print("#FERTA=");
    Serial2.print(fertAVal);
    Serial2.println("\n");
    if(fertAVal!= 0)
    {
      ALED.on(); 
      LINE.notify("Fertilizer A is ON");
    }else
    {
      ALED.off();  
      LINE.notify("Fertilizer A is OFF");
    }
  }

  BLYNK_WRITE(V6)
  {
    fertBMan = param.asInt();

    Serial.println("FertBMan = ");
    Serial.println(fertBMan);
    Serial2.print("#FERTB=");
    Serial2.print(fertBMan);
    Serial2.println("\n");
    if(fertBMan!= 0)
    {
      BLED.on(); 
      LINE.notify("Fertilizer B is ON");
    }else
    {
      BLED.off();  
      LINE.notify("Fertilizer B is OFF");
    }
  }
  BLYNK_WRITE(V7)
  {
      solVal = param.asInt();
      Serial.print("SolVal =");
      Serial.println(solVal);

      Serial2.print("#SOL=");
      Serial2.print(solVal);
      Serial2.println("\n");

    if(solVal!= 0)
    {
      SolLED.on(); 
      LINE.notify("Solenoid Valve is ON");
    }else
    {
      SolLED.off();  
      LINE.notify("Solenoid Valve is OFF");
    }
  }

  BLYNK_WRITE(V8)
  {
      autoStatus = param.asInt();
      Serial.print("autoStatus = ");
      Serial.println(autoStatus);

      Serial2.print("#AUTO=");
      Serial2.print(autoStatus);
      Serial2.print("\n");

    if(autoStatus!= 0)
    {
      AutoLED.on(); 
      LINE.notify("Auto is ON");
      AutoLED.on();
    }else
    {
      SolLED.off();  
      LINE.notify("Auto is OFF");
      AutoLED.off();
    }
  }
  
  BLYNK_WRITE(V0)
  {
      totalDate = param.asInt();

      Serial.print("TotalDate = ");
      Serial.println(totalDate);

      Serial2.print("#totalDate=");
      Serial2.print(totalDate);
      Serial2.print("\n");
      LINE.notify("Total Date is change to");
      LINE.notify(totalDate);
      
  }
  BLYNK_WRITE(V1)
  {
    startDate = param.asInt();

    Serial.print("StartDate = ");
    Serial.println(startDate);

    Serial2.print("#startDate=");
    Serial2.print(startDate);
    Serial2.print("\n");
    LINE.notify("Start Date is change to");
    LINE.notify(startDate);
  }
  BLYNK_WRITE(V3)
  {
      configMoist = param.asInt();
  }
  BLYNK_WRITE(V17)
  {
    configFerA = param.asInt();  
  }

  int setHour;
  int setMin;
  BLYNK_WRITE(V24)
  {
    setHour = param.asInt();
  }
  BLYNK_WRITE(V25)
  {
    setMin = param.asInt();
  }
    
void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth, ssid, pass, IPAddress(147,182,177,185),8080);
  Serial2.begin(115200,SERIAL_8N1, RXD2,TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  Serial.println("Successful");
  Serial.begin(9600);
  pinMode(ledPin23, OUTPUT);
  pinMode(ledPin26, OUTPUT);
  LINE.setToken("vK1AmrrSb6dpcT62Sz16aI3xa2iuJ72tzan3G9TWWyD");
  setSyncInterval(1);
  startMillis = millis();
}


void loop() {
  Blynk.run();
  timer.run();
  potValue = analogRead(potPin);
  moistPercent= (potValue * 0.02417582417);
  Blynk.virtualWrite(V9, moistPercent);

  currentMillis = millis();
  if(currentMillis - startMillis > period)
  {
    String currentTime = String(hour()) + ":" + minute() + ":" +second();
    String currentDate = String(day())+" "+month()+" "+year();
    String currentDay = String(day());
    Serial.print("Current time: ");
    Serial.println(currentTime);
    Serial.println(currentDate);

    Blynk.virtualWrite(V16, currentTime);
    Blynk.virtualWrite(V15, currentDate);
    Blynk.virtualWrite(V22, currentDay);
  }

  Serial2.print("#Hour=");
  Serial2.print(hour());
  Serial2.println("\n");
  Serial2.print("Min=");
  Serial2.print(minute());
  Serial2.println("\n");
  Serial2.println("#Sec=");
  Serial2.print(second());
  Serial2.println("\n");
}
