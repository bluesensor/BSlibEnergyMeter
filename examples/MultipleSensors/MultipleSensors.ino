#include <BslibEnergyMeter.h>

// Sensores
#define PIN_CURRENT_REF 12
#define PIN_CURRENT 13
#define PIN_VOLTAGE 14
#define PIN_BATTERY 15

#define CURRENT_FACTOR 0.200
#define VOLTAGE_FACTOR 0.60
#define BATTERY_FACTOR 0.20
#define BATTERY_OFFSET 1.00

BslibEnergyMeter CurrentMeter;
BslibEnergyMeter VoltageMeter;
BslibEnergyMeter BatteryMeter;

void setup()
{
  CurrentMeter.SensorCurrent(PIN_CURRENT_REF, PIN_CURRENT, CURRENT_FACTOR);
  VoltageMeter.SensorVoltage(PIN_VOLTAGE, VOLTAGE_FACTOR);
  BatteryMeter.SensorBattery(PIN_BATTERY, BATTERY_FACTOR, BATTERY_OFFSET);
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
  float voltaje = VoltageMeter.GetVoltage(50);
  float battery = BatteryMeter.GetBattery(50);

  Serial.print("Corriente en motor N del sistema:\t");
  Serial.println(voltaje);

  Serial.print("Voltaje en motor N del sistema:\t");
  Serial.println(corriente);

  Serial.print("Nivel de bateria del sistema:\t");
  Serial.println(battery);
}
