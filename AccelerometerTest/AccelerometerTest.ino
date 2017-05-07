// Test the ADXL335 Accelerometer and map the values to MIDI

const int analogPinX = A0; 
const int analogPinY = A2;
const int analogPinZ = A4;  

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
 
}

void loop() {
  //analogReference(EXTERNAL);    //connect 3.3v to AREF
  // read the analog in value:
  int x = analogRead(analogPinX);            
  int velocityX = map(x, 0, 1023, 0, 127);
  // change the analog out value:
  delayMicroseconds(30); // from kyub                     
  //
  int y = analogRead(analogPinY);            
  int velocityY = map(y, 0, 1023, 0, 127);

  delayMicroseconds(30); // from kyub                     
  //
  int z = analogRead(analogPinZ);            
  int velocityZ = map(z, 0, 1023, 0, 127);

  // print the results to the serial monitor:
  Serial.print("x = " );                       
  Serial.print(x);      
  Serial.print("\t mapped x = " );                       
  Serial.print(velocityX); 
  
  Serial.print("\ty = " );                       
  Serial.print(y);      
  Serial.print("\t mapped y = " );                       
  Serial.print(velocityY); 
  
  Serial.print("\tz = " );                       
  Serial.print(z);      
  Serial.print("\t mapped z = " );                       
  Serial.println(velocityZ); 
  
  delay(200);                     
}


