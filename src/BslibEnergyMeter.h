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

class BslibEnergyMeter
{
public:

	void SetSensorCurrent(unsigned int _inPinCurrent, unsigned int _inPinCurrentRef, float _factorCurrent);
	void SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage, float _offsetVoltage);
	void SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage);

	unsigned int FilterValueADC(unsigned int pinADC);
	unsigned int AutoCalibrationCurrent(unsigned int lastVRef);
	void SetCurrentReference(unsigned int _currentReference);
	void SetAnalogReference(float _analogReference);
	void SetFilterSamples(unsigned int _numberOfSamples);


	float SoftwareDCA (unsigned int digitalValue);
	float GetCurrent();
	float GetCurrentADC();
	int GetCurrentRaw();
	float GetVoltage();

	float ReadVcc();


private:
	unsigned int currentReference;
	unsigned int calibrationCurrent;

	unsigned int inPinCurrent;
	unsigned int inPinCurrentRef;
	unsigned int inPinVoltage;

	unsigned int numberOfSamples;
	float analogReference;
	float factorCurrent;
	float factorVoltage;
	float offsetVoltage;
};

#endif
