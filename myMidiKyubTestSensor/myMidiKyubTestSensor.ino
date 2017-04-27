#include <CapacitiveSensor.h>

const int sensorAmount = 7;

#define CS(Y) CapacitiveSensor(12, Y)

CapacitiveSensor cs[] = {CS(0), CS(1), CS(2), CS(3), CS(4), CS(6), CS(7)}; // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

long t = 1000;
long off = 400;
long lastMeasure[sensorAmount];
float coef = 0.5;
bool noteState[sensorAmount];
unsigned long timer[sensorAmount];
int turn = 0;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < sensorAmount; i++) {
    cs[i].set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
    noteState[i] = 0;
    timer[i] = 0;
    lastMeasure[i] = 0;
  }
  //t = cs_4_2.capacitiveSensor(30);
  Serial.begin(9600);
}

void loop() {

  long m =  cs[turn].capacitiveSensor(30);
  long measure = coef * m + lastMeasure[turn] * coef;
//  Serial.print(turn);
//  Serial.print("\t");
//  Serial.print(noteState[turn]);
//  Serial.print("\t");
//  Serial.println(measure);

  if (measure > t && noteState[turn] == 0 && millis() - timer[turn] > 10)
  {
    timer[turn] = millis();
    // MIDI note on
    noteState[turn] = 1;
    Serial.print(turn);
    Serial.print("\t");
    Serial.print(noteState[turn]);
    Serial.print("\t");
    Serial.print(measure);
    Serial.print("\t");
    Serial.println("ON");

  } else if (measure < t - off && noteState[turn] == 1 && millis() - timer[turn] > 10)
  {
    //MIDI note off
    noteState[turn] = 0;
    timer[turn] = millis();
    Serial.print(turn);
    Serial.print("\t");
    Serial.print(noteState[turn]);
    Serial.print("\t");
    Serial.print(measure);
    Serial.print("\t");
    Serial.println("OFF");
  }
  lastMeasure[turn] = measure;
  turn = (turn + 1) % sensorAmount;
}
