#include <Wire.h>  // Wire library - used for I2C communication
#include <RunningMedian.h>

//Pin for vibration motor
const int PIN_MOTOR = 9; 

//Vibration feedback
const float PLATE_FEEDBACK_VALUE_MIN = .6;
const float PLATE_FEEDBACK_VALUE_MAX = 1;

//For adjustment
const double DEADZONE = .2;
const float PRESSURE_FOR_BUTTON_A = 8;

// Plate ADXL345 sensor I2C address
const int ADXL1 = 0x53;

// Plate Sensors Outputs
float X_out, Y_out;

//Medians for smoothing the accelerometer input
RunningMedian samplesX = RunningMedian(5);
RunningMedian samplesY = RunningMedian(5);

void setup() 
{
  Serial1.begin(9600);
  Serial1.setTimeout(10);
  Wire.begin(); // Initiate the Wire library

  //Setup accelerometers
  Wire.beginTransmission(ADXL1); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();

  //Setup pins
  pinMode(PIN_MOTOR, OUTPUT);
}
void loop() 
{
  ReadGyroscope();
  MoveCharacter();
  CheckForFeedback();
  delay(10);
}
void ReadGyroscope()
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
//Converts a number to a new number between a MIN and MAX
int FloatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//Sending the data for moving the character
void MoveCharacter()
{
  
  //Takes the raw output and converts it to a number between 0-255
  int finalXOutput = FloatMap(X_out, -1, 1, 0, 126);
  int finalYOutput = FloatMap(Y_out, -1, 1, 128, 255);
  //Smooths the output
  samplesX.add(finalXOutput);
  samplesY.add(finalYOutput);
  finalXOutput = samplesX.getMedian();
  finalYOutput = samplesY.getMedian();

  //Checking if the output is out of the deadzone
  if(!OutOfDeadZone())
  {
    finalXOutput = 63;
    finalYOutput = 191;
  }
  //Transmitting the output
  Serial1.write(finalXOutput);
  Serial1.write(finalYOutput);
}
//Checking if the input is out of the deadzone
bool OutOfDeadZone()
{
  if(NormalizedAxis() < DEADZONE)
    return false;
  return true;
}
//Nomralizing the output
double NormalizedAxis() 
{
  return sqrt(X_out * X_out + Y_out * Y_out);
}
//Checking if there should be vibration feedback
void CheckForFeedback()
{
  float normalizedAxis = NormalizedAxis();
  if(normalizedAxis < PLATE_FEEDBACK_VALUE_MAX) RunFeedbackPlate(false);
  else if(normalizedAxis > PLATE_FEEDBACK_VALUE_MAX) RunFeedbackPlate(true);
}
//Runs the vibration motor
void RunFeedbackPlate(bool _toggle)
{
  if(_toggle)
    digitalWrite(PIN_MOTOR, HIGH);
  else
    digitalWrite(PIN_MOTOR, LOW);

}