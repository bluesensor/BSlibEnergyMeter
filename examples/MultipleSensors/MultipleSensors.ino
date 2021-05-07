#include <BslibEnergyMeter.h>

// Sensores
#define PIN_CURRENT_REF 12
#define PIN_CURRENT 13
#define PIN_VOLTAGE 14
#define PIN_BATTERY 15

#define CURRENT_FACTOR 0.200
#define VOLTAGE_FACTOR 0.60
#define VOLTAGE_OFFSET 0.60

BslibEnergyMeter CurrentMeter;
BslibEnergyMeter VoltageMeter;

void setup()
{
  CurrentMeter.SensorCurrent(PIN_CURRENT_REF, PIN_CURRENT, CURRENT_FACTOR);
  VoltageMeter.VoltageMeter(PIN_VOLTAGE, VOLTAGE_FACTOR, VOLTAGE_OFFSET);
  // VoltageMeter.VoltageMeter(PIN_VOLTAGE, VOLTAGE_FACTOR); tmb puede ser sin offset
}
void loop()
{

  //   calibracion de el sensor cuando hay cero amperios fluyendo por el sensor

  int referenceSensorCurrent = CurrentMeter.AutoCalibrationCurrent(179);
  Serial.print("referenceSensorCurrent:\t");
  Serial.println(referenceSensorCurrent);
  CurrentMeter.SetCurrentReference(referenceSensorCurrent);

  // lectura individual de cada sensor
  // ! la cantidad de lecturas debe ser configurada a discreción

  float corriente = CurrentMeter.GetCurrent(50);
  float voltaje = VoltageMeter1.GetVoltage(50);

  Serial.print("Corriente del sistema:\t");
  Serial.println(voltaje);

  Serial.print("Voltaje del sistema:\t");
  Serial.println(corriente);
}
