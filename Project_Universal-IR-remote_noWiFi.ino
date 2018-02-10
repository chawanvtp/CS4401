
/*
 *  IR Setup Configurations.
 */
#include <IRremote.h>
long unsigned int remoteControl[] = { 16738455, 16724175, 16718055, 16743045, 16716015, 16726215, 16734885, 16728765, 16730805, 16732845, 16736925 };
int lastButtonState;
int state = 2;

int RECV_PIN = 7;
int BUTTON_PIN = 8;
int STATUS_PIN = 13;
int pointer = 0;
IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;
decode_results storedButton[9];


// Storage for the recorded code
int codeType = -1; // The type of code
//unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[9][RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state

// For using in communication
//unsigned long  storedCode[9];
//int storedCodeLen[9];


/*
 WiFiEsp example: WebServerLed
 
 A simple web server that lets you turn on and of an LED via a web page.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(18, 19); // RX, TX
#endif

char ssid[] = "cvtp";            // your network SSID (name)
char pass[] = "0818055882";        // your network password
int status = WL_IDLE_STATUS;

int ledStatus = LOW;

WiFiEspServer server(80);

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);   // initialize serial for debugging
  Serial1.begin(9600);    // initialize serial for ESP module
  /*
  WiFi.init(&Serial1);    // initialize ESP module

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
*/
  /*   ---------------------------------------------
   *    Initialize (IR)'s : Receiver , Sender.
   *  ---------------------------------------------
   */ 
   
  irrecv.enableIRIn(); // Start the receiver
  pinMode(BUTTON_PIN, INPUT);
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
}


void loop()
{
  //listenClient();
  listenIR();
}


void sendHttpResponse(WiFiEspClient client)
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  client.print(stateGet(state));
  client.print(" = MODE ===:: ");
  client.println("<br>");
  client.print(" === Pointer ===>  ");
  client.print(pointer);
  client.println("<br>");
  client.println("<br>");
  client.println("<br>");
  
  client.print("<button type\"button\" value=\"/R\"> <a href=\"/R\">Record</a> </button>&nbsp;&nbsp;&nbsp;");
  client.println("<button type\"button\" value=\"/S\"> <a href=\"/S\">Send</a> </button>&nbsp;&nbsp;&nbsp;<br><br>");
  client.print("<button type\"button\" value=\"/7\"> <a href=\"/7\">7</a> </button>&nbsp;&nbsp;&nbsp;");
  client.print("<button type\"button\" value=\"/8\"> <a href=\"/8\">8</a> </button>&nbsp;&nbsp;&nbsp;");
  client.println("<button type\"button\" value=\"/9\"> <a href=\"/9\">9</a> </button>&nbsp;&nbsp;&nbsp;<br><br>");
  client.print("<button type\"button\" value=\"/4\"> <a href=\"/4\">4</a> </button>&nbsp;&nbsp;&nbsp;");
  client.print("<button type\"button\" value=\"/5\"> <a href=\"/5\">5</a> </button>&nbsp;&nbsp;&nbsp;");
  client.println("<button type\"button\" value=\"/6\"> <a href=\"/6\">6</a> </button>&nbsp;&nbsp;&nbsp;<br><br>");
  client.print("<button type\"button\" value=\"/1\"> <a href=\"/1\">1</a> </button>&nbsp;&nbsp;&nbsp;");
  client.print("<button type\"button\" value=\"/2\"> <a href=\"/2\">2</a> </button>&nbsp;&nbsp;&nbsp;");
  client.println("<button type\"button\" value=\"/3\"> <a href=\"/3\">3</a> </button>&nbsp;&nbsp;&nbsp;<br><br>");
  client.print("<button type\"button\" value=\"/0\"> <a href=\"/0\">0</a> </button>&nbsp;&nbsp;&nbsp;");

  
  // The HTTP response ends with another blank line:
  client.println();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

String stateGet(int s){
  if(state==1){
    Serial.println("- - - - - - - - - - - - - - -");
    Serial.println("::: Sending MODE :::");
     Serial.println("- - - - - - - - - - - - - - -");
    return "Send";
  }else if(state==2){
     Serial.println("- - - - - - - - - - - - - - -");
    Serial.println("::: Receiving MODE :::");
     Serial.println("- - - - - - - - - - - - - - -");
    return "Receive";
  }

  
}


/*
 * TO - Receiving Clients "Request".
 */
void listenClient()
{
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println("******************************************");
    Serial.println("[O] - NEW Client connected. !! - ");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);
        
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (buf.endsWith("GET /R")) {
          webPush(10);
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (buf.endsWith("GET /S")) {
          webPush(11);
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }else if (buf.endsWith("GET /0")){
          webPush(0);
        }else if (buf.endsWith("GET /1")){
          webPush(1);
        }else if (buf.endsWith("GET /2")){
          webPush(2);
        }else if (buf.endsWith("GET /3")){
          webPush(3);
        }else if (buf.endsWith("GET /4")){
          webPush(4);
        }else if (buf.endsWith("GET /5")){
          webPush(5);
        }else if (buf.endsWith("GET /6")){
          webPush(6);
        }else if (buf.endsWith("GET /7")){
          webPush(7);
        }else if (buf.endsWith("GET /8")){
          webPush(8);
        }else if (buf.endsWith("GET /9")){
          webPush(9);
        }

        
      }
    }
    
    // close the connection
    client.stop();
    Serial.println("[X] - Client disconnected. !! - ");
    Serial.println("******************************************");
  }
}

/*
 *  TO - Receive IR signal.
 */
void listenIR(){
   int buttonState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("*******************************");
    if(state==1){
      Serial.println("Switch to --- Receiving ...");
      state = 2;
    }else{
      Serial.println("Switch to --- Sendig ...");
      digitalWrite(STATUS_PIN, LOW);
      state = 1;
    }
    Serial.println("*******************************");
    irrecv.enableIRIn(); // Re-enable receiver
  }

switch (state) {
  case 1:
      //irrecv.enableIRIn(); // Re-enable receiver
      if (irrecv.decode(&results)) {
          digitalWrite(STATUS_PIN, HIGH);
          if(check(&results)){
          Serial.println(" Code SENT !! ");
          //irsend.enableIROut(9);
          sendCode(1);
          delay(200);
          sendCode(0);
          }
          digitalWrite(STATUS_PIN, LOW);
          irrecv.resume();
          irrecv.enableIRIn(); // Re-enable receiver
      }
      //irrecv.resume();
      
      //delay(50);
    break;
  case 2:
      if (irrecv.decode(&results)) {
          digitalWrite(STATUS_PIN, HIGH);
          if(!check(&results)){
          storeCode(&results);
          }
         irrecv.resume(); // resume receiver
          //digitalWrite(STATUS_PIN, LOW);
      }
    break;
  //default:
    // statements
}
/**
  if (buttonState) {
    irrecv.enableIRIn(); // Re-enable receiver
    //Serial.println("Pressed, sending");
    
    digitalWrite(STATUS_PIN, HIGH);
    // if(check(&results)){
      sendCode(lastButtonState == buttonState);
      Serial.println("Code sent !! ");
   // }
    //Serial.println(lastButtonState);
    //sendCode(lastButtonState == buttonState);
    digitalWrite(STATUS_PIN, LOW);
    delay(50); // Wait a bit between retransmissions
  }
  else if (irrecv.decode(&results)) {
    digitalWrite(STATUS_PIN, HIGH);
    if(!check(&results)){
      storeCode(&results);
    }
    irrecv.resume(); // resume receiver
    digitalWrite(STATUS_PIN, LOW);
  }
  */
  
  lastButtonState = buttonState;
}



/*
 * TO - Stored IR Signal Code
 */

