idf_component_register(SRCS "DHT22.c" "model.c"
                    INCLUDE_DIRS "include" "../open62541lib/include"
                    REQUIRES "driver")
component_compile_options(-Wno-error=format= -Wno-format)