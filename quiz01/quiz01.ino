// Devices
const int buttonPin = 2;
const int pinLed01 = 3;
const int pinLed02 = 4;
const int pinLed03 = 5;
int state = 0;
int lastState = 0;
int counter = -1;
void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(pinLed01, OUTPUT);
  pinMode(pinLed02, OUTPUT);
  pinMode(pinLed03, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  state = digitalRead(buttonPin);
  if(state != lastState){
    if(state == HIGH){
    
      counter++;
    }
    delay(50);
  }
  lastState = state;

  if(counter==0){
    digitalWrite(pinLed01, HIGH);
    digitalWrite(pinLed02, HIGH);
    digitalWrite(pinLed03, HIGH);
  }else if(counter==1){
    digitalWrite(pinLed01, LOW);
    digitalWrite(pinLed02, LOW);
    digitalWrite(pinLed03, LOW);
  }else if(counter==2){
    digitalWrite(pinLed01, HIGH);
  }else if(counter==3){
    digitalWrite(pinLed02, HIGH);
  }else{
    //digitalWrite(pinLed03, HIGH);
    counter = 0;
  }
  
}