void storeCode(decode_results *results) {
  codeType = results->decode_type;
  //int count = results->rawlen;
  storedButton[pointer] = *results;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    codeLen = results->rawlen - 1;
    // To store raw codes:
    // Drop first value (gap)
    // Convert from ticks to microseconds
    // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[pointer][i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else {
        // Space
        rawCodes[pointer][i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(rawCodes[pointer][i - 1], DEC);
    }
    Serial.println("");
  }
  else {
    if (codeType == NEC) {
      Serial.print("Received NEC: ");
      if (results->value == REPEAT) {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    } 
    else if (codeType == SONY) {
      Serial.print("Received SONY: ");
    } 
    else if (codeType == PANASONIC) {
      Serial.print("Received PANASONIC: ");
    }
    else if (codeType == JVC) {
      Serial.print("Received JVC: ");
    }
    else if (codeType == RC5) {
      Serial.print("Received RC5: ");
    } 
    else if (codeType == RC6) {
      Serial.print("Received RC6: ");
    }else if (codeType == SAMSUNG) {
      Serial.print("Received SAMSUNG: "); 
    }
    else {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results->value, HEX);  
    //codeValue = results->value;
    //codeLen = results->bits;
    //storedCode[pointer] = codeValue;
    //storedCodeLen[pointer] = codeLen;
    //storedButton[pointer] = *results;
    //storedButton[pointer]->bits = results->bits;
  }
}


/*
 *  TO - Send IR Signal Code
 */
void sendCode(int repeat) {
  
  if (storedButton[pointer].decode_type == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, storedButton[pointer].bits);
      Serial.println(" === Sent NEC repeat ===> ");
    } 
    else {
      irsend.sendNEC(storedButton[pointer].value, storedButton[pointer].bits);
      Serial.print(" === Sent NEC ===> ");
      Serial.println(storedButton[pointer].value, HEX);
    }
    delay(400);
    irsend.sendRaw(storedButton[pointer].rawbuf, storedButton[pointer].rawlen-1, 38);
  } 
  else if (storedButton[pointer].decode_type == SONY) {
    irsend.sendSony(storedButton[pointer].value, storedButton[pointer].bits);
    Serial.print(" === Sent Sony ===> ");
    Serial.println(storedButton[pointer].value, HEX);
  } 
  else if (storedButton[pointer].decode_type == PANASONIC) {
    irsend.sendPanasonic(storedButton[pointer].value, storedButton[pointer].bits);
    Serial.print(" === Sent Panasonic ===> ");
    Serial.println(storedButton[pointer].value, HEX);
  }
  else if (storedButton[pointer].decode_type == JVC) {
    irsend.sendJVC(storedButton[pointer].value, storedButton[pointer].bits, false);
    Serial.print(" === Sent JVC ===> ");
    Serial.println(storedButton[pointer].value, HEX);
  }
  else if (storedButton[pointer].decode_type == RC5 || codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      toggle = 1 - toggle;
    }
    // Put the toggle bit into the code to send
    storedButton[pointer].value = storedButton[pointer].value & ~(1 << (codeLen - 1));
    storedButton[pointer].value = storedButton[pointer].value | (toggle << (codeLen - 1));
    if (storedButton[pointer].decode_type == RC5) {
      Serial.print(" === Sent RC5 ===> ");
      Serial.println(storedButton[pointer].value, HEX);
      irsend.sendRC5(storedButton[pointer].value, storedButton[pointer].bits);
    } 
    else {
      irsend.sendRC6(storedButton[pointer].value, storedButton[pointer].bits);
      Serial.print(" === Sent RC6 ===> ");
      Serial.println(storedButton[pointer].value, HEX);
    }
  }else if (storedButton[pointer].decode_type == SAMSUNG){
    irsend.sendSAMSUNG(storedButton[pointer].value, storedButton[pointer].bits); 
    Serial.println(" === Send SAMSUNG ===> ");
  }
  else if (storedButton[pointer].decode_type == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    //irsend.sendRaw(rawCodes[pointer], storedButton[pointer].bits, 38);
    irsend.sendRaw(storedButton[pointer].rawbuf, storedButton[pointer].rawlen-1, 38);
    Serial.println(" === Sent raw ===> ");
  }
  
    /*irsend.sendRaw(storedButton[pointer].rawbuf, storedButton[pointer].rawlen-1, 38);
    Serial.println(" === Sent raw ===> ");
 **/
  return;
}

