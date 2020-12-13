#!/usr/bin/env python
#
# Copyright 2018 Espressif Systems (Shanghai) PTE LTD
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from __future__ import print_function
from builtins import input
import argparse
import textwrap
import time
import os
import sys
import json
from getpass import getpass

import esp_prov.security as security
import esp_prov.transport as transport
import esp_prov.prov as prov

# Set this to true to allow exceptions to be thrown
config_throw_except = False

class ProvisioningAPI():
    def on_except(self, err):
        if config_throw_except:
            raise RuntimeError(err)
        else:
            print(err)


    def get_security(self, secver, pop=None, verbose=False):
        if secver == 1:
            return security.Security1(pop, verbose)
        elif secver == 0:
            return security.Security0(verbose)
        return None


    def get_transport(self, sel_transport, service_name):
        try:
            tp = None
            if (sel_transport == 'softap'):
                if service_name is None:
                    service_name = '192.168.4.1:80'
            return transport.Transport_HTTP(service_name)
        except RuntimeError as e:
            on_except(e)
            return None


    def version_match(self, tp, protover, verbose=False):
        try:
            response = tp.send_data('proto-ver', protover)

            if verbose:
                print("proto-ver response : ", response)

            # First assume this to be a simple version string
            if response.lower() == protover.lower():
                return True

            try:
                # Else interpret this as JSON structure containing
                # information with versions and capabilities of both
                # provisioning service and application
                info = json.loads(response)
                if info['prov']['ver'].lower() == protover.lower():
                    return True

            except ValueError:
                # If decoding as JSON fails, it means that capabilities
                # are not supported
                return False

        except Exception as e:
            on_except(e)
            return None


    def has_capability(self, tp, capability='none', verbose=False):
        # Note : default value of `capability` argument cannot be empty string
        # because protocomm_httpd expects non zero content lengths
        try:
            response = tp.send_data('proto-ver', capability)

            if verbose:
                print("proto-ver response : ", response)

            try:
                # Interpret this as JSON structure containing
                # information with versions and capabilities of both
                # provisioning service and application
                info = json.loads(response)
                supported_capabilities = info['prov']['cap']
                if capability.lower() == 'none':
                    # No specific capability to check, but capabilities
                    # feature is present so return True
                    return True
                elif capability in supported_capabilities:
                    return True
                return False

            except ValueError:
                # If decoding as JSON fails, it means that capabilities
                # are not supported
                return False

        except RuntimeError as e:
            on_except(e)

        return False


    def get_version(self, tp):
        response = None
        try:
            response = tp.send_data('proto-ver', '---')
        except RuntimeError as e:
            on_except(e)
            response = ''
        return response


    def establish_session(self, tp, sec):
        try:
            response = None
            while True:
                request = sec.security_session(response)
                if request is None:
                    break
                response = tp.send_data('prov-session', request)
                if (response is None):
                    return False
            return True
        except RuntimeError as e:
            on_except(e)
            return None


    def custom_config(self, tp, sec, custom_info, custom_ver):
        try:
            message = prov.custom_config_request(sec, custom_info, custom_ver)
            response = tp.send_data('custom-config', message)
            return (prov.custom_config_response(sec, response) == 0)
        except RuntimeError as e:
            on_except(e)
            return None


    def custom_data(self, tp, sec, custom_data):
        try:
            message = prov.custom_data_request(sec, custom_data)
            response = tp.send_data('custom-data', message)
            return (prov.custom_data_response(sec, response) == 0)
        except RuntimeError as e:
            on_except(e)
            return None


    def scan_wifi_APs(self, sel_transport, tp, sec):
        APs = []
        group_channels = 0
        readlen = 100
        if sel_transport == 'softap':
            # In case of softAP we must perform the scan on individual channels, one by one,
            # so that the Wi-Fi controller gets ample time to send out beacons (necessary to
            # maintain connectivity with authenticated stations. As scanning one channel at a
            # time will be slow, we can group more than one channels to be scanned in quick
            # succession, hence speeding up the scan process. Though if too many channels are
            # present in a group, the controller may again miss out on sending beacons. Hence,
            # the application must should use an optimum value. The following value usually
            # works out in most cases
            group_channels = 5
        elif sel_transport == 'ble':
            # Read at most 4 entries at a time. This is because if we are using BLE transport
            # then the response packet size should not exceed the present limit of 256 bytes of
            # characteristic value imposed by protocomm_ble. This limit may be removed in the
            # future
            readlen = 4
        try:
            message = prov.scan_start_request(sec, blocking=True, group_channels=group_channels)
            start_time = time.time()
            response = tp.send_data('prov-scan', message)
            stop_time = time.time()
            prov.scan_start_response(sec, response)

            message = prov.scan_status_request(sec)
            response = tp.send_data('prov-scan', message)
            result = prov.scan_status_response(sec, response)
            if result["count"] != 0:
                index = 0
                remaining = result["count"]
                while remaining:
                    count = [remaining, readlen][remaining > readlen]
                    message = prov.scan_result_request(sec, index, count)
                    response = tp.send_data('prov-scan', message)
                    APs += prov.scan_result_response(sec, response)
                    remaining -= count
                    index += count

        except RuntimeError as e:
            on_except(e)
            return None

        return APs


    def send_wifi_config(self, tp, sec, ssid, passphrase):
        try:
            message = prov.config_set_config_request(sec, ssid, passphrase)
            response = tp.send_data('prov-config', message)
            return (prov.config_set_config_response(sec, response) == 0)
        except RuntimeError as e:
            on_except(e)
            return None


    def apply_wifi_config(self, tp, sec):
        try:
            message = prov.config_apply_config_request(sec)
            response = tp.send_data('prov-config', message)
            return (prov.config_apply_config_response(sec, response) == 0)
        except RuntimeError as e:
            on_except(e)
            return None


    def get_wifi_config(self, tp, sec):
        try:
            message = prov.config_get_status_request(sec)
            response = tp.send_data('prov-config', message)
            return prov.config_get_status_response(sec, response)
        except RuntimeError as e:
            on_except(e)
            return None


    def wait_wifi_connected(self, tp, sec):
        """
        Wait for provisioning to report Wi-Fi is connected

        Returns True if Wi-Fi connection succeeded, False if connection consistently failed
        """
        TIME_PER_POLL = 5
        retry = 3

        while True:
            time.sleep(TIME_PER_POLL)
            print("\n==== Wi-Fi connection state  ====")
            ret = get_wifi_config(tp, sec)
            if ret == "connecting":
                continue
            elif ret == "connected":
                print("==== Provisioning was successful ====")
                return True
            elif retry > 0:
                retry -= 1
                print("Waiting to poll status again (status %s, %d tries left)..." % (ret, retry))
            else:
                print("---- Provisioning failed ----")
                return False


    def desc_format(self, *args):
        desc = ''
        for arg in args:
            desc += textwrap.fill(replace_whitespace=False, text=arg) + "\n"
        return desc

