#include <CapacitiveSensor.h>
// Analog pins
const int analogPinX = A5;
const int analogPinY = A6;
const int analogPinZ = A7;

const int pot1 = A8;
const int pot2 = A9;

int potPrevious1 = 0;
int potPrevious2 = 0;
// Capacitive sensors
const int sensorAmount = 12;
bool noteState[sensorAmount];
// Buffers
const int sensorBufferAmount = 20;
long sensorBuffer[sensorAmount][sensorBufferAmount];
long lastMeasure[sensorAmount];
long dynamicOffset[sensorAmount];
unsigned long timer[sensorAmount];
long meanThreshold[sensorAmount];
int buf[sensorAmount];
int accBuffer[3][sensorBufferAmount];
int accCounter = 0;
unsigned long sendCCTimer = 0;

// Capacitive sensor pins
#define CS(Y) CapacitiveSensor(14, Y) // driver pin 14
// pins: 0 1 2 3 - 5 6 7 8 - 9 10 11 12
CapacitiveSensor cs[] = {CS(0), CS(1), CS(2), CS(3), CS(5), CS(6), CS(7), CS(8), CS(9), CS(10), CS(11), CS(12)}; // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

long minithreshold = 300;
long off = 400;
int turn = 0;
// MIDI notes and MIDI channel
int scale[] = {69, 60, 65, 62, 61, 67, 64, 71, 70, 66, 74, 72};
int channel = 1;

bool firstrun = false;
float accScale = 3;
int zeroX, zeroY, zeroZ;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < sensorAmount; i++) {
    cs[i].set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
    noteState[i] = 0;
    timer[i] = 0;
    lastMeasure[i] = 0;
    buf[i] = 0;
    meanThreshold[i] = 0;
    dynamicOffset[i] = minithreshold;
    for (int j = 0; j < sensorBufferAmount; j++) sensorBuffer[i][j] = 0;
  }
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < sensorBufferAmount; j++) accBuffer[i][j] = 0;
  }
}

void loop() {
    if (!firstrun) {
    firstrun = true;
    zeroX = analogRead(analogPinX);
    // change the analog out value:
    delayMicroseconds(30); // from kyub
    //
    zeroY = analogRead(analogPinY);

    delayMicroseconds(30); // from kyub
    //
    zeroZ = 507; // from test
  }

  long measure = cs[turn].capacitiveSensor(30);

  if (measure > meanThreshold[turn] + dynamicOffset[turn] && noteState[turn] == 0 && millis() - timer[turn] > 10)
  {
    dynamicOffset[turn] = (measure - meanThreshold[turn]);
    if (dynamicOffset[turn] > 500) dynamicOffset[turn] = 500;
    else if (dynamicOffset[turn] < 200) dynamicOffset[turn] = 200;

    timer[turn] = millis();
    // MIDI note on
    noteState[turn] = 1;
    usbMIDI.sendNoteOn(scale[turn], 90, channel);
  } else if (measure < meanThreshold[turn] && noteState[turn] == 1 && millis() - timer[turn] > 10)
  {
    //MIDI note off
    noteState[turn] = 0;
    timer[turn] = millis();
    usbMIDI.sendNoteOff(scale[turn], 0, channel);

  } else if (noteState[turn] == 0 && millis() - timer[turn] > 10) {
    timer[turn] = millis();
    // if the sensed value is neither a note on or off
    // Dynamic threshold

    sensorBuffer[turn][buf[turn]] = measure;
    buf[turn] = (buf[turn] + 1) % sensorBufferAmount;

    // reset threshold
    meanThreshold[turn] = 0;
    for (int i = 0; i < sensorBufferAmount; i++) {
      // add all the sensed data
      meanThreshold[turn] += sensorBuffer[turn][i];
    }
    // moving average
    meanThreshold[turn] /= sensorBufferAmount;

    meanThreshold[turn] += minithreshold;
    //if(buf[turn] > sensorBufferAmount) buf[turn] = 0;
  }

  turn = (turn + 1) % sensorAmount;
  
  readPotentiometer();
  readAccelerometerData();
}

void readPotentiometer() {
  int p1 = analogRead(pot1);
  int p1_mapped = map(p1, 1023, 0, 0, 127);
  delayMicroseconds(30);
  int p2 = analogRead(pot2);
  int p2_mapped = map(p2, 1023, 0, 0, 127);

  // Send CC values
  if (p1_mapped != potPrevious1) {
    usbMIDI.sendControlChange(0, p1_mapped, channel + 1);
    potPrevious1 = p1_mapped;
  }
  if (p2_mapped != potPrevious2) {
    usbMIDI.sendControlChange(1, p2_mapped, channel + 1);
    potPrevious2 = p2_mapped;
  }
  delayMicroseconds(30);
}
void readAccelerometerData() {
  // Accelerometer
  int x = analogRead(analogPinX);
  delayMicroseconds(30);
  int y = analogRead(analogPinY);
  delayMicroseconds(30);
  int z = analogRead(analogPinZ);

  // store the reading the buffer
  accBuffer[0][accCounter] = x;
  accBuffer[1][accCounter] = y;
  accBuffer[2][accCounter] = z;

  accCounter = (accCounter + 1) % sensorBufferAmount;
  if (sendCCTimer + 10 < millis()) {
    sendCCTimer = millis();
    for (int i = 0; i < sensorBufferAmount; i++) {
      x += accBuffer[0][i];
      y += accBuffer[1][i];
      z += accBuffer[2][i];
    }
    
    int xMap = map(x / sensorBufferAmount, 0, 1023, 0, 127);
    int yMap = map(y / sensorBufferAmount, 0, 1023, 0, 127);
    int zMap = map(z / sensorBufferAmount, 0, 1023, 0, 127);
    
    usbMIDI.sendControlChange(2, xMap,channel + 1);
    usbMIDI.sendControlChange(3, yMap, channel + 1);
    usbMIDI.sendControlChange(4, zMap, channel + 1);
  }

}


