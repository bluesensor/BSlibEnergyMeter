/**
 * @file EnergyMeter.h
 * @author JP Merchan (jhonpaulo1698@gmail.com)
 * @brief Biblioteca medidor de energia, permite medir voltaje, y corriente con gran precisión.
 * @version 2.1.1
 * @date 2021-04-13
 * 
 * @copyright DataAnalitic (c) {2021}
 * 
 */

#ifndef BSlib_EnergyMeter_h
#define BSlib_EnergyMeter_h
#include "Arduino.h"

#define ADC_SCALE 1023.0
#define VOLT_INPUT_MAIN 5.0
#define VOLT_INPUT_DRIVER 3.3

class BSlib_EnergyMeter
{
public:
	void SensorCurrent(unsigned int _inPinCurrent, unsigned int _inPinCurrentRef, float _factorCurrent);
	void SensorBattery(unsigned int _inPinBatt, float _factorBatt, float _offsetBatt);
	void SensorVoltage(unsigned int _inPinVolt, float _factorVolt);

	unsigned int FilterValueADC(unsigned int pinADC, unsigned int samples);
	unsigned int AutoCalibrationCurrent(unsigned int _numberOfSamples);
	void SetCurrentReference(unsigned int _currentReference);

	float GetCurrent(unsigned int _numberOfSamples);
	float GetVoltage(unsigned int _numberOfSamples);
	float GetBattery(unsigned int _numberOfSamples);

private:
	unsigned int inPinCurrent;
	unsigned int inPinCurrentRef;
	unsigned int inPinVolt;
	unsigned int inPinBatt;

	float factorCurrent;
	float factorVolt;
	float factorBatt;

	float offsetBatt;
	unsigned int currentReference;
	unsigned int calibrationCurrent;
};

#endif
