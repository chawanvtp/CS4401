// song 0
const int note0[] = {262,294,330,349,392,440,494,523};
const int noteList0[] = {0,555,1,555,2,555,3,555,4,555,5,555,6,555,7,555};
const int delay0[] = {500,500};

// song 1
const int note1[] = {261,294,329,349,391,415,440,455,466,526,554,587,622,659,698,740,784,830,880};
const int noteList1[] = {6,6,6,3,9,6,3,9,6,555,13,13,13,14,9,5,3,9,6,555,18,6,6,18,17,16,15,14,15,555,7,12,11,10,9,8,9,555,3,5,3,6,9,6,9,13,555,18,6,6,18,17,16,15,14,15,555,7,12,11,10,9,8,9,555,3,5,3,9,6,3,9,6,555};
const int delay1[] = {500,500,500,350,150,500,350,150,650,500,500,500,500,350,150,500,350,150,650,500,500,300,150,500,325,175,125,125,125,250,325,250,500,325,175,125,125,250,350,250,500,350,125,500,375,125,650,500,500,300,150,500,325,175,125,125,250,325,250,500,325,175,125,125,250,350,250,500,375,125,500,375,125,650,650};

// dynamic VAR
int cusor[] = {0,0};
int playingSong = 0;
int maxNote = sizeof(noteList0)/2;
int maxDelay = sizeof(delay0)/2;
int counter,butCount = 0;

// Devices
const int buzzerPin = 9;
const int buttonPin = 2;

void setup() {
// put your setup code here, to run once:
pinMode(buttonPin,INPUT);
pinMode(buzzerPin,OUTPUT);
digitalWrite(buttonPin, HIGH);
Serial.begin(9600);
//Serial.println(sizeof(noteList1));
Serial.print("max NOTE : "); Serial.print(maxNote);
Serial.print(" - max DELAY : "); Serial.println(maxDelay);
}

void loop() {
  
// BUTTON - is pressed
  if(digitalRead(buttonPin)==LOW and butCount < 1){
  Serial.println("button PRESS !!");
  if(playingSong==0){
    playingSong = 1;
    maxNote = sizeof(noteList1)/2;
    maxDelay = sizeof(delay1)/2;
  }else{
    playingSong = 0;
    maxNote = sizeof(noteList0)/2;
    maxDelay = sizeof(delay0)/2;
  }
  cusor[0],cusor[1] = 0;
  Serial.println("Switch to song : ");
  Serial.println(playingSong);
  //delay(300);
  butCount = 2;
  }

  
 // PLAY - the playingSong   
    if(playingSong==0){
      if(noteList0[cusor[0]]>=555){
          delay(delay0[cusor[1]]);
        // delay(500);
          Serial.print("DELAY - "); Serial.println(delay0[cusor[1]]);
          cusor[0] += 1;
          //cusor[1] += 1;
          Serial.print("at IF cursors move to "); Serial.print(cusor[0]); Serial.print(" "); Serial.println(cusor[1]);
       
        }
       //beep(note0[noteList0[cusor[0]]],delay0[cusor[1]]);
       Serial.print("Note : "); Serial.println(noteList0[cusor[0]]);
       Serial.print("Delay : "); Serial.println(delay0[cusor[1]]);
       tone(buzzerPin, note0[noteList0[cusor[0]]], 500);

      }
      
      if(playingSong==1){
        if(noteList1[cusor[0]]>=555){
          delay(delay1[cusor[1]]);
          Serial.print("DELAY - "); Serial.print(delay1[cusor[1]]);
          cusor[0] += 1;
          cusor[1] += 1;
          Serial.print("at IF cursors move to "); Serial.print(cusor[0]); Serial.print(" "); Serial.println(cusor[1]);
        }
        Serial.print("Note : "); Serial.println(noteList0[cusor[0]]);
        Serial.print("Delay : "); Serial.println(delay0[cusor[1]]);
        beep(note1[noteList1[cusor[0]]],delay1[cusor[1]]);
       
        
      }

    // DELAY - switching song button
    if(butCount>=1){butCount--;}

    // MOVE - cursor
    cusor[0]+=1; cusor[1]+=1;
Serial.print("cursors move to "); Serial.print(cusor[0]); Serial.print(" "); Serial.println(cusor[1]);
        
    // RESET - if song end & etc..
    if(cusor[1]>= maxDelay){cusor[1] = 0;}
    if(cusor[0]>= maxNote){cusor[0]=0; cusor[1] = 0;}

  
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
  
    delay(duration);
 
  }else
  {
    
    delay(duration);
    
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
