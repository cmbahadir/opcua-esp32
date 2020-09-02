# OPC UA Server on ESP32
[![Build Status](https://travis-ci.org/cmbahadir/opcua-esp32.svg?branch=master)](https://travis-ci.org/cmbahadir/opcua-esp32)

It is aimed to run OPC UA server on ESP32 board (xtensa toolchain). The information model is handled under models.c/h file. It contains a temperature sensor(DHT22) read port 4, a callback method to call ESP32-WROOM on board LED and 2 relays, which can be controlled as OPCUA objects.

    On Board LED : GPIO 2
    DHT22 Sensor : GPIO 4
    RELAY 0      : GPIO 32
    RELAY 1      : GPIO 33

It is tested by UAExpert and also Matrikon Mobile IOS OPC UA Client. So if you face any problems with other clients please let me know.

### Prerequisites

- ESP-IDF : https://docs.espressif.com/projects/esp-idf/en/latest/get-started/
- Open62541 (Optional) : https://github.com/open62541/open62541

## Getting Started
A more detailed information can be found here: https://selftronics.com/2020/06/23/cheapest-embedded-opc-ua-server/

It is required to have locally configured ESP-IDF (https://github.com/espressif/esp-idf).

    git clone --recursive https://github.com/espressif/esp-idf.git <your-esp-idf-root-path>
    export IDF_PATH=<your-esp-idf-root-path>
    export PATH=$PATH:<xtensa-toolchain-path>
    cd <your-esp-idf-root-path>/examples/
    git clone https://github.com/cmbahadir opcua-esp32
    cd opcua-esp32
    idf.py build
    
Open62541 stack functionaliy is provided by open62541.c and open62541.h files. If you want to dive into OPC UA part, it is advised to check Open62541 project.

Also, if you want to build open62541.c/.h/.lib from source, note the changes on https://github.com/cmbahadir/opcua-esp32/blob/master/components/open62541lib/README.md in order to be xtensa-toolchain compatible.

- I haven't analyzed the required modifications on open62541.c/.h  files, these are just a result of trial and error process, so an explanatory contribution would be great. :)


## License

This project is licensed under the Mozilla Public License Version 2.0 which is inherited from Open62541 project - see the [LICENSE.md](LICENSE.md) file for details.
