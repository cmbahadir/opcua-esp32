#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := opcua_esp32
EXCLUDE_COMPONENTS := mqtt \
			asio \
			coap \
			jsmn \
			bt \
			openssl \
			aws_iot \
			expat \
			freemodbus \
			libsodium 
include $(IDF_PATH)/make/project.mk

