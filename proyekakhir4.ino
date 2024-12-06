#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#include <Wire.h>
#include <Stepper.h>

Servo myservo;
int gerak = 0;
int steps = 0;
int button = 6;
int nilaibutton;
int nilaiphotodioda;
int photodioda = 0;
int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

#define STEPS 512 //steps change stepper
Stepper motor(STEPS, in1Pin, in2Pin, in3Pin, in4Pin);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  myservo.attach(8);
  pinMode(6,INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(1000);

  motor.setSpeed(20); //5,86 ms

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
}

void loop()                     // run over and over again
{
  getFingerprintID(); //perintah menggerakan servo ketika sidik jari sesuai
  nilaibutton = digitalRead(button); //perintah menggerakan servo ketika button ditekan
    if (nilaibutton == HIGH){
    for (gerak = 0; gerak <= 90; gerak += 1){
    myservo.write(gerak);
    delay(5);
    }
    delay(1000);
    for (gerak = 90; gerak >= 0; gerak -= 1){
    myservo.write(gerak);
    delay(5);
    }  
    } else {
      gerak = 0;
      myservo.write(gerak);
    }
    nilaiphotodioda = analogRead(photodioda);
    Serial.println(nilaiphotodioda);
    if (nilaiphotodioda >= 700){ // kondisi malam hari
    steps = 400;
    motor.step(steps);
    delay(8000);
    } else { // kondisi siang hari
      steps = 0;
      motor.step(steps);
      delay(1000);
    }  

  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
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
    if (finger.fingerID == 1){
      Serial.println("1");
    } else if (finger.fingerID == 2) {
      Serial.println("2");
    } else if (finger.fingerID == 3) {
      Serial.println("3");
    } 

    for (gerak = 0; gerak <= 90; gerak += 1){
      myservo.write(gerak);
      delay(5);
    }
    delay(1000);
    for (gerak = 90; gerak >= 0; gerak -= 1){
      myservo.write(gerak);
      delay(5);
    }

    //kondisi setelah satu detik balik ke kondisi awal
    delay(1000);
    Serial.println("place your finger");

    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
    } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    

    delay(1000);
    Serial.println("place your finger");

    return p;

  
  } else {
    Serial.println("unknown error");
    return p;
  }
  return finger.fingerID;
}


  


