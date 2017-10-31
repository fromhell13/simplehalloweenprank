
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 6;    //the digital pin connected to the PIR sensor's output
int backlight1 = 8;
int backlight2 = 3;
int eye1      = 4;
int eye2      = 5;                
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int counter = 0;


/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  
  pinMode(backlight1, OUTPUT);
  pinMode(backlight2, OUTPUT);
  pinMode(eye1, OUTPUT);
  pinMode(eye2, OUTPUT);
  delay(2000);
  
  Serial.begin(19200);
  Serial.println("\\:v 255");    //Set the volume, from 0 (minimum)-255 (maximum)
  delay(50);
  
  

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

////////////////////////////
//LOOP
void loop(){
  
      

     if(digitalRead(pirPin) == HIGH){
      //turn scream on
      Serial.println("\\:n");
      //Serial.println("\\:s");
      digitalWrite(eye1, HIGH);
      digitalWrite(eye2, HIGH);
      digitalWrite(backlight1, HIGH);
      digitalWrite(backlight2, HIGH);   
      delay(100);              
      digitalWrite(backlight1, LOW);  
      digitalWrite(backlight2, LOW); 
      delay(100);
      //Serial.print(counter);
      counter++;
      if (counter > 30) {
        digitalWrite(backlight1, HIGH);  
        digitalWrite(backlight2, HIGH);  
        delay(5000);
      }   
     
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
        // delay(5000);
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");  
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
      //Serial.println("\\HLW002.mp3");
      //Serial.println(counter);
      //counter =0;
      //delay(5000);
      digitalWrite(backlight1, LOW);
      digitalWrite(backlight2, LOW); 
      digitalWrite(eye1, LOW);
      digitalWrite(eye2, LOW);
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
  }
