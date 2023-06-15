#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
int buzzer = 4;
#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TRIGGER_PIN2  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN2     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE2 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


NewPing sensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sensor2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE2); // NewPing setup of pins and maximum distance.
int counter;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  lcd.init();
  lcd.backlight();
  pinMode (buzzer, OUTPUT);
}

void loop() {
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  lcd.setCursor(0,0);
  lcd.print(" ===Welcome=== ");
  lcd.setCursor(0,1);
  lcd.print("Enter fgr print");
  int jarak=sensor.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  int jarak2=sensor2.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  if (jarak>0 && jarak <=20){
        digitalWrite(buzzer, HIGH);
        delay (300);
        Serial.println ("buzzer hidup");
        digitalWrite(buzzer, LOW);
        delay(300);
        Serial.println ("buzzer mati");
  }
   if (jarak2>0 && jarak2 <=20){
        digitalWrite(buzzer, HIGH);
        delay (300);
        Serial.println ("buzzer hidup");
        digitalWrite(buzzer, LOW);
        delay(300);
        Serial.println ("buzzer mati");
       }
 }
