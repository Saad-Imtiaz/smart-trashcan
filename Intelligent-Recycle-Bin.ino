

#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros


String twitterAPIKey = "5E8PUPRLFWA74YUP";
String twitter = "hskeek1";
//int writeFields(unsigned long channelNumber, const char * writeAPIKey)
unsigned long channelNumber = 1291602;
const char *  writeAPIKey = "5E8PUPRLFWA74YUP";

char ssid[] = SECRET_SSID;    //  your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
int Trig = 8; // Trig connected to pin 8
const int Echo = 9; // Echo connected to pin 9
// Variables for the ultrasonic sensor
long duration, cm;
long sendVal;
// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 13;     // the number of the Dip Switch 1
const int buttonPin2 = 12;     // the number of the Dip Switch 2
const int buttonPin3 = 11;     // the number of the Dip Switch 3
const int buttonPin4 = 10;     // the number of the Dip Switch 4
// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int binNum = 0;
const int red = 7; // Red LED is on Pin 7
const int green = 6; // Green LED is on Pin 6
String notification = "";
void setup() {
  Serial.begin(115200); //Initialize serial
  // Defining the input and outputs for the pins of arduino
  pinMode(Trig, OUTPUT); // UNO's output, ranger's input
  pinMode(Echo, INPUT); // UNO's input, ranger's output
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }  
  
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
 // read the state of the Dip Switch value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  long duration, cm;
  // Getting a reading from the ultrasonic sensor
  // 1. produce a 5us HIGH pulse in Trig
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig, LOW);

  // 2. measure the duration of the HIGH pulse in Echo
  //      & every 58 us is an obstacle distance of 1 cm
  duration = pulseIn(Echo, HIGH);
  cm = duration / 58;
  Serial.print(cm);
  Serial.println(F("cm"));


  // check if the Dip Switch is High. If it is, the buttonState is HIGH:
  // Setting the notification for each dip switch
  if (buttonState1 == HIGH) {
    // turn LED on:
    Serial.println(F("1 on"));
    binNum = 1;
    notification = "The Bin Number 1 is Full" ;
  }

  if (buttonState2 == HIGH) {
    binNum = 2;
    // turn LED on:
    Serial.println(F("2 on"));
    notification = "The Bin Number 2 is Full" ;
  }

  if (buttonState3 == HIGH) {
    binNum = 3;
    // turn LED on:
    Serial.println(F("3 on"));
    notification = "The Bin Number 3 is Full" ;
  }
  if (buttonState4 == HIGH) {
    binNum = 4;
    // turn LED on:
    Serial.println(F("4 on"));
    notification = "The Bin Number 4 is Full" ;
  }

// Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  if (cm <= 20) {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    Serial.println(notification);

ThingSpeak.setStatus(notification);
ThingSpeak.setTwitterTweet(twitter, notification);
ThingSpeak.writeFields(channelNumber,writeAPIKey);
ThingSpeak.setTwitterTweet("hskeek1",notification);
ThingSpeak.writeFields(channelNumber,writeAPIKey);
  }
    /*if(yy == 200){
    Serial.println("Twitter update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(yy));
  }
    delay (30000);

  }
  
  else {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
  }
  */
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, cm, myWriteAPIKey); 
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  
  delay(20000); // Wait 20 seconds to update the channel again
}

// This function attempts to set the ESP01 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial1.begin(baudrate);
}
