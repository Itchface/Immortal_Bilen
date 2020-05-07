#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>


//inställningar för motor
#define enA 10
#define in1 7
#define in2 8
#define button 11
#define RAD_TO_DEG 57.295779513082320876798154814105
int rotDirection = 0;
int pressed = false;

//deklarerar sensorer
VL53L1X left_90;
VL53L1X left_45;
VL53L1X right_45;
VL53L1X right_90;
VL53L1X sensor5;



//funktion som visar hur mkt den ska svänga åt vilket håll
float svang(float pV_45, float pH_45){
  //Vinkel från likriktad triangel, Svängvinkeln (hur mkt den ska avvika från mitten), vinkeln i servots "mått".
  float V,SV,SeV,x,y;

  //vinklarna tilldelas sina variaber + 60mm som är avståndet från sensorerna till mitten vid 90dgr 
  x=pH_45+60;
  y=pV_45+60;

  //en vinkel på en liksidig triangel räknas ut och omvandlas till dgr från rad
  V=atan2(y,x);
  V=V*RAD_TO_DEG;
  //Servot rör sig från 16-170 och rör sig från 0-180dgr, här multipliceras gradantalet V till Servots enheter
  SeV=(45-V)*0.856;
 
    //Om vinkeln blir mer än 45dgr är en närmre ena sidan än den andra och ska därför svänga åt ett visst håll
    //principen håller för båda sidorna men kommer antingen avvika positivt eller negativt från servots mitt.
    if ( V<45 ) {
      
      //90.74 är servots mitt i dess egna värdesystem, bilen ska antingen gå neg eller pos från mitten.
      SV=90.74-SeV;
  
    }
      //se kommentar från förra if-satsen
      else if( V>45 ) {
       
        SV=90.74+SeV;
      }
//Skicka tillbaka svängvinkeln, dvs vinkeln den ska avvika 
Serial.println(SV);
Serial.println("ksskskk");
return SV;
}



Servo myservo;  //Skapar ett servoobjekt som kontrollerar servot
int pos;


/* ---  S E T  U P ---  */
void setup()
{
  //deklarerar sensor pins
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //pinMode(6, OUTPUT);
  myservo.attach(9);  //kopplar servot till pin 9 på arduinon

  //deklarerar motor pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  //digitalWrite(6, LOW);
 

 // Initalisiert I2C
  delay(500);
  Wire.begin();
  Wire.beginTransmission(0x29);
  Serial.begin (9600);

  digitalWrite(2, HIGH);
  delay(150);
  Serial.println("00");
  left_90.init();
  Serial.println("01");
  delay(100);
  left_90.setAddress(0x31);
  Serial.println("02");

/*
//Tror att den första sensorn räknas som standard och behöver inte tilldelas en adress?
  digitalWrite(2, HIGH);
  delay(150);
  Serial.println("09");
  left_90.init();
  Serial.println("10");
  delay(100);
*/
// Detta ger sensorn kopplad till pin 3 (sensor2) adressen 0x33
  digitalWrite(3,HIGH);
  delay(150);
  left_45.init();
  Serial.println("01");
  delay(100);
  left_45.setAddress(0x33);
  Serial.println("02");

digitalWrite(4,HIGH);
  delay(150);
  right_45.init();
  Serial.println("03");
  delay(100);
  right_45.setAddress(0x35);
  Serial.println("04");

  digitalWrite(5,HIGH);
  delay(150);
  right_90.init();
  Serial.println("05");
  delay(100);
  right_90.setAddress(0x37);
  Serial.println("06");

/*
  digitalWrite(6,HIGH);
  delay(150);
  sensor5.init();
  Serial.println("07");
  delay(100);
  sensor5.setAddress(0x39);
  Serial.println("08");
  */
  

  left_90.setDistanceMode(VL53L1X::Short);
  left_90.setMeasurementTimingBudget(50000);
  left_90.startContinuous(50);
  left_90.setTimeout(100);

  left_45.setDistanceMode(VL53L1X::Short);
  left_45.setMeasurementTimingBudget(50000);
  left_45.startContinuous(50);
  left_45.setTimeout(100);

  right_45.setDistanceMode(VL53L1X::Short);
  right_45.setMeasurementTimingBudget(50000);
  right_45.startContinuous(50);
  right_45.setTimeout(100);
  
  right_90.setDistanceMode(VL53L1X::Short);
  right_90.setMeasurementTimingBudget(50000);
  right_90.startContinuous(50);
  right_90.setTimeout(100);

  /*
  sensor5.setDistanceMode(VL53L1X::short);
  sensor5.setMeasurementTimingBudget(50000);
  sensor5.startContinuous(50);
  sensor5.setTimeout(100);
  */
  
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

  myservo.write(93);
}

/* ---  M A I N  L O O P --- */
void loop()
{
   
 
int hast=150;
 //Motorfunktionen
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 2000, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin

 
  //Styrfunktionen
    float funkL;

    funkL=svang(left_45.read(),right_45.read());
    

  myservo.write(funkL);
  Serial.print("hsjkgsö");
  
    //Serial.println("vänstersväng");
  }
 
 // while(left_45.read()>220 && right_45.read()>220)
  //{
   // myservo.write(91);
    //Serial.println("Rakt fram!");
  //}

  
