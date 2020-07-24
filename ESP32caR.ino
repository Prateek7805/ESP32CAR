#include<WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiServer server(80);
String header;
// the pins for the 74hc595
int dataPin=27;
int clkPin=26;
int latchPin=25; 

int en[4]={13,4,33,32};// enable pins 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel[4] ={0,1,2,3};
const int resolution = 8;

const int forward=0x66;
const int reverse=0x99;
const int right=0xAA;
const int left=0x55;


int dutyCycle = 170;

void setup() {
  // sets the pins as outputs:
  pinMode(dataPin,OUTPUT);
  pinMode(clkPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  for(int i=0; i<4; i++)
    pinMode(en[i], OUTPUT);

  shiftOut(dataPin,clkPin,MSBFIRST,0);
  latchIt();
  
  // configure LED PWM functionalitites
  for(int i=0; i<4; i++)
    ledcSetup(pwmChannel[i], freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  for(int i=0;i<4; i++)
    ledcAttachPin(en[i], pwmChannel[i]);

  Serial.begin(115200);
 
  delay(100);  

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  
}

void loop() {
  for(int i=0;i<4;i++)
    ledcWrite(pwmChannel[i], dutyCycle);
   WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (! client.available())
  {
    delay (1);
  }
  
 delay(10);
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  // Match the request
  
  if (req.indexOf("/fi") != -1)  {
    moveCar(forward);
  }
  else if(req.indexOf("/b") != -1)
  {
   moveCar(reverse);
  }
   else if(req.indexOf("/r") != -1)
  {
    moveCar(left);
  } else if(req.indexOf("/l") != -1)
  {
   moveCar(right);
  }
  else if(req.indexOf("/s") != -1){
    moveCar(0);
  }

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");
client.println(""); 

client.println("<!DOCTYPE HTML>");
client.println("<HTML>");
client.println("<H1> Car Control </H1>");
client.println("<br />");
client.println("<body>");
 client.println("<a href=\"/fi\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> forward </button> </a>");
 client.println("<a href=\"/b\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> reverse </button> </a>");
 client.println("<a href=\"/l\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> left </button> </a>");
  client.println("<a href=\"/r\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> right </button> </a>");
 client.println("<a href=\"/s\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> stop </button> </a>");
 client.println("</body></html>");
 
  delay(10);
}

void latchIt(){
digitalWrite(latchPin,HIGH);
delayMicroseconds(10);
digitalWrite(latchPin,LOW);
delayMicroseconds(10);
}

void moveCar(int num){
shiftOut(dataPin,clkPin,MSBFIRST,num);
latchIt();
}
