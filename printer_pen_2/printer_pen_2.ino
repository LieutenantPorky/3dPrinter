#define stepPinX 13
#define directionPinX 12

#define stepPinY 11
#define directionPinY 10

#define stepPinZ 9
#define directionPinZ 8

#define endX 7
#define endY 6
#define endZ 5

// Save pin configurations in array for easy iteration
int stepPins[] = {
    stepPinX, stepPinY, stepPinZ
  };
int directionPins[] = {
    directionPinX, directionPinY, directionPinZ
  };
int ends[] = {
    endX, endY, endZ
  };

void setup() {
  // set pinmodes:
  for (int i = 0; i < 3; i++) {
    pinMode(stepPins[i], OUTPUT);
    pinMode(directionPins[i], OUTPUT);
    pinMode(ends[i], INPUT);
  }
  
  // Calibration sequence start message
  Serial.begin(9600);
  Serial.println("begin calibration in 2 seconds");
  delay(1000);
  Serial.println("1 second");
  delay(1000);
  Serial.println("calibration start");
  calibrate();

}

void loop() {
}

void calibrate(){
  //Calibrate steppers by returning to initial position
  
  int timer = millis(); // used for serial monitoring of sensors
  bool activate[] = {
    true, true, true
  };

  bool go = false;
  while (not go){
    // set go to true, and then and it with sensor output - if any of these are false, we want to keep looping
    go = true;
    for (int i = 0; i < 3; i++) {
      go = go and (digitalRead(ends[i]) ==  HIGH);      
      if (digitalRead(ends[i]) == LOW){
        activate[i] = true;     
      }
      else {
        activate[i] = false;
      }
    }

    // Now we want to actually move steppers that are marked for activation

    for (int i = 0; i < 3; i++) {
      digitalWrite(directionPins[i], HIGH);
      if (activate[i]) {
        digitalWrite(stepPins[i], HIGH);
      }
    }
    
    delay(5);
    
    for (int i = 0; i < 3; i++) {
      digitalWrite(stepPins[i], LOW);
    }


    if (millis() - timer > 100) {
      timer = millis();
      for (int i = 0; i < 3; i++) {
        Serial.print(digitalRead(ends[i]));
        Serial.print(":");
        Serial.print(activate[i]);
        Serial.print(", ");
      }
      Serial.println("");
    }
    
  }
  
}
