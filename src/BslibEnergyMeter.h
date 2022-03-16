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

#if 

class BslibEnergyMeter
{
public:

	void SetSensorCurrent(unsigned int _inPinCurrent, unsigned int _inPinCurrentRef, float _factorCurrent);
	void SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage, float _offsetVoltage);
	void SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage);

	unsigned int FilterValueADC(unsigned int pinADC, unsigned int samples);
	unsigned int AutoCalibrationCurrent(unsigned int _numberOfSamples);
	void SetCurrentReference(unsigned int _currentReference);
	void SetAnalogReference(float _analogReference);

	float SoftwareDCA (unsigned int digitalValue);
	float GetCurrent(unsigned int _numberOfSamples);
	float GetVoltage(unsigned int _numberOfSamples);
	// float DebugDCA();

private:
	unsigned int currentReference;
	unsigned int calibrationCurrent;

	unsigned int inPinCurrent;
	unsigned int inPinCurrentRef;
	unsigned int inPinVoltage;

	float analogReference;
	float factorCurrent;
	float factorVoltage;
	float offsetVoltage;
};

#endif