/*
 * IR - Functions
 */
 
boolean check(decode_results *results){
  //Serial.println("Checking input ...");
 /* for(int i=0; i<=10; i++){
  Serial.println(remoteControl[i]);
  }
 */ 
  if(results->value==remoteControl[0]){
    pointer = 0;
    printRes();
    return true;
  }else if(results->value==remoteControl[1]){
    pointer = 1;
    printRes();
    return true;
  }else if(results->value==remoteControl[2]){
    pointer = 2;
    printRes();
    return true;
  }else if(results->value==remoteControl[3]){
    pointer = 3;
    printRes();
    return true;
  }else if(results->value==remoteControl[4]){
    pointer = 4;
    printRes();
    return true;
  }else if(results->value==remoteControl[5]){
    pointer = 5;
    printRes();
    return true;
  }else if(results->value==remoteControl[6]){
    pointer = 6;
    printRes();
    return true;
  }else if(results->value==remoteControl[7]){
    pointer = 7;
    printRes();
    return true;
  }else if(results->value==remoteControl[8]){
    pointer = 8;
    printRes();
    return true;
  }else if(results->value==remoteControl[9]){
    pointer = 9;
    printRes();
    return true;
  }else if(results->value==remoteControl[10]){
    if(state==1){
      Serial.println("Switch to --- Receiving ...");
      state = 2;
    }else{
      Serial.println("Switch to --- Sendig ...");
      digitalWrite(STATUS_PIN, LOW);
      state = 1;
    }
    return true;
  }
  

  return false;
}

void webPush(int target){

  switch(target){
    case 0:
      Serial.println("<= WebPush [0]");
      webPushAction(target);
      break;
    case 1:
      Serial.println("<= WebPush [1]");
      webPushAction(target);
    break;
    case 2:
      Serial.println("<= WebPush [2]");
      webPushAction(target);
    break;
    case 3:
      Serial.println("<= WebPush [3]");
      webPushAction(target);
    break;
    case 4:
      Serial.println("<= WebPush [4]");
      webPushAction(target);
    break;
    case 5:
      Serial.println("<= WebPush [5]");
      webPushAction(target);
    break;
    case 6:
      Serial.println("<= WebPush [6]");
      webPushAction(target);
    break;
    case 7:
      Serial.println("<= WebPush [7]");
      webPushAction(target);
    break;
    case 8:
      Serial.println("<= WebPush [8]");
      webPushAction(target);
    break;
    case 9:
      Serial.println("<= WebPush [9]");
      webPushAction(target);
    break;
    case 10:
      Serial.println("<= WebPush [10]");
      state = 2;
      Serial.println("- Switch to -> Receiving MODE ...");
    break;
    case 11:
      Serial.println("WebPush [11]");
      state = 1;
      Serial.println("- Switch to -> Sending MODE ...");
    break;
  }
  
}

void webPushAction(int target){
  pointer = target;
  Serial.print("=> Pointer =: ");
  Serial.println(pointer);
  delay(100);
  if(state==1){
    sendCode(0);
    Serial.print(" === Sending IR Code at index - ");
    Serial.println(target);
    delay(100);
    sendCode(1);
    delay(100);
    irrecv.resume();
    irrecv.enableIRIn(); // Re-enable receiver
  }

  
  
  return;
}

void printRes(){
    Serial.println("--------------------");
    //Serial.print("Pointer --:> ");
    //Serial.println(pointer);
    Serial.print("CODE at index [");
    Serial.print(pointer);
    Serial.print("] : ");
    Serial.print(storedButton[pointer].value,HEX);
    Serial.print(" - ");
    Serial.print(storedButton[pointer].bits,HEX);
    Serial.println(" bits. ");
    Serial.println("--------------------");
    return;
}

