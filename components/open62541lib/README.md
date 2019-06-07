# CMake Build Options Of Open62541:

 - Architectures included in amalgamation: freertosLWIP
 - The selected architecture is: freertosLWIP
 - Could NOT find LATEX (missing:  LATEX_COMPILER) 
 - Configuring done
 - Generating done
 - Build files have been written to: /home/cmb/workspace/opcua/open62541/build
 - Cache values

- BUILD_SHARED_LIBS:BOOL=ON
- CMAKE_BUILD_TYPE:STRING=Debug
- CMAKE_INSTALL_PREFIX:PATH=/usr/local
- LIB_INSTALL_DIR:PATH=lib
- UA_ARCHITECTURE:STRING=freertosLWIP
- UA_ARCH_ADD_FLAGS:STRING=
- UA_ARCH_EXTRA_INCLUDES:STRING=
- UA_ARCH_FREERTOS_USE_OWN_MEMORY_FUNCTIONS:BOOL=OFF
- UA_ARCH_LINKER_FLAGS:STRING=
- UA_ARCH_REMOVE_FLAGS:STRING=
- UA_BUILD_EXAMPLES:BOOL=OFF
- UA_BUILD_UNIT_TESTS:BOOL=OFF
- UA_ENABLE_AMALGAMATION:BOOL=ON
- UA_ENABLE_COVERAGE:BOOL=OFF
- UA_ENABLE_DISCOVERY:BOOL=ON
- UA_ENABLE_DISCOVERY_MULTICAST:BOOL=OFF
- UA_ENABLE_ENCRYPTION:BOOL=OFF
- UA_ENABLE_HISTORIZING:BOOL=ON
- UA_ENABLE_METHODCALLS:BOOL=ON
- UA_ENABLE_NODEMANAGEMENT:BOOL=ON
- UA_ENABLE_QUERY:BOOL=OFF
- UA_ENABLE_SUBSCRIPTIONS:BOOL=ON
- UA_ENABLE_SUBSCRIPTIONS_EVENTS:BOOL=OFF
- UA_LOGLEVEL:STRING=100
- UA_NAMESPACE_ZERO:STRING=REDUCED
- UA_ENABLE_PUBSUB:BOOL=ON
- UA_ENABLE_PUBSUB_INFORMATIONMODEL:BOOL=ON

# Open62541.c
 - Change task.h as freertos/task.h
 - int UA_access(const char *pathname, int mode) { return 0; } eklendi (open62541.c) (Optional)
 - Add freertos and lwip as component under components/.
 - Add #define UA_ARCHITECTURE_FREERTOSLWIP, this may be a bug (https://github.com/open62541/open62541/issues/2209)

# Open62541.h
 - Comment out //#define UA_access (Optional)
 - Use calloc rather than pcPortCalloc so comment out  //# define UA_calloc pvPortCalloc ->  # define UA_calloc calloc (Optional)
 - Comment out //#define UA_IPV6 LWIP_IPV6 - probably esp-idf lwip does not support IPV6
