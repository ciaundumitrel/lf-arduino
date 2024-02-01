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
const float Vmin = 0.0;  
const float Vmax = 6.0;  

const int analogInPin = A0;  
int sensorValue = 0; 

bool start = false;


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Set tracker sensor pins as input
  pinMode(leftTrackerPin, INPUT);
  pinMode(centerTrackerPin, INPUT);
  pinMode(rightTrackerPin, INPUT);

  // Serial communication for debugging
  Serial.println("Line Following Robot - Debug Output:");
}

void makeRobotRotate360() {
  // Set motor speeds for clockwise rotation

  // Set the motor speeds and start the motors
  analogWrite(motorPin1, 512);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 512);
  analogWrite(motorPin4, 0);

  delay(50); // Adjust this delay based on your application

  // Stop the motors
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void startRobot() {
  start = true;
}

void stopRobot() {
  start = false;
}

void loop() {


  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    // Read the client request
    String request = client.readStringUntil('\r');
    Serial.println(request);
  if (request.indexOf("/rotate") != -1) {
        Serial.println("Received request to rotate 360 degrees");
        // Make the robot rotate 360 degrees
        makeRobotRotate360();
      }

  if (request.indexOf("/start") != -1) {
        Serial.println("Received request to start");
        // Make the robot rotate 360 degrees
        startRobot();
      }
  if (request.indexOf("/stop") != -1) {
        Serial.println("Received request to stop");
        // Make the robot rotate 360 degrees
        stopRobot();
      }

    // Handle the request
    if (request.indexOf("/") != -1) {
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

  if ((leftValue == HIGH && centerValue == LOW && rightValue == LOW) || (leftValue == HIGH && centerValue == LOW && rightValue == LOW)) {
    Serial.println("Turning Left");

    analogWrite(motorPin1, curveSpeed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, 0);

    lastLeft = speed;
    lastRight = 0;
    lastCenter = 0;
} else if (leftValue == LOW && centerValue == HIGH && rightValue == LOW) {

    Serial.println("Going Straight");
    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, speed);
    
    lastLeft = 0;
    lastRight = 0;
    lastCenter = speed;

} else if ((leftValue == LOW && centerValue == LOW && rightValue == HIGH ) ||(leftValue == LOW && centerValue == HIGH && rightValue == HIGH ) ) {
    // Turn right
    Serial.println("Turning Right");
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin3, 0);
    analogWrite(motorPin4, curveSpeed);


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



  delay(1);  
  // Adjust this delay based on your application
  // sensorValue = analogRead(analogInPin);
  // float voltage = sensorValue * (Vmax - Vmin) / 4095.0 + Vmin;

  // Calculate battery percentage
  // float batteryPercentage = ((voltage - Vmin) / (Vmax - Vmin)) * 100.0;

}
