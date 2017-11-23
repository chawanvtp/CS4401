// [ - Initial VARIABLES - ] 

// - LED Devices -
const int ledList[] = {0,2,4,12,13};  // List of led's PIN.
const int ledIndex = (sizeof(ledList)/2)-1; // Number of led's PIN.

// - BUTTON Devices -
const int butList[] = {0,9,10,11,6};  // List of button's PIN.
int buttonStageList[] = {0,0,0,0,0};  // Initialize button's stage.
const int butIndex = (sizeof(ledList)/2)-1; // Number of button's PIN.

// - Game's Components -
const int limitLv = 40; // TO - SET Number of Stages.
int answers[limitLv];  //  Answers List
int onLevel = 4;
int onAnswer = 0;
int stage = 0;
int buttonState[4];

//------------------------------------------------------------------------


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
   for(int i=1; i<=ledIndex; i++){
     pinMode(ledList[i], OUTPUT);
     Serial.print("Initialize LED's pin at: ");
     Serial.println(ledList[i]);
  }

   for(int i=1; i<=butIndex; i++){
     pinMode(butList[i], INPUT);
     Serial.print("Initialize BUTTON's pin at: ");
     Serial.println(butList[i]);
  }
  
  //onLevel = 4;
  stage = 1;
  onAnswer = 1;
  
  // Initialize Answers for Stages 1-4 .
  for(int i = 1;  i <= 4; i++){
    answers[i] = random(1,5);
  }

  // DISPLAY - All answers
  for(int i = 0;  i <= 4; i++){
    Serial.println(answers[i]);
  }
}

//-------------------------------------------------------------------------


void loop() {

  /** STAGE is used to perform a correct task for game's events.
   *  0 - xxx Stage
   *  1 - Dispaly Answer Stage
   *  2 - Playing Stage
   */
  switch(stage){
    case  0:
      //Serial.println("0000");
      break;
    case  1:
      delay(500);
      for(int i = 1;  i <=  onLevel;  i++){
        Serial.print("Next is : ");
        Serial.println(answers[i]);
        digitalWrite(ledList[answers[i]], HIGH);
        delay(500);
        digitalWrite(ledList[answers[i]], LOW);
        delay(500);
      }

      blinkAll(3);
     
      delay(500);
      stage = 2;
      break;
    case  2:
      buttonState[1] = digitalRead(butList[1]);
      buttonState[2] = digitalRead(butList[2]);
      buttonState[3] = digitalRead(butList[3]);
      buttonState[4] = digitalRead(butList[4]);
      Serial.print(onAnswer);
      Serial.print(" - Push : ");
      Serial.println(answers[onAnswer]);

      /**
       * IF - User push An answer correctly.
       */
      if( buttonState[answers[onAnswer]] == HIGH ){
        digitalWrite(ledList[answers[onAnswer]], HIGH);
        delay(200);
        digitalWrite(ledList[answers[onAnswer]], LOW);
        delay(200);
        digitalWrite(ledList[answers[onAnswer]], HIGH);
        delay(2);
        digitalWrite(ledList[answers[onAnswer]], LOW);
        Serial.println("Correct Answer");

        onAnswer += 1;

        /** 
         *  IF - All Answers are pushed correctly, 
         *  GO to NEXT Level. !!
         */
        if( onAnswer  >  onLevel ){
         
          blinkAll(3);
          onAnswer = 1;
          onLevel += 1;
          answers[onLevel] = random(1,5);
           Serial.print("!! Level Up to :=> ");
           Serial.print(onLevel);
           Serial.println("  !! ");
          stage = 1;
        }
        
      }
      break;
  }
}


// Blink All LED for -> I times.
void blinkAll(int i){
        for(int j = 1; j <= i; j++){
        digitalWrite(ledList[1], HIGH);
        digitalWrite(ledList[2], HIGH);
        digitalWrite(ledList[3], HIGH);
        digitalWrite(ledList[4], HIGH);
        delay(300);
        digitalWrite(ledList[1], LOW);
        digitalWrite(ledList[2], LOW);
        digitalWrite(ledList[3], LOW);
        digitalWrite(ledList[4], LOW);
        delay(300);
        }
        return;
}

