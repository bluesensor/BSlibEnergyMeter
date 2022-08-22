/**
 * @file BslibEnergyMeter.h
 * @author JP Merchan (jhonpaulo1698@gmail.com)
 * @brief Biblioteca medidor de energia, permite medir voltaje, y corriente con gran precisión.
 * @version 2.2.1
 * @date 2021-04-16
 *
 * @copyright DataAnalitic (c) {2021}
 *
 */

#ifndef BslibEnergyMeter_h
#define BslibEnergyMeter_h
#include "Arduino.h"

#define ADC_SCALE 1023.0

// define theoretical vref calibration constant for use in readvcc()
// 1100mV*1024 ADC steps
#ifndef READVCC_CALIBRATION_CONST
#define READVCC_CALIBRATION_CONST 1126400L
#endif

class BslibEnergyMeter {
public:
  void SetSensorCurrent(int _inPinCurrent, int _inPinCurrentRef, float _factorCurrent);
  void SetSensorVoltage(int _inPinVoltage, float _factorVoltage, float _offsetVoltage);
  void SetSensorVoltage(int _inPinVoltage, float _factorVoltage);

  void SetCurrentReference(int _currentReference);
  void SetAnalogReference(float _analogReference);
  void SetFilterSamples(int _numberOfSamples);

  int FilterValueADC(int pinADC);
  float SoftwareDAC(int digitalValue);

  int AutoCalibrationCurrent(int lastVRef);

  void CalCurrent();
  int GetCurrentRefADC();
  int GetCurrentADC();
  float GetCurrentDAC();
  float GetCurrent();

  void CalVoltage();
  int GetVoltageADC();
  float GetVoltageDAC();
  float GetVoltage();

  // int GetDistanceADC();
  // float GetDistanceDAC();
  // float GetDistance();

  float ReadVcc();

private:
  int inPinCurrent;
  int inPinCurrentRef;
  int inPinVoltage;

  int numberOfSamples;
  float analogReference;
  float factorCurrent;
  float factorVoltage;
  float offsetVoltage;
};

#endif
