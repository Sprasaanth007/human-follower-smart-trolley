#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>     //ESP Web Server Library to host a web page

#include <Servo.h>  //Servo motor for rotating the Ultrasonic sensor
Servo myservo;     


int Echo = D2;  
int Trig = D1; 

//motor controller pins        
int IN1 = D5;
int IN2 = D6;
int IN3 = D7;
int IN4 = D8;

int rightDistance = 0, leftDistance = 0;     //The distance of human to the right and left of the ultrasonic sensor

//Our HTML webpage contents in program memory
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Trolley Controller</title>
    <style>
        *{
            padding: 0;
            margin: 0;
        }
        body{
            font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;
            background-color: coral;
        }
        li{
            list-style: none;
        }
        a{
            text-decoration: none;
        }
        .container{
            display: flex;
            flex-direction: column;
            align-items: center;
            max-width: 80%;
            margin: auto;
        }
        h1{
            margin-bottom: 5rem;
        }
        .nav li{
            padding: 1rem 1rem;
            margin-top :4rem;
            margin-bottom: 4rem;
            background-color: aquamarine;
            text-align: center;
        }
        li:hover{
            font-weight: 900;
            font-size: large;
            background-color: chartreuse;
        }
    </style>
</head>

<body>
    <div class="container">
        <h1>Human Follower Smart Trolley</h1>
        <ul class="nav">
            <li><a href="Forward">FORWARD</a></li>
            <li><a href="Backward">BACKWARD</a></li>
            <li><a href="Left">LEFT</a></li>
            <li><a href="Right">RIGHT</a></li>
            <li><a href="Stop">STOP</a></li>
        </ul>
    </div>
</body>
</html>

)=====";

//Static IP address configuration
IPAddress staticIP(192, 168, 43, 90); //ESP static ip
IPAddress gateway(192, 168, 43, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS
 
//SSID and Password of your WiFi router
const char* ssid = "yourWiFissid";
const char* password = "yourWiFipassword";
 
//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void forward(){             //Function to move the trolley forward 
  Serial.println("Forward page");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
  server.send(200, "text/html", "Forward"); //Send ADC value only to client ajax request
}

void backward() {                             //Function to move the trolley backward 
  Serial.println("Back");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
  server.send(200, "text/html", "back); //Send ADC value only to client ajax request
}

void left() {                             //Function to move the trolley left
  Serial.println("Left page");
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
  server.send(200, "text/html", "Left"); //Send ADC value only to client ajax request
}

void right() {                             //Function to move the trolley right
  Serial.println("Right page");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
  server.send(200, "text/html", "Right"); //Send ADC value only to client ajax request
}

void stop() {
  Serial.println("Stop page");
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
  server.send(200, "text/html", "Stop"); //Send ADC value only to client ajax request
}

void setup(){
    Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  myservo.attach(3);  // attach servo on pin 3 to servo object   
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stop();                 //To initially stop the motors from rotating
 
  WiFi.disconnect();          //Prevent connecting to wifi based on previous configuration
  
  WiFi.hostname(deviceName);                                // DHCP Hostname (useful for finding device for static lease)
  WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.begin(ssid, password);
 
  WiFi.mode(WIFI_STA);                                  //WiFi mode station (connect to wifi router only
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());                     //IP address assigned to your ESP
 
  server.on("/", handleRoot);                       //Which routine to handle at root location. This is display page
  server.on("/Forward", forward);                  //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/Backward", backward);
  server.on("/Left", left);
  server.on("/Right", right);
  server.on("/Stop", stop);
 
  server.begin();                                 //Start server
  Serial.println("HTTP server started");
}

void loop(){
    myservo.write(60);                 //set servo position to right side
    delay(200); 
    rightDistance = Distance_test();

    myservo.write(120);               //set servo position to left side
    delay(200); 
    leftDistance = Distance_test();

        if((rightDistance > 70)&&(leftDistance > 70)){          //This is for the trolley to avoid going towards farther objects like a wall
      stop();
    }else if((rightDistance >= 20) && (leftDistance >= 20)) {     
      forward();
    }else if((rightDistance <= 10) && (leftDistance <= 10)) {     //This is for the trolley to stay at a minimum distance of 10cms away from us
        back();
        delay(100);
    }else if(rightDistance - 3 > leftDistance) {
        left();
        delay(100);
    }else if(rightDistance + 3 < leftDistance) {
        right();
        delay(100);
    }else{
      stop();
    }

    server.handleClient();          //Handle client requests
}
