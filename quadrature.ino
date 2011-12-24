/*
Quadrature
 Harry Johnson, 3/11/11
 
 Quick and debounced quadrature encoder reading. Put data lines on pins 2 and 3.
 Automatically and quickly handles the 4 types of transitions and appropriate invalid/repetitive states by use of multiplication.
 Based the transition state graph from http://www.arduino.cc/playground/Main/RotaryEncoders
 Needs to have 2 significant transitions happen in a row for the encoder to increment.
 The counter still increments on all 4 transitions, though, so there is no decrease in resolution.
 */

//Global Variables. 
volatile int pastA = 0; //last seen falling: -1. rising: 1. unchanged since previous: 0. 
volatile int pastB = 0;
volatile int A = 0; //rising, 1, falling, -1.
volatile int B = 0;
volatile int encoderCount = 0; //cycles * 4. Handles all 4 transition states.
int oldEncoderCount = -1;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if(oldEncoderCount != encoderCount) {
    oldEncoderCount = encoderCount; 
    Serial.println(oldEncoderCount); //print to serial when the encoder value changes.
  }
  delay(10);

}

void doEncoderA() { //edge detected on A.
  A = (digitalRead(2) == HIGH) ? 1 : -1; //This code is a switch. If the pin is high (rising edge), A = 1. If low, (falling edge), A = -1. 
  pastA = A; //A only becomes "past" and useful when an edge is detected on B. 
  encoderCount =encoderCount + (A * pastB); // A rising, B rising, increment. A rising, B falling, decrement. A falling, B rising, decrement. A falling, B falling, increment.
  //See transition graph to see how this works.
  pastB = 0; //If we trigger back to here again, (bouncing back and forth on an A edge, we don't want the encoder incrementing each time. 
}

void doEncoderB() { //edge detected on B.
  B = (digitalRead(3) == HIGH) ? 1 : -1; //see above.
  pastB = B;
  encoderCount = encoderCount + (B * -pastA); //Again, see transition graph. 
  // A rising, B rising, decrement. A rising, B falling, increment. A falling, B rising, increment. A falling, B falling, decrement.
  pastA = 0;//avoid multiple counts.
}




