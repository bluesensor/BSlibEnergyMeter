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
unsigned int BslibEnergyMeter::AutoCalibrationCurrent(unsigned int _numberOfSamples, unsigned int lastVRef)
{

    float vRef = FilterValueADC(inPinCurrentRef, _numberOfSamples);
    float vOut = FilterValueADC(inPinCurrent, _numberOfSamples);

    if (lastVRef == vRef)
    {
        calibrationCurrent = lastVRef;
    }
    else
    {
        calibrationCurrent = vRef;
    }
    // algunos sensores tienen un offset (compensacion) cuando Vref<Vout
    // float offset = vOut - vRef;
    // calibrationCurrent += offset;
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
 * @brief Configura el valor de referencia analógica para el MCU
 * 
 * @param _analogReference 
 */
void BslibEnergyMeter::SetAnalogReference(float _analogReference)
{
    analogReference = _analogReference;
}

/**
 * @brief Calcula el valor en voltaje de la lectura ADC del MCU
 * 
 * @param digitalValue 
 * @return float 
 */
float BslibEnergyMeter::SoftwareDCA (unsigned int digitalValue)
{
    float convertValueDCA = float(digitalValue) / ADC_SCALE * analogReference;
    return convertValueDCA;

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

    float voltageSensor = SoftwareDCA(filteredCurrent);
    float current = voltageSensor / factorCurrent;
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
    float voltageSensor = SoftwareDCA(filteredVoltage);
    float voltage = (voltageSensor * factorVoltage) + offsetVoltage;

    return voltage;
}

float BslibEnergyMeter::ReadVcc()
{
    long vccADC;

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB1286__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRB &= ~_BV(MUX5); // Without this the function always returns -1 on the ATmega2560 http://openenergymonitor.org/emon/node/2253#comment-11432
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);

#endif

#if defined(__AVR__)
    delay(2);            // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA, ADSC))
        ;
    vccADC = ADCL;
    vccADC |= ADCH << 8;
    vccADC = READVCC_CALIBRATION_CONST / vccADC; //1100mV*1024 ADC steps
    return vccADC / 1000.0;
#elif defined(__arm__)
    return (3300); //Arduino Due
#else
    return (3300);        //Guess that other un-supported architectures will be running a 3.3V!
#endif
}

