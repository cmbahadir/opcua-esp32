# OPC UA Server on ESP32

An OPC UA running server on ESP32 board with a minimal information model to control on board relays of ESP32-EVB development board. Also with a temperature node reading over GPIO 4 from a DHT-22 sensor.

    DHT22 Sensor : GPIO 4
    RELAY 0      : GPIO 32
    RELAY 1      : GPIO 33

It is tested with UAExpert and Matrikon Mobile IOS OPC UA Client. If you see any problems with other clients please let me know.

### Prerequisites

- Hardware : Development is based on [ESP32-EVB](https://www.olimex.com/Products/IoT/ESP32/ESP32-EVB/open-source-hardware) from Olimex but any ESP32 board is expected to work.
- ESP-IDF : https://docs.espressif.com/projects/esp-idf/en/latest/get-started/

## Getting Started
Blog post with more information: [Selftronics](https://selftronics.com/2020/06/23/cheapest-embedded-opc-ua-server/)

It is required to have [ESP-IDF](https://github.com/espressif/esp-idf) and configured in local.

    git clone --recursive https://github.com/espressif/esp-idf.git <your-esp-idf-root-path>
    cd <your-esp-idf-root-path>
    ./install.sh
    source export.sh
    cd <your-esp-idf-root-path>/examples/
    git clone https://github.com/cmbahadir opcua-esp32
    cd opcua-esp32
    idf.py build
    
Open62541.c and open62541.h are amalgamated OPC UA C stack files. If you want to dive into OPC UA part, please check Open62541 project.

If you want to build open62541.c/.h/.lib from source, be sure to apply the changes in [here](https://github.com/cmbahadir/opcua-esp32/blob/master/components/open62541lib/README.md) to make it compatible with FreeRTOS and xtensa-toolchain.

- I haven't analyzed these modifications in open62541.c/.h  files, these are just a result of trial and error process, but open62541 project is still being developed to make it easier. You can follow [this](https://github.com/open62541/open62541/pull/2511#issuecomment-732238526) long but useful thread to gain more knowledge about open62541's FreeRTOS compatibility.

- More about OPC UA stack: [Open62541](https://github.com/open62541/open62541)

### Configuration
Type of connection, Wi-Fi SSID, Wi-fi password and static ip configuration can be set through esp-idf's menuconfig tool. Beware that static ip is only supported for ethernet connection for now. Connection configuration can be done from under "Connection configuration" on menuconfig. To launch the configuration menu run the command.

    idf.py menuconfig
    
## License

This project is licensed under the Mozilla Public License Version 2.0 which is inherited from Open62541 project - see the [LICENSE.md](LICENSE.md) file for details.
