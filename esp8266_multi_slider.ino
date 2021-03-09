//Origin : Rui Santos site Servo example
//Client is named "tolna"

#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// GPIO the servo is attached to
static const int servoPin = 13;

const char* ssid     = "Your SSID";
const char* password = "Password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(0);
int pos1 = 0;
int pos2 = 0, tpos=0;
byte sw_state;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  pinMode(2,OUTPUT);digitalWrite(2,1);
 
  delay(100);

 Serial.begin(115200); delay(100); for (byte i=0; i<255; i++) Serial.print("*");Serial.println("");//garbage out
  
  myservo.attach(servoPin);  // attaches the servo on the servoPin to the servo object

  // Connect to Wi-Fi network with SSID and password

  Serial.print("Connect ");
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

void loop(){
  WiFiClient tolna = server.available();   // Listen for incoming client

  if (tolna) {                           // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    digitalWrite(2,0); //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (tolna.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (tolna.available()) {             // if there's bytes to read from the client,
        char c = tolna.read();             // read a byte, then
          //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
        //===============================================================================  
          if (currentLine.length() == 0) {
            tpos=0; 
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            tolna.println("HTTP/1.1 200 OK");
            tolna.println("Content-type:text/html");
            tolna.println("Connection: close");
            tolna.println();

            // Display the HTML web page
            tolna.println("<!DOCTYPE html><html>");
            tolna.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            tolna.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons, change the background-color and font-size attributes to fit your preferences
            
            tolna.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            tolna.println(".slider { width: 300px; }</style>");
            
            tolna.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            tolna.println(".slider1 { width: 300px; }</style>");
            
            tolna.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
          
            // Web Page
            tolna.println("</head><body><h1>ESP8266 Tolna</h1>");  
            // Sliders
            tolna.println("<p>Left limit: <span id=\"servoPos\"></span></p>");          
            tolna.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            tolna.println("<br><br><br>");
            tolna.println("<p>Right limit: <span id=\"servoPos1\"></span></p>");  
            tolna.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value=\""+valueString+"\"/>");
            tolna.println("<br><br><br>");
            tolna.println("<p>Speed : <span id=\"servoPos2\"></span></p>");  
            tolna.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value=\""+valueString+"\"/>");
            
            tolna.println("<script>var slider = document.getElementById(\"servoSlider\");");
            tolna.println("var slider1 = document.getElementById(\"servoSlider1\");");
            tolna.println("var slider2 = document.getElementById(\"servoSlider2\");");
            
            tolna.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            tolna.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            tolna.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innerHTML = slider2.value;");
          
            tolna.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }"); 
            tolna.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            tolna.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            
            tolna.println("$.ajaxSetup({timeout:1000});");
            tolna.println(" function servo(pos) { $.get(\"/?value=\" +  10+pos + \"&\"); {Connection: close};}");
            tolna.println(" function servo1(pos) { $.get(\"/?value=\" + 20+pos + \"&\"); {Connection: close};}");
            tolna.println(" function servo2(pos) { $.get(\"/?value=\" + 30+pos + \"&\"); {Connection: close};}");
            tolna.println("</script>");
         
            tolna.println("</body></html>");         //body ends
            
            if(header.indexOf("GET /?value=")>=0 ) {
              //Serial.print(header);
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+3, pos2);
              String index = header.substring(pos1+1, pos1+2); 
              // do something more with 3 variables
              // myservo.write(valueString.toInt());
              static int slider[3];                         //array 3 values
              slider [index.toInt()-1]=valueString.toInt(); //this is the final result
              // print 3 variables
              Serial.print(" Received= "); Serial.print(header.substring(pos1+1, pos1+2));Serial.print("  "); Serial.print(valueString); Serial.println("  ");
              Serial.print(" Received so far "); Serial.print(slider[0]);Serial.print("  "); Serial.print(slider[1]); 
                             Serial.print("  "); Serial.print(slider[2]);Serial.println("  ");
            }       
            
            // The HTTP response ends with another blank line
              tolna.println();
            // Break out of the while loop
            break;
          }  //newline 
         //=====================================
          else { // newline,clear currentLine
            currentLine = "";
          }
           } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }  //available
    }  //connected
    // Clear the header variable
    header = "";
    // Close the connection
    tolna.stop();
    digitalWrite(2,1);//Serial.println("Client disconnected.");
  }
}
