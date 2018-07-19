# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/lwip/include/lwip $(PROJECT_PATH)/components/lwip/include/lwip/port $(PROJECT_PATH)/components/lwip/include/lwip/posix $(PROJECT_PATH)/components/lwip/apps/ping
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/lwip -llwip
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += lwip
component-lwip-build: 
