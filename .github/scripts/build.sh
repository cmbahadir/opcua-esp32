#!/usr/bin/env bash

cd $HOME
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
bash install.sh
source export.sh

cp -r $GITHUB_WORKSPACE $HOME/esp-idf/examples/
cd $HOME/esp-idf/examples/opcua-esp32
idf.py build