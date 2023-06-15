//NAMA    : INGGA PRATAMA 
//JURUSAN : TEKNIK ELEKTRO
//NIM     :19010422016

#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <NewPing.h>
#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TRIGGER_PIN2  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN2     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE2 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
const int relay = 5;
int buzzer = 4;
NewPing sensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sensor2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE2); // NewPing setup of pins and maximum distance.
int counter=0;
bool kondisi_relay = false;
int jarak;
int jarak2;

void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  pinMode(relay, OUTPUT);
  lcd.init();
  lcd.backlight();
  pinMode (buzzer, OUTPUT);
  lcd.setCursor(1,0);
  lcd.print(" ===Welcome=== ");
  lcd.setCursor(0,1);
  lcd.print("Enter fgr print");
}

void loop()                     // run over and over again
{
  jarak=sensor.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  jarak2=sensor2.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.  
 }

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(buzzer, LOW);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      if (jarak>0 && jarak <=20){
        delay(2000);
        digitalWrite(buzzer, HIGH);
        Serial.println ("buzzer hidup");
        lcd.setCursor(1,0);
        lcd.print(" Pencuri ");
        lcd.setCursor(1,1);
        lcd.print("bahaya....");
        }
     if (jarak2>0 && jarak2 <=20){
        delay(2000);
        digitalWrite(buzzer, HIGH);
        delay (300);
        Serial.println ("buzzer hidup");
        lcd.setCursor(1,0);
        lcd.print(" Pencuri ");
        lcd.setCursor(1,1);
        lcd.print("bahaya....");
        }
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    counter++;
    Serial.println(counter);
    if(counter==1){
    lcd.setCursor(1,0);
    lcd.print(" Try Again ");
    lcd.setCursor(1,1);
    lcd.print(" Two More Try ");
    return p;}
    if(counter==2){
    lcd.setCursor(1,0);
    lcd.print(" Try Again ");
    lcd.setCursor(1,1);
    lcd.print(" One More Try ");
    return p;  
    }
    if(counter==3){
    lcd.setCursor(1,0);
    lcd.print(" Not Owner ");
    lcd.setCursor(1,1);
    lcd.print(" ============ ");
//    return p;
    digitalWrite(buzzer, HIGH);}
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  cekrelay();

  
  return finger.fingerID;

}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
   return finger.fingerID;
}
void cekrelay(){
  if(kondisi_relay == false){
    digitalWrite(relay, LOW);
    kondisi_relay=true;
    lcd.setCursor(3,0);
    lcd.print("Succes");
    lcd.setCursor(2,1);
    lcd.print("Thank you"); 
  }
  else{
    digitalWrite(relay, HIGH);
    kondisi_relay=false;
  }
}
