# BlueSensor Energy Meter Arduino Library

![GitHub release (latest by date)](https://img.shields.io/github/v/release/bluesensor/BSlibEnergyMeter)
[![arduino linter](https://github.com/bluesensor/BSlibEnergyMeter/actions/workflows/main.yml/badge.svg?branch=main&event=push)](https://github.com/bluesensor/BSlibEnergyMeter/actions/workflows/main.yml)
[![Library format version](https://img.shields.io/badge/Arduino--Library%20format-v1.5.x%2B-blue)](https://arduino.github.io/arduino-cli/latest/library-specification/)
![cpp format](https://img.shields.io/badge/Clang--Format%20style%20-LLVM-orange)
![GitHub](https://img.shields.io/github/license/bluesensor/BSlibEnergyMeter)
![Copyright](https://img.shields.io/badge/Copyright-Datanalitic%20S.A.-informational?style=flat&logo=data:image/png;base64,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&color=002147)

## Introducción

Esta librería permite obtener lecturas de sensores analógicos de voltaje (voltios) y corriente (amperios) en **circuitos DC**.

## Compatibilidad

Soporta sensores de corriente que tengan salida de referencia analógica de corriente cero y segidores de voltaje
Dependiendo de su **función de transferencia nominal** acepta configuracion de **factor de sensibilidad** y/o **compensación(offset)**.

## Uso

### LLamada a la biblioteca

```cpp
#include <BslibEnergyMeter>
```

## Ejemplo de uso

En carpeta [examples/MultipleSensors](https://github.com/bluesensor/BSlibEnergyMeter/blob/main/examples/MultipleSensors/MultipleSensors.ino) hay un ejemplo de uso de la librería para voltaje y corriente al mismo tiempo, debe configurar los pines y las valores de la funcion de transferencia nominal detallado en el datasheet de su integrado o calibrarlo para poder obtener un valor adecuado.

## Formato de código (clang-format)

El estilo de código debe formatearse de acuerdo con [los estándares de codificación LLVM](https://llvm.org/docs/CodingStandards.html) que es el valor predeterminado de la herramienta clang-format. Si realiza unan modificación/mejora debe instalar y ejecutar la herramienta en su ordenador para mantener el formato.

```shell
clang-format -i <archivo>`
```
