idf_component_register(SRCS "open62541.c"
                    INCLUDE_DIRS "include")
component_compile_options(-Wno-error=format= -Wno-format -Wempty-body)