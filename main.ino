#include <ESP8266WiFi.h>

const char* ssid = "Dumi ";
const char* password = "parola123";
WiFiServer server(80); 

int speed = 180;
int curveSpeed = 360;

int motorPin1 = D1;   
int motorPin2 = D2;    

int leftTrackerPin = D5;  
int centerTrackerPin = D6;  
int rightTrackerPin = D7;  

int motorPin3 = D3;
int motorPin4 = D4;

int red = D0;
int blue = D8;

// const float Vmin = 0.0;  
// const float Vmax = 6.0;  

// const int analogInPin = A0;  
// int sensorValue = 0; 

bool start = false;


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.begin();


  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  // Set tracker sensor pins as input
  pinMode(leftTrackerPin, INPUT);
  pinMode(centerTrackerPin, INPUT);
  pinMode(rightTrackerPin, INPUT);

}

void makeRobotRotate360() {
  analogWrite(motorPin1, speed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, speed);
  analogWrite(motorPin4, 0);

  delay(5000); // Adjust this delay based on your application

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}


void moveForward() {
    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, speed);
    delay(1000);
    analogWrite(motorPin1, 0);
    analogWrite(motorPin4, 0);
}

void moveBackward() {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, speed);
    analogWrite(motorPin3, speed);
    analogWrite(motorPin4, 0);

   delay(1000);

   analogWrite(motorPin2, 0);
   analogWrite(motorPin3, 0);
   }

void moveLeft() {
    analogWrite(motorPin1, curveSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, 0);

    delay(1000);
   
    analogWrite(motorPin1, 0);
}

void moveRight() {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, curveSpeed);
    delay(1000);
    analogWrite(motorPin4, 0);

}

void startRobot() {
  start = true;
}

void stopRobot() {
  start = false;
}


void loop() {
  digitalWrite(blue, HIGH);
  digitalWrite(red, HIGH);

  WiFiClient client = server.available();
  if (client) {
    // Read the client request
    String request = client.readStringUntil('\r');
    Serial.println(request);
  if (request.indexOf("/rotate") != -1) {
        Serial.println("Received request to rotate 360 degrees");
        // Make the robot rotate 360 degrees
        makeRobotRotate360();
      }

  else if (request.indexOf("/start") != -1) {
    Serial.println("Received request to start");
        // Make the robot rotate 360 degrees
        startRobot();
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("OK");  
        }
    else if (request.indexOf("/forward") != -1) {
      Serial.println("Received request to move forward");
      moveForward();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Moving Forward");
    } 
    else if (request.indexOf("/backward") != -1) {
      Serial.println("Received request to move backward");
      moveBackward();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Moving Backward");
    } 
    else if (request.indexOf("/left") != -1) {
      Serial.println("Received request to move left");
      moveLeft();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Moving Left");
    }
     else if (request.indexOf("/right") != -1) {
      Serial.println("Received request to move right");
      moveRight();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Moving Right");
    }
    else if (request.indexOf("/stop") != -1) {
        Serial.println("Received request to stop");
        // Make the robot rotate 360 degrees
        stopRobot();
      }
  
    else if (request.indexOf("/") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>ESP8266 Web Server</title>");
      client.println("</head>");
      client.println("<body>");
      client.println("<h1>Welcome to ESP8266 Web Server</h1>");
      client.println("<p>This is a simple web server hosted on ESP8266</p>");
      client.println("</body>");
      client.println("</html>");
    }
  }


  int lastLeft = 0;
  int lastCenter = 0;
  int lastRight = 0;

  int leftValue = digitalRead(leftTrackerPin);
  int centerValue = digitalRead(centerTrackerPin);
  int rightValue = digitalRead(rightTrackerPin);
  if (start == true){

  if ((leftValue == HIGH && centerValue == LOW && rightValue == LOW) ||(leftValue == HIGH && centerValue == HIGH && rightValue == LOW) ) {

    analogWrite(motorPin1, curveSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, 0);

    lastLeft = speed;
    lastRight = 0;
    lastCenter = 0;
} else if (leftValue == LOW && centerValue == HIGH && rightValue == LOW) {

    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, speed);
    
    lastLeft = 0;
    lastRight = 0;
    lastCenter = speed;

} else if ((leftValue == LOW && centerValue == LOW && rightValue == HIGH ) ||(leftValue == LOW && centerValue == HIGH && rightValue == HIGH ) ) {
    // Turn right
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, 512);


    lastLeft = 0;
    lastRight = speed;
    lastCenter = 0;

} else if (leftValue == LOW && centerValue == LOW && rightValue == LOW) {
    // Stop
    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, speed);

} else {
    // Stop
    digitalWrite(motorPin1, 0);
    digitalWrite(motorPin2, 0);
    digitalWrite(motorPin3, 0);
    digitalWrite(motorPin4, 0);
}
}
else{
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, 0);
}



  delay(5);  
  // Adjust this delay based on your application
  // sensorValue = analogRead(analogInPin);
  // float voltage = sensorValue * (Vmax - Vmin) / 4095.0 + Vmin;

  // Calculate battery percentage
  // float batteryPercentage = ((voltage - Vmin) / (Vmax - Vmin)) * 100.0;

}
