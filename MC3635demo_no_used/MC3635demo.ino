#include "MC3635.h"

MC3635 MC3635_acc = MC3635();
void setup() {
  Serial.begin(9600);
  pinMode(8 , OUTPUT);
  digitalWrite(8, HIGH);
  delay(10);

  MC3635_acc.SetNumOfDevice(1);
  MC3635_acc.SetCSPin(0, 10);
  MC3635_acc.SetInterface(1);
  MC3635_acc.start(1); // SPI interface

 
  
  // Set measurement range here:
 // MC3635_acc.SetRangeCtrl(MC3635_RANGE_2G,0);  // Choose: RANGE_2G, RANGE_4G, RANGE_8G, RANGE_16G
  
  Serial.println("mCube Accelerometer MC3635:");
 // checkRange();
  //checkResolution();
  //checkSamplingRate();
}


void checkRange()
{
  switch(MC3635_acc.GetRangeCtrl(0))
  {
    case MC3635_RANGE_16G:            Serial.println("Range: +/- 16 g"); break;     
    case MC3635_RANGE_12G:            Serial.println("Range: +/- 12 g"); break;         
    case MC3635_RANGE_8G:             Serial.println("Range: +/- 8 g"); break;
    case MC3635_RANGE_4G:             Serial.println("Range: +/- 4 g"); break;
    case MC3635_RANGE_2G:             Serial.println("Range: +/- 2 g"); break;
    default:                          Serial.println("Range: +/- 8 g"); break;
  }   
}  

void checkResolution()
{
  switch(MC3635_acc.GetResolutionCtrl(0))
  {
    case MC3635_RESOLUTION_6BIT:            Serial.println("Resolution: 6bit"); break;     
    case MC3635_RESOLUTION_7BIT:            Serial.println("Resolution: 7bit"); break;         
    case MC3635_RESOLUTION_8BIT:            Serial.println("Resolution: 8bit"); break;
    case MC3635_RESOLUTION_10BIT:           Serial.println("Resolution: 10bit"); break;
    case MC3635_RESOLUTION_14BIT:           Serial.println("Resolution: 14bit"); break;
    case MC3635_RESOLUTION_12BIT:           Serial.println("Resolution: 12bit"); break;
    default:                                Serial.println("Resolution: 14bit"); break;
  } 
}

void checkSamplingRate()
{
  switch(MC3635_acc.GetCWakeSampleRate(0))
  {
    MC3635_CWAKE_SR_DEFAULT_50Hz:   Serial.println("Output Sampling Rate: 50 Hz"); break;
    MC3635_CWAKE_SR_0p4Hz:                  Serial.println("Output Sampling Rate: 0.4 Hz"); break;
    MC3635_CWAKE_SR_0p8Hz:                  Serial.println("Output Sampling Rate: 0.8 Hz"); break; 
    MC3635_CWAKE_SR_2Hz:                    Serial.println("Output Sampling Rate: 2 Hz"); break; 
    MC3635_CWAKE_SR_6Hz:                    Serial.println("Output Sampling Rate: 6 Hz"); break; 
    MC3635_CWAKE_SR_13Hz:                   Serial.println("Output Sampling Rate: 13 Hz"); break; 
    MC3635_CWAKE_SR_25Hz:           Serial.println("Output Sampling Rate: 25 Hz"); break; 
    MC3635_CWAKE_SR_50Hz:                   Serial.println("Output Sampling Rate: 50 Hz"); break;
    MC3635_CWAKE_SR_100Hz:                  Serial.println("Output Sampling Rate: 100 Hz"); break; 
    MC3635_CWAKE_SR_200Hz:                  Serial.println("Output Sampling Rate: 200 Hz"); break; 
    default:                                Serial.println("Output Sampling Rate: 50 Hz"); break;
  }   
}  

void loop() {
  
  
    MC3635_acc_t rawAccel = MC3635_acc.readRawAccel(0);

    // Optional: filter out bad data
  //  if (isnan(rawAccel.XAxis_g) || abs(rawAccel.XAxis_g) > 50) return;

    // Get time since start (in milliseconds)
    unsigned long timestamp = millis();

    // Print time + acceleration values
    Serial.print(timestamp);         // time
    Serial.print(",");
    Serial.print(rawAccel.XAxis_g);  // X
    Serial.print(",");
    Serial.print(rawAccel.YAxis_g);  // Y
    Serial.print(",");
    Serial.println(rawAccel.ZAxis_g); // Z (end with newline)

    delay(10);  // control update rate


}
