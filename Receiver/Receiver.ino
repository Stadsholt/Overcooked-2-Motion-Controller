#include <XInput.h>
#include <SoftwareSerial.h>

#define tx 9
#define rx 10

SoftwareSerial connection(tx, rx);

// max Magnitune: 46339
const float plate_throw_trigger = 0.7;

//Variables for the XYindex and the XY input
int index = 0, xInput = 0, yInput = 0;

void setup() 
{
  Serial1.begin(9600);
  Serial1.setTimeout(10);
  connection.begin(9600);
  connection.setTimeout(10);
  XInput.begin();
}
//Converts a number to a new number between a MIN and MAX
int FloatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void loop() 
{
  //Reading the incoming data from the KNIFE
  if (Serial1.available()) CheckButtons(Serial1.read());
  //Reading the incoming data from the PLATE
  if (connection.available()) ReadPlate();
}
void ReadPlate()
{
  int read = (int)connection.read();
  if(read < 127)
  {
    xInput = FloatMap(read, 0, 126, -32768, 32767);
  }
  if(read > 127)
  {
    yInput = FloatMap(read, 128, 255, -32768, 32767);
  }
  UpdateLeftAnalog(xInput, yInput);
}
//Checking for button inputs
void CheckButtons(int _state)
{
  switch(_state) 
  { 
  case 1:  //A button ON
    XInput.press(BUTTON_A);
    break;
  case 2:  //A button OFF
    XInput.release(BUTTON_A);
    break;
  case 3:  //X button ON
    XInput.press(BUTTON_X);
    break;  
  case 4:  //X button OFF
    XInput.release(BUTTON_X);
    break;
  case 5:  //B button ON
  if(XInput.getButton(BUTTON_B) == true) break;
    XInput.press(BUTTON_B);
    break;
  case 6:  //B button OFF
  if(XInput.getButton(BUTTON_B) == false) break;
    XInput.release(BUTTON_B);
    break;
  case 7:  //RB button ON
    XInput.press(BUTTON_RB);
    break;
  case 8:  //RB button OFF
    XInput.release(BUTTON_RB);
    break;
  default:
    break;
  }
}
int Magnitune(double x, double y) 
{
  return sqrt(x*x + y*y);
}
//Moves left analog stic
void UpdateLeftAnalog(int _x, int _y)
{
  if (Magnitune(_x, _y) > plate_throw_trigger * 46339) 
  { //Throw trigger * max Magnitune
    CheckButtons(3);
    CheckButtons(4);
  }
  
  XInput.setJoystick(JOY_LEFT, _x, _y);
}