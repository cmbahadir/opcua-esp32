# CMake Build Options Of Open62541 (v1.5.5)

 - Architecture: `freertos-lwip` (renamed from `freertosLWIP` used in older versions)
 - Requires a real FreeRTOS-Kernel checkout with the `portable/ThirdParty/GCC/Posix`
   port (used only for the host-side amalgamation build, not the ESP32 target) -
   `git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git`, then pass
   `-DFREERTOS_KERNEL_PATH=<path>` and
   `-DFREERTOS_PORT_PATH=<path>/portable/ThirdParty/GCC/Posix`.
 - The upstream `CMakeLists.txt`'s `install(EXPORT open62541Targets ...)` /
   `export(TARGETS open62541 ...)` calls fail to configure with this
   architecture (`freertos_config`/`freertos_kernel` aren't part of any
   export set). Since we only need the `open62541-amalgamation` target, not
   `make install`, guard those two blocks locally in your throwaway
   open62541 checkout with `if(NOT UA_ARCHITECTURE_FREERTOS) ... endif()`
   before running cmake. This is not something to upstream - just a local
   workaround for generating the amalgamation.

Cache values used:

```
cmake -DUA_ARCHITECTURE=freertos-lwip \
      -DUA_ENABLE_AMALGAMATION=ON \
      -DUA_BUILD_EXAMPLES=OFF -DUA_BUILD_UNIT_TESTS=OFF \
      -DUA_ENABLE_DISCOVERY=ON -DUA_ENABLE_DISCOVERY_MULTICAST=OFF \
      -DUA_ENABLE_ENCRYPTION=OFF -DUA_ENABLE_HISTORIZING=OFF \
      -DUA_NAMESPACE_ZERO=MINIMAL \
      -DUA_ENABLE_SUBSCRIPTIONS=ON -DUA_ENABLE_SUBSCRIPTIONS_EVENTS=OFF \
      -DUA_ENABLE_METHODCALLS=ON -DUA_ENABLE_NODEMANAGEMENT=ON \
      -DUA_ENABLE_QUERY=OFF -DUA_ENABLE_PUBSUB=OFF -DUA_ENABLE_PUBSUB_INFORMATIONMODEL=OFF \
      -DUA_ENABLE_DIAGNOSTICS=OFF -DUA_ENABLE_DA=OFF \
      -DUA_LOGLEVEL=300 \
      -DUA_MULTITHREADING=0 \
      -DFREERTOS_KERNEL_PATH=<path-to-FreeRTOS-Kernel> \
      -DFREERTOS_PORT_PATH=<path-to-FreeRTOS-Kernel>/portable/ThirdParty/GCC/Posix \
      ..
make open62541-amalgamation
```

## Why `UA_NAMESPACE_ZERO=MINIMAL` and not `REDUCED`

The old (v1.2) amalgamation used `REDUCED`. In v1.5.5 the generated `REDUCED`
namespace-zero nodeset has grown substantially (more default types/objects
per later versions of the OPC UA spec) and reliably exhausts this board's
available heap during bootstrap (~276KB total DRAM at boot under ESP-IDF
v6.0.2, of which only ~100-130KB remains free by the time the OPC UA task
starts, after WiFi/lwIP/SNTP overhead) — even after disabling
`UA_ENABLE_DIAGNOSTICS`/`UA_ENABLE_DA` (both default ON, unlike in v1.2) and
trimming the FreeRTOS task's stack size. `MINIMAL` avoids generating that
nodeset at all and reliably fits.

**Known limitation:** `MINIMAL` mode does not create the standard Root/Objects
folders or reference-type hierarchy (unlike `REDUCED`/`FULL`). Our custom
nodes (Temperature, Relay0, Relay1 in `components/model/model.c`) are still
created successfully and are fully readable/writable by NodeId
(`ns=1;s=temperature` etc.), but they are **not** browsable from the Objects
folder the way they were under v1.2 - a generic client like UAExpert that
auto-discovers the address space via Browse won't show them in the tree;
connecting directly by NodeId still works fully. Reconstructing a proper
browsable hierarchy under `MINIMAL` would require replicating open62541's
internal reference-type index/bitset bookkeeping (normally set up by the
generated namespace-zero bootstrap), which isn't exposed via public API and
wasn't pursued further - flagged here as a follow-up if full Browse support
is needed.

## Required amalgamation patch

Of the four patches the old (v1.2) recipe needed, only one still applies to
the freshly generated v1.5.5 amalgamation - verify by grepping the fresh
output before reapplying any of them:

```
sed -i 's|#include <FreeRTOS\.h>|#include "freertos/FreeRTOS.h"|; s|#include <task\.h>|#include "freertos/task.h"|' open62541.c
```

The other three (`UA_access` stub, `#define UA_ARCHITECTURE_FREERTOSLWIP`,
`UA_calloc`/`UA_IPV6` comment-outs) are obsolete in v1.5.5 - none of those
symbols/macros exist in the generated output anymore.

## Application-code changes needed for v1.5.5

- `UA_ServerConfig_setCustomHostname()` was removed. It was purely cosmetic
  (an advertised discovery hostname); the actual bind address comes from
  `config->serverUrls`, which `UA_ServerConfig_setMinimalCustomBuffer`
  already sets to a wildcard `opc.tcp://:<port>` that binds on all
  interfaces. Do not override `serverUrls` with a non-resolvable hostname -
  it breaks the TCP listener entirely (`"The server has no server socket"`).
