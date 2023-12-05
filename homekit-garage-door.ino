#include "HomeSpan.h"

const int OUTPUT_PIN = 15;
const int PULSE_LENGTH = 300; // ms
const int SWITCH_RESET_DELAY = 300; // ms
const char *PAIRING_CODE = "22252225";

struct GarageDoor : Service::Switch {

  int outputPin;
  SpanCharacteristic *buttonPressed;

  GarageDoor(int pin)
    : Service::Switch() {

    buttonPressed = new Characteristic::On();
    outputPin = pin;
    pinMode(pin, OUTPUT);
  }

  boolean update() override {

    if (!buttonPressed->getNewVal()) {
      // if turning switch off, don't do anything
      return true;
    }

    digitalWrite(outputPin, true);
    delay(PULSE_LENGTH);
    digitalWrite(outputPin, false);

    return true;
  }

  void loop() override {

    // reset the button if it's been on for more than a second
    if (buttonPressed->getVal() && buttonPressed->timeVal() > SWITCH_RESET_DELAY) {
      buttonPressed->setVal(false);
    }
  }
};

void setup() {
  Serial.begin(115200);

  homeSpan.begin(Category::GarageDoorOpeners, "Garage Door Opener");
  homeSpan.setPairingCode(PAIRING_CODE);

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new GarageDoor(OUTPUT_PIN);
}

void loop() {
  homeSpan.poll();
}
