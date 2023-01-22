idf_component_register(SRCS "opcua_esp32.c"
                    INCLUDE_DIRS ".")
component_compile_options(-Wno-error=format= -Wno-format)