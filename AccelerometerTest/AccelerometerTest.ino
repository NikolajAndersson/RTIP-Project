// Test the ADXL335 Accelerometer and map the values to MIDI

const int analogPinX = A5;
const int analogPinY = A6;
const int analogPinZ = A7;

const int pot1 = A8;
const int pot2 = A9;

bool firstrun = false;
float accScale = 10;
int zeroX, zeroY, zeroZ;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

}

void loop() {
  //analogReference(EXTERNAL);    //connect 3.3v to AREF
  // read the analog in value:
  if (!firstrun) {
    firstrun = true;
    zeroX = analogRead(analogPinX);
    // change the analog out value:
    delayMicroseconds(30); // from kyub
    //
    zeroY = analogRead(analogPinY);

    delayMicroseconds(30); // from kyub
    //
    zeroZ = 550; // from test
  }
  int x = analogRead(analogPinX);
  float accX = (x - zeroX);
  int midiX = map(abs(accX), 0, 1023 - zeroZ, 0, 127);

  // change the analog out value:
  delayMicroseconds(30);
  //
  int y = analogRead(analogPinY);
  float accY = (y - zeroY);
  int midiY = map(abs(accY), 0, 1023 - zeroY, 0, 127);

  delayMicroseconds(30); // from kyub
  //
  int z = analogRead(analogPinZ);
  float accZ = (z - zeroZ);
  int midiZ = map(abs(accZ), 0, 1023 - zeroZ, 0, 127);

  int p1 = analogRead(pot1);
  int p1_mapped = map(p1, 1023, 0, 0, 127);
  // change the analog out value:
  delayMicroseconds(30);

  int p2 = analogRead(pot2);
  int p2_mapped = map(p2, 1023, 0, 0, 127);
  // change the analog out value:
  delayMicroseconds(30);

  // print the results to the serial monitor:
  //  Serial.print("x = " );
  //  Serial.print(x);
  //  Serial.print("\t mapped x = " );
  //  Serial.print(accX);
  //  Serial.print("\t midi mapped x = " );
  //  Serial.print(midiX);
  //  Serial.print("\ty = " );
  //  Serial.print(y);
  //  Serial.print("\t mapped y = " );
  //  Serial.print(accY);
  //  Serial.print("\t midi mapped x = " );
  //  Serial.print(midiY);
  //  Serial.print("\tz = " );
  //  Serial.print(z);
  //  Serial.print("\t mapped z = " );
  //  Serial.print(accZ);
  //  Serial.print("\t midi mapped z = " );
  //  Serial.println(midiZ);

  //  Serial.print("\tpot1 = " );
  //  Serial.print(p1);
  //  Serial.print("\t mapped pot1 = " );
  //  Serial.print(p1_mapped);
  //
  //  Serial.print("\tpot2 = " );
  //  Serial.print(p2);
  //  Serial.print("\t mapped pot1 = " );
  //  Serial.println(p2_mapped);
  //accScale = p2;
  float xAcc = (x - zeroX) / accScale;
  float yAcc = (y - zeroY) / accScale;
  float zAcc = (z - zeroZ) / accScale;
  Serial.print("scale = " );
  Serial.print(accScale);
  Serial.print("x = " );
  Serial.print(xAcc);
  Serial.print("\ty = " );
  Serial.print(yAcc);
  Serial.print("\tz = " );
  Serial.print(zAcc);

  // apply trigonometry to get the pitch and roll:
  float pitch = atan(xAcc / sqrt(pow(yAcc, 2) + pow(zAcc, 2)));
  float roll = atan(yAcc / sqrt(pow(xAcc, 2) + pow(zAcc, 2)));
  //convert radians into degrees
  pitch = pitch * (180.0 / PI);
  roll = roll * (180.0 / PI) ;
  Serial.print("\tpitch = " );
  Serial.print(pitch);
  Serial.print("\troll = " );
  Serial.println(roll);
  delay(20);
}

