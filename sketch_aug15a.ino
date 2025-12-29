#include <LiquidCrystal.h>
#include <Servo.h>

// LCD pin definitions
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Servo pin definition
const int servoPin = 9;
Servo parkingServo;

// IR sensor pins
const int incomingSensorPin = 8;
const int outgoingSensorPin = 7;

// Parking slots tracking
int availableSlots = 4;  // Start with all slots available

// Flags for sensor states
int flag1 = 0;
int flag2 = 0;

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.clear();
  
  // Display welcome messages
  lcd.setCursor(0, 0);
  lcd.print("ARDUINO");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("PARKING SYSTEM");
  delay(2000);
  lcd.clear();

  // Initialize the Servo
  parkingServo.attach(servoPin);
  parkingServo.write(100); // Set initial position of servo (closed)

  // Initialize IR sensor pins
  pinMode(incomingSensorPin, INPUT);
  pinMode(outgoingSensorPin, INPUT);

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the sensor values
  int incomingSensorState = digitalRead(incomingSensorPin);
  int outgoingSensorState = digitalRead(outgoingSensorPin);

  // Check if a car is at the entrance
  if (incomingSensorState == 0 && flag1 == 0) {
    if (availableSlots > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        parkingServo.write(0); // Open the gate
        availableSlots--; // Decrease slot count
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SORRY :(");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full");
      delay(3000);
      lcd.clear();
    }
  }

  // Check if a car is at the exit
  if (outgoingSensorState == 0 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      parkingServo.write(0); // Open the gate
      availableSlots++; // Increase slot count
    }
  }

  // Close the gate after a car passes
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    parkingServo.write(100); // Close the gate
    flag1 = 0;
    flag2 = 0;
  }

  // Display the number of available slots
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("Slots Left: ");
  lcd.print(availableSlots);

  delay(500);
} 
