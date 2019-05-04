#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := open62541_esp32
EXCLUDE_COMPONENTS := 	protocomm \
		      	mqtt \
			asio \
			coap \
			jsmn \
			bt \
			openssl \
			aws_iot \
			expat \
			freemodbus \
			libsodium \
			json \
			mdns
include $(IDF_PATH)/make/project.mk

