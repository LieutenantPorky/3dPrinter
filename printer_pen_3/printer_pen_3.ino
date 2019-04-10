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


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
int xPos = 0;
int yPos = 0;

boolean newData = false;

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
  
  //calibrate();

}

void loop() {
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
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



void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part
    xPos = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    yPos = atoi(strtokIndx);     // same

}

//============

void showParsedData() {
    Serial.print("X: ");
    Serial.println(xPos);
    Serial.print("Y: ");
    Serial.println(yPos);
}
