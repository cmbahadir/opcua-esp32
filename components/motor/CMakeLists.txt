idf_component_register(SRCS "motor_driver.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "esp_timer")
component_compile_options(-Wno-error=format= -Wno-format)
