#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick(0x05,0x04,8,0,true,true,false,false,false,false,false,false,false,false,false);

// Constant that maps the phyical pin to the joystick button.
const uint8_t BUTTON_1 = 0;
const uint8_t BUTTON_2 = 1;
const uint8_t BUTTON_3 = 2;
const uint8_t BUTTON_4 = 3;
const uint8_t BUTTON_5 = 4;
const uint8_t BUTTON_6 = 5;
const uint8_t BUTTON_7 = 6;
const uint8_t BUTTON_8 = 7;
const uint8_t UP = 8;
const uint8_t DOWN = 9;
const uint8_t LEFT = 10;
const uint8_t RIGHT = 11;


//Ports. change as necessary.
const uint8_t NUM_PORTS = 12;
const uint8_t ports[] = {2,3,4,5,6,7,8,9,10,16,14,15};
// Last state of the button
bool buttonStates[12] =  {};
int16_t lastX;
int16_t lastY;
void setup() {
  // Initialize Button Pins
  for (int i = 0; i < NUM_PORTS; i++)
  {
    pinMode(ports[i], INPUT_PULLUP);
  }
    
  // Initialize Joystick Library
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
  Joystick.begin(true);
}


int16_t determineAxis(int prevMinus, int prevPlus, bool minusPressed, bool plusPressed)
{
  if (minusPressed && !plusPressed) {
    return -1;
  } else if (plusPressed && !minusPressed) {
    return 1;
  } else if (!plusPressed && !minusPressed) { 
    return 0; 
  }

  return 0;
  //todo. have timestamps, and make the new value override the old value.
}
void loop() {
  bool sendMessage = false;
  // Read pin values
  for (int i = BUTTON_1; i <= BUTTON_8; i++)
  {
    bool state = !digitalRead(ports[i]);    
    if (state != buttonStates[i]) {
      Joystick.setButton(i, state);
      sendMessage = true;
    }
    buttonStates[i] = state;
  }

  bool left = !digitalRead(ports[LEFT]);
  bool right = !digitalRead(ports[RIGHT]);
  int16_t xAxis = determineAxis(0,0,left,right);
  bool up = !digitalRead(ports[UP]);
  bool down = !digitalRead(ports[DOWN]);
  int16_t yAxis = determineAxis(0,0,down, up);
  if (xAxis != lastX) {
    Joystick.setXAxis(xAxis);
    sendMessage = true;
  }
  
  if (yAxis != lastY) {
    Joystick.setYAxis(yAxis);
    sendMessage = true;
  }
  lastX = xAxis;
  lastY = yAxis;
  
  if (sendMessage) {
    Joystick.sendState();
  }
}
