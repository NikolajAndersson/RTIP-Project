#include <CapacitiveSensor.h>

const int sensorAmount = 12;
const int sensorBufferAmount = 20;
long sensorBuffer[sensorAmount][sensorBufferAmount];
long lastMeasure[sensorAmount];
long dynamicOffset[sensorAmount];
bool noteState[sensorAmount];
unsigned long timer[sensorAmount];
long meanThreshold[sensorAmount];
int buf[sensorAmount];

#define CS(Y) CapacitiveSensor(14, Y)

CapacitiveSensor cs[] = {CS(0), CS(1), CS(2), CS(3), CS(5), CS(6), CS(7), CS(8), CS(9),CS(10),CS(11), CS(12)}; // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

long minithreshold = 300;
long off = 400;
int turn = 0;
int scale[] = {69,60,65,62,61,67,64,71,70,66,74, 72};
int channel = 1;

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
}

void loop() {

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
  //lastMeasure[turn] = measure;
  turn = (turn + 1) % sensorAmount;

  //delay(100);
}
