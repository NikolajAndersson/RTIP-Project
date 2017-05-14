// Test the ADXL335 Accelerometer and map the values to MIDI

const int analogPinX = A5; 
const int analogPinY = A6;
const int analogPinZ = A7;  

const int pot1 = A8;  
const int pot2 = A9;  


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

  int p1 = analogRead(pot1);            
  int p1_mapped = map(p1, 1023, 0, 0, 127);
  // change the analog out value:
  delayMicroseconds(30);       

  int p2 = analogRead(pot2);            
  int p2_mapped = map(p2, 1023, 0, 0, 127);
  // change the analog out value:
  delayMicroseconds(30);       
  
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
  Serial.print(velocityZ); 

  Serial.print("\tpot1 = " );                       
  Serial.print(p1);      
  Serial.print("\t mapped pot1 = " );                       
  Serial.print(p1_mapped); 
  
  Serial.print("\tpot2 = " );                       
  Serial.print(p2);      
  Serial.print("\t mapped pot1 = " );                       
  Serial.println(p2_mapped); 
  delay(20);                     
}


