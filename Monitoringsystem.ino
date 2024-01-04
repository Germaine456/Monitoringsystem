// Define color sensor pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define switchpin 2
#define alarm 9
#define servo 10 

int batch= 0;
int redCount = 0;
int greenCount = 0;
int blueCount = 0;
int blackCount = 0;
int switchstate = 0;
int previousSwitchState = HIGH; // Variable to track the previous switch state
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red, green, and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(alarm, HIGH);
    delay(500); // Adjust this delay based on your preference for the beep duration
    digitalWrite(alarm, LOW);
    delay(500); // Adjust this delay based on your preference for the gap between beeps
  }
}

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(switchpin, INPUT_PULLUP);
  pinMode(alarm, OUTPUT);
  pinMode(servo, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  // Begins serial communication
  Serial.begin(9600);
}

void loop() {
  switchstate = digitalRead(switchpin);
  Serial.println(switchstate);
  if (switchstate != previousSwitchState) {
    delay(50);  // Debounce
    if (switchstate == LOW) {
      beep(1);
    } else {
      beep(2);
    }
    previousSwitchState = switchstate;
  }
  if (switchstate == LOW) {
    

    // Setting RED (R) filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
  
    // Reading the output frequency
    redFrequency = pulseIn(sensorOut, LOW);
    // Remapping the value of the RED (R) frequency from 0 to 255
    redColor = map(redFrequency, 366, 475, 255, 0);
  
    // Printing the RED (R) value
    Serial.print("R = ");
    Serial.print(redColor);
    delay(500);
  
    // Setting GREEN (G) filtered photodiodes to be read
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
  
    // Introducing a delay for proper separation of color readings
    delay(50);
  
    // Reading the output frequency
    greenFrequency = pulseIn(sensorOut, LOW);
    // Remapping the value of the GREEN (G) frequency from 0 to 255
    greenColor = map(greenFrequency, 399, 463, 255, 0);
  
    // Printing the GREEN (G) value  
    Serial.print(" G = ");
    Serial.print(greenColor);
    delay(500);
 
    // Setting BLUE (B) filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
  
    // Introducing a delay for proper separation of color readings
    delay(50);
  
    // Reading the output frequency
    blueFrequency = pulseIn(sensorOut, LOW);
    // Remapping the value of the BLUE (B) frequency from 0 to 255
    blueColor = map(blueFrequency, 188, 255, 255, 0);
  
    // Printing the BLUE (B) value 
    Serial.print(" B = ");
    Serial.print(blueColor);
    delay(500);

    // Checks the current detected color and prints
    // a message in the serial monitor
    if (redColor > 0 && redColor > blueColor && redColor > 150) {
      Serial.println(" - RED detected!");
      beep(1);
      analogWrite(servo, 210);
      delay(1000);
      analogWrite(servo, 90);
      delay(1000);
      
       redCount++;  // Increment the red count
      Serial.print("Number of defects = ");
      Serial.println(redCount);
    }
    
    
    if (blueColor > redColor && blueColor > greenColor) {
      Serial.println(" - BLUE detected!");
      blueCount++;  // Increment the blue count
      Serial.print("Number of good products = ");
      Serial.println(blueCount);
      delay(1000);
      
    }
    
    if (greenColor > redColor && greenColor > blueColor) {
    Serial.println(" - GREEN detected!");
    greenCount++;  // Increment the green count
      Serial.print("Number of toppled products = ");
      Serial.println(greenCount);
    delay(1000);
    }

  }
}

