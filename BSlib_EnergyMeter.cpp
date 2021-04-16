/**
 * @file EnergyMeter.cpp
 * @author JP Merchan (jhonpaulo1698@gmail.com)
 * @brief Biblioteca medidor de energia, permite medir voltaje, y corriente con gran precisión.
 * @version 2.1.1
 * @date 2021-04-13
 * 
 * @copyright DataAnalitic (c) {2021}
 * 
 */

#include "BSlib_EnergyMeter.h"

/**
 * @brief Configuracion de los pines del sensor de corriente
 * 
 * @param _inPinCurrent pin analógico Vout del sensor
 * @param _inPinCurrentRef pin analógico Vref del sensor
 * @param _factorCurrent factor de sensiblidad del sensor
 */
void BSlib_EnergyMeter::SensorCurrent(unsigned int _inPinCurrent, unsigned int _inPinCurrentRef, float _factorCurrent)
{
    inPinCurrent = _inPinCurrent;
    inPinCurrentRef = _inPinCurrentRef;
    factorCurrent = _factorCurrent;
}

/**
 * @brief Configuracion de los pines del sensor de bateria
 * 
 * @param _inPinBatt pin analógico de sensor de bateria
 * @param _factorBatt factor de sensiblidad del sensor
 * @param _offsetBatt compensacion para obtener voltaje real
 */
void BSlib_EnergyMeter::SensorBattery(unsigned int _inPinBatt, float _factorBatt, float _offsetBatt)
{
    inPinBatt = _inPinBatt;
    factorBatt = _factorBatt;
    offsetBatt = _offsetBatt;
}

/**
 * @brief Configuracion de los pines del seguidor de voltaje
 * 
 * @param _inPinVolt pin analógico del sensor
 * @param _factorVolt factor de sensiblidad del sensor
 */
void BSlib_EnergyMeter::SensorVoltage(unsigned int _inPinVolt, float _factorVolt)
{
    inPinVolt = _inPinVolt;
    factorVolt = _factorVolt;
}

/**
 * @brief Filtro suave (promedio) de lecturas ADC
 * 
 * @param pinADC pin analógico al cual leer
 * @param samples número de muestras para el promedio
 * @return int 
 */
unsigned int BSlib_EnergyMeter::FilterValueADC(unsigned int pinADC, unsigned int samples)
{
    unsigned long valueADC = 0;
    unsigned int filteredValueADC = 0;
    for (unsigned int i = 0; i < samples; i++)
    {
        valueADC += analogRead(pinADC);
    }
    filteredValueADC = valueADC / samples;
    return filteredValueADC;
}

/**
 * @brief Calibrar automaticamente el sensor de corriente con Vref. Usar esta función cuando la corriente sea cero.
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return int 
 */
unsigned int BSlib_EnergyMeter::AutoCalibrationCurrent(unsigned int _numberOfSamples)
{
    calibrationCurrent = FilterValueADC(inPinCurrentRef, _numberOfSamples);
    // algunos sensores tienen un offset (compensacion) cuando Vref<Vout
    float offset = FilterValueADC(inPinCurrent, _numberOfSamples) - calibrationCurrent;
    calibrationCurrent += offset;
    return calibrationCurrent;
}

/**
 * @brief Se usar después de la función AutoCalibrationCurrent para configurar el valor de referencia de corriente debido a que no siempre la corriente es cero.
 * 
 * @param _currentReference 
 */
void BSlib_EnergyMeter::SetCurrentReference(unsigned int _currentReference)
{
    currentReference = _currentReference;
}

/**
 * @brief Obtiene la corriente del sensor motor
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return float 
 */
float BSlib_EnergyMeter::GetCurrent(unsigned int _numberOfSamples)
{
    int filteredCurrent = FilterValueADC(inPinCurrent, _numberOfSamples) - currentReference;

    if (filteredCurrent < 0)
    {
        filteredCurrent = 0;
    }

    float convertValueADC = float(filteredCurrent) / ADC_SCALE * VOLT_INPUT_DRIVER;
    float current = convertValueADC / factorCurrent;
    return current;
}

/**
 * @brief Obtiene el voltaje de la bateria
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return float 
 */
float BSlib_EnergyMeter::GetBattery(unsigned int _numberOfSamples)
{
    int filteredBatt = FilterValueADC(inPinBatt, _numberOfSamples);
    float convertValueADC = float(filteredBatt) / ADC_SCALE * VOLT_INPUT_MAIN;
    float batt = (convertValueADC * factorBatt) + offsetBatt;
    return batt;
}

/**
 * @brief Obtiene el voltaje del seguidor de corriente
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return float 
 */
float BSlib_EnergyMeter::GetVoltage(unsigned int _numberOfSamples)
{
    int filteredVolt = FilterValueADC(inPinVolt, _numberOfSamples);
    float convertValueADC = float(filteredVolt) / ADC_SCALE * VOLT_INPUT_DRIVER;
    float volt = convertValueADC * factorVolt;
    return volt;
}
