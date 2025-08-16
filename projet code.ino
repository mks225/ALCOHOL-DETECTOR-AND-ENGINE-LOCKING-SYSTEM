#include <SoftwareSerial.h> 
SoftwareSerial sim(8, 9); 
#include <TinyGPS++.h> 
#include <LiquidCrystal.h> 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
float lattitude,longitude; 
SoftwareSerial gpsSerial(0,1); 
TinyGPSPlus gps; 
int value;  
#define motor 10 
#define buzzer 13 
#define led 7 
String number = "+917992448952";  
int a; 
void setup()  
{  
pinMode(motor,OUTPUT); 
pinMode(buzzer,OUTPUT); 
pinMode(led,OUTPUT); 
a=300; 
Serial.begin(9600); 
lcd.begin(16, 2); 
lcd.setCursor(0,0); 
lcd.print("    WELCOME"); 
lcd.setCursor(0,1); 
lcd.print(" ALCOHOL SESING DEVICE"); 
sim.begin(9600); 
gpsSerial.begin(9600);  
delay(6000); 
  
  lcd.clear(); 
} 
 
void loop()  
{ 
value=analogRead(A0);  
lcd.setCursor(0,0);   
lcd.print("value of alcohol");   
lcd.setCursor(0,1);   
lcd.print(value);   
delay(100);   
 digitalWrite(motor, HIGH); 
digitalWrite(buzzer, LOW);   
digitalWrite(led, LOW);  
 
   if (value > a) 
   { SendMessage(); 
     
    } 
} 
void SendMessage() 
{ 
    digitalWrite(motor, LOW);  // Lock the engine 
    digitalWrite(buzzer, HIGH);  // Activate the buzzer 
 
    boolean newData = false; 
 
    // Try to get new GPS data 
    for (unsigned long start = millis(); millis() - start < 2000;) 
    { 
        while (gpsSerial.available() > 0) 
        { 
            if (gps.encode(gpsSerial.read())) 
            { 
                newData = true; 
            } 
 

        } 
    } 
 
    if (newData) 
    { 
        float lat = gps.location.lat(); 
        float lng = gps.location.lng(); 
 
        // Debugging output 
        Serial.print("Latitude= "); 
        Serial.print(lat, 6); 
        Serial.print(" Longitude= "); 
        Serial.println(lng, 6); 
 
        // Send SMS 
        sim.println("AT+CMGF=1"); 
        delay(200); 
        sim.println("AT+CMGS=\"" + number + "\"\r"); 
        delay(200); 
        sim.print("http://maps.google.com/maps?q=loc:"); 
        sim.print(lat, 6); 
        sim.print(","); 
        sim.print(lng, 6); 
        delay(100); 
        sim.println((char)26); 
        delay(200); 
        Serial.println("GPS Location SMS Sent Successfully."); 
 
        // Update LCD to indicate GPS message sent 
        lcd.clear(); 
        lcd.setCursor(0, 0); 
        lcd.print("Sending GPS..."); 
        lcd.setCursor(0, 1); 
        lcd.print("Loc Sent to No."); 
        delay(5000);  // Display this message for 5 seconds 
    } 
 
  
    else 
    { 
        // Handle GPS data acquisition failure 
        lcd.clear(); 
        lcd.setCursor(0, 0); 
        lcd.print("GPS Error!"); 
        lcd.setCursor(0, 1); 
        lcd.print("Retry Later..."); 
        delay(5000); 
    } 
 
    // Lock the engine and display high alcohol alert 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print(" High Alcohol! "); 
    lcd.setCursor(0, 1); 
    lcd.print("Engine Locked!"); 
    delay(5000);  // Keep this message on the screen 
 
    // Alert with LED and buzzer 
    for (int i = 0; i < 10; i++) // Blink 10 times (or adjust for longer/shorter duration) 
    { 
        digitalWrite(led, HIGH); 
        digitalWrite(buzzer, HIGH); 
        delay(500); 
        digitalWrite(led, LOW); 
        digitalWrite(buzzer, LOW); 
        delay(500); 
    } 
}