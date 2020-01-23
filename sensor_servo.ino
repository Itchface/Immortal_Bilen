#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>


VL53L1X sensor;
VL53L1X sensor2;


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; 

void setup()
{
  myservo.attach(9);
  pinMode(2, OUTPUT);  //erster Sensor muss nicht über XSHUT angesteuert werden
  pinMode(3, OUTPUT);
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
 

 // Initalisiert I2C
  delay(500);
  Wire.begin();
  Wire.beginTransmission(0x29);
  Serial.begin (9600);
/*
  digitalWrite(2, HIGH);
  delay(150);
  Serial.println("00");
  sensor.init();
  Serial.println("01");
  delay(100);
  sensor.setAddress(0x31);
  Serial.println("02");
*/
  digitalWrite(3,HIGH);
  delay(150);
  sensor2.init();
  Serial.println("01");
  delay(100);
  sensor2.setAddress(0x33);
  Serial.println("02");

  digitalWrite(2, HIGH);
  delay(150);
  Serial.println("09");
  sensor.init();
  Serial.println("10");
  delay(100);


  

  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);
  sensor.setTimeout(100);

  sensor2.setDistanceMode(VL53L1X::Short);
  sensor2.setMeasurementTimingBudget(50000);
  sensor2.startContinuous(50);
  sensor2.setTimeout(100);
  
  delay(150);
  Serial.println("addresses set");

  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;


  for (byte i = 1; i < 120; i++)
  {

    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");


}

void loop()
{

  Serial.print("Sensor1_horizontal: ");
  Serial.print(sensor.read());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.print("  -   ");
  
  Serial.print("Sensor2_22.5: ");
  Serial.print(sensor2.read());
  //if (sensor2.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
  
 // delay(500);

  Serial.print("Servo:");
  Serial.println(myservo.read());
Serial.println();
  while(sensor.read()<100 && sensor2.read()>100)
  {
    myservo.write(180);
    Serial.println("högersväng");
  }
  while(sensor.read()>100 && sensor2.read()<100)
  {
    myservo.write(0);
    Serial.println("vänstersväng");
  }
  while(sensor.read()>100 && sensor2.read()>100)
  {
    myservo.write(90);
    Serial.println("Rakt fram!");
  }
}
