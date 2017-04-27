#include <Bounce2.h>  // Bounce library makes button change detection easy
const int channel = 1;

Bounce button1 = Bounce();  // 5 = 5 ms debounce time
Bounce button2 = Bounce();  // which is appropriate for good
Bounce button3 = Bounce();  // quality mechanical pushbuttons
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
   // After setting up the button, setup the object
 button1.attach( 2);
 button1.interval(5);
 button2.attach(3);
 button2.interval(5);
 button3.attach(4);
 button3.interval(5);

}

void loop() {
  button1.update();
  button2.update();
  button3.update();
  // Note On messages when each button is pressed
  if (button1.fell()) {
    usbMIDI.sendNoteOn(60, 99, channel);  // 60 = C4
  }
  if (button2.fell()) {
    usbMIDI.sendNoteOn(61, 99, channel);  // 61 = C#4
  }
  if (button3.fell()) {
    usbMIDI.sendNoteOn(62, 99, channel);  // 62 = D4
  }
  // Note Off messages when each button is released
  if (button1.rose()) {
    usbMIDI.sendNoteOff(60, 0, channel);  // 60 = C4
  }
  if (button2.rose()) {
    usbMIDI.sendNoteOff(61, 0, channel);  // 61 = C#4
  }
  if (button3.rose()) {
    usbMIDI.sendNoteOff(62, 0, channel);  // 62 = D4
  }

  // MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
  }
}
