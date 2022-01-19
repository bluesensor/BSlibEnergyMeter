/**
 * @file BslibEnergyMeter.cpp
 * @author JP Merchan (jhonpaulo1698@gmail.com)
 * @brief Biblioteca medidor de energia, permite medir voltaje, y corriente con gran precisión.
 * @version 2.2.1
 * @date 2021-04-16
 * 
 * @copyright DataAnalitic (c) {2021}
 * 
 */

#include "BslibEnergyMeter.h"

/**
 * @brief Configuracion de los pines del sensor de corriente
 * 
 * @param _inPinCurrent pin analógico Vout del sensor de voltaje
 * @param _inPinCurrentRef pin analógico Vref del sensor
 * @param _factorCurrent factor de sensiblidad del sensor
 */
void BslibEnergyMeter::SetSensorCurrent(unsigned int _inPinCurrent, unsigned int _inPinCurrentRef, float _factorCurrent)
{
    inPinCurrent = _inPinCurrent;
    inPinCurrentRef = _inPinCurrentRef;
    factorCurrent = _factorCurrent;
}

/**
 * @brief Configuracion de los pines del sensor de voltaje
 * 
 * @param _inPinVoltage pin analógico de Vout de sensor de voltaje
 * @param _factorVoltage factor de sensiblidad del sensor
 * @param _offsetVoltage compensacion para obtener voltaje real
 */
void BslibEnergyMeter::SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage, float _offsetVoltage = 0)
{
    inPinVoltage = _inPinVoltage;
    factorVoltage = _factorVoltage;
    offsetVoltage = _offsetVoltage;
}

/**
 * @brief Configuracion de los pines del sensor de voltaje
 * 
 * @param _inPinVoltage pin analógico de sensor de voltaje
 * @param _factorVoltage factor de sensiblidad del sensor
 */
void BslibEnergyMeter::SetSensorVoltage(unsigned int _inPinVoltage, float _factorVoltage)
{
    inPinVoltage = _inPinVoltage;
    factorVoltage = _factorVoltage;
}

/**
 * @brief Filtro suave (promedio) de lecturas ADC
 * 
 * @param pinADC pin analógico al cual leer
 * @param samples número de muestras para el promedio
 * @return int 
 */
unsigned int BslibEnergyMeter::FilterValueADC(unsigned int pinADC, unsigned int samples)
{
    unsigned long valueADC = 0;
    unsigned int filteredValueADC = 0;
    float varVolt=0.00450211785842;
    float varProcess=1e-8;
    float Pc=0.0;
    float G=0.0;
    float P=1.0;
    float Xp=0.0;
    float Zp=0.0;
    float Xe=0.0;


    for (unsigned int i = 0; i < samples; i++)
    {
        valueADC += analogRead(pinADC);
            // kalman process
        Pc = P + varProcess;
        G = Pc/(Pc + varVolt);    // kalman gain
        P = (1-G)*Pc;
        Xp = Xe;
        Zp = Xp;
        Xe = G*((float)valueADC-Zp)+Xp;   // the kalman estimate of the sensor voltage
    }
    filteredValueADC = (int)Xe;
    return filteredValueADC;
}

/**
 * @brief Calibrar automaticamente el sensor de corriente con Vref. Usar esta función cuando la corriente sea cero.
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return int 
 */
unsigned int BslibEnergyMeter::AutoCalibrationCurrent(unsigned int _numberOfSamples)
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
void BslibEnergyMeter::SetCurrentReference(unsigned int _currentReference)
{
    currentReference = _currentReference;
}

/**
 * @brief Obtiene la corriente del sensor motor
 * 
 * @param _numberOfSamples número de muestras a tomar en cada lenctura
 * @return float 
 */
float BslibEnergyMeter::GetCurrent(unsigned int _numberOfSamples)
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
float BslibEnergyMeter::GetVoltage(unsigned int _numberOfSamples)
{
    int filteredVoltage = FilterValueADC(inPinVoltage, _numberOfSamples);
    float convertValueADC = float(filteredVoltage) / ADC_SCALE * VOLT_INPUT_MAIN;
    float voltage = (convertValueADC * factorVoltage) + offsetVoltage;

    return voltage;
}
