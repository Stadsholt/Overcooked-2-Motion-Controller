#include <Wire.h> 

//Accelerometer adress
int ADXL1 = 0x53;

//Digital pins for the buttons
const int PIN_BUTTON_B = 5;
const int PIN_BUTTON_RB = 6;

//X output from accelerometer
float X_out, Y_out;

//Pressure for activating the A button
const float PRESSURE_FOR_A_BUTTON = 8;
//Activation point for the chopping
const float xButtonActivationPoint = -0.6;

//State for all buttons; 0 = OFF, 1 = ON
int aButtonState, bButtonState, xButtonState, rbButtonState;

void setup() 
{
  Serial1.begin(9600);
  Serial1.setTimeout(10);
  Wire.begin(); // Initiate the Wire library

  SetupPins();
  SetupAccelerometer();
}
//Setup for the button and pressure pins.
void SetupPins()
{
  //Setup pins
  pinMode(A5, OUTPUT); //Pressure A button
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RB, INPUT_PULLUP);
}
//Setup for the accelerometer
void SetupAccelerometer()
{
  //Setup accelerometers
  Wire.beginTransmission(ADXL1); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
}
void loop() 
{
  ReadAccelerometer();
  CheckForChop();
  CheckForButtons();
  delay(10);
}
//Read the accelerometer
void ReadAccelerometer()
{
     // === Read plate acceleromter data === //
  Wire.beginTransmission(ADXL1);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL1, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
  X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
  Y_out = Y_out/256;
}
//Checking the accelerometer to see if the player has chopped
void CheckForChop()
{
  if(Y_out < xButtonActivationPoint)
  {
    if(AsLastButton(3)) return;

    xButtonState = 1;
    Serial1.write(3);
  }
  else if(Y_out > 0)
  {
    if(AsLastButton(4)) return;

    xButtonState = 0;
    Serial1.write(4);
  }
}
//Checking if a button has been pressed
void CheckForButtons()
{
  //Button A
  if(analogRead(A3) > PRESSURE_FOR_A_BUTTON)
  {
    if(!AsLastButton(1))
    {
      aButtonState = 1;
      Serial1.write(1);
    }
  }
  else
  {
    if(!AsLastButton(2))
    {
      aButtonState = 0;
      Serial1.write(2);
    }
  }
  //Button B
  if(digitalRead(PIN_BUTTON_B) == LOW)
  {
    if(!AsLastButton(5))
    {
      bButtonState = 1;
      Serial1.write(5);
    }
  }
  else
  {
    if(!AsLastButton(6))
    {
      bButtonState = 0;
      Serial1.write(6);
    }
  }
  //Button RB
  if(digitalRead(PIN_BUTTON_RB) == LOW)
  {
    if(!AsLastButton(7))
    {
      rbButtonState = 1;
      Serial1.write(7);
    }
  }
  else
  {
    if(!AsLastButton(8))
    {
      rbButtonState = 0;
      Serial1.write(8);
    }
  }
}
//Checking to see if the button is already pressed
bool AsLastButton(int _buttonPressed)
{ 
  switch (_buttonPressed) {
    //State for button A
  case 1:
    if(aButtonState == 1) return true;
    else return false;
  case 2:
    if(aButtonState == 0) return true;
    else return false;
    //State for button X
  case 3:
    if(xButtonState == 1) return true;
    else return false;
  case 4:
    if(xButtonState == 0) return true;
    else return false;
    //State for button B
  case 5:
    if(bButtonState == 1) return true;
    else return false;
  case 6:
    if(bButtonState == 0) return true;
    else return false;
    //State for button RB
  case 7:
    if(rbButtonState == 1) return true;
    else return false;
  case 8:
    if(rbButtonState == 0) return true;
    else return false;
  }
}