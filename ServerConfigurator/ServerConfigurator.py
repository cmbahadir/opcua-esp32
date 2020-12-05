#! /usr/bin/env python3
import sys

from gui import Ui_ServerConfigurator
from esp_prov import ProvisioningAPI

from PyQt5.QtCore import QRegExp
from PyQt5.QtGui import QFont, QRegExpValidator, QIntValidator, QIcon
from PyQt5.QtWidgets import QApplication, QMainWindow, QLineEdit, QMessageBox


class Window(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setWindowIcon(QIcon('logo/logo.png'))
        self.ui = Ui_ServerConfigurator()
        self.prov_api = ProvisioningAPI()
        self.ui.setupUi(self)
        self.font = QFont("Arial", 10, QFont.Serif)
        self.session = False

        self.ui.Download.setEnabled(False)
        self.ui.port_input.setEnabled(False)
        self.ui.static_ip_checkbox.setEnabled(False)
        self.ui.connection_type_combo.setEnabled(False)
        self.ui.Download.pressed.connect(self.download_configuration)

        self.ui.pushButton.pressed.connect(self.connect)
        self.ui.pop_text.setEchoMode(QLineEdit.Password)
        self.ui.wifi_password.setEchoMode(QLineEdit.Password)

        self.ui.wifi_password.setEnabled(False)
        self.ui.available_wifis.setEnabled(False)
        self.ui.connection_type_combo.currentTextChanged.connect(self.toggle_connection_type)

        self.ui.ip_address_input.setEnabled(False)
        self.ui.netmask_input.setEnabled(False)
        self.ui.gateway_input.setEnabled(False)
        self.ui.static_ip_checkbox.stateChanged.connect(self.toggle_static_ip_inputs)

        # Port validator
        self.onlyInt = QIntValidator(0, 65535)
        self.ui.port_input.setValidator(self.onlyInt)
        self.ui.port_input.setText('4840')

        # IP validator for IP Address, Netmask and Gateway fields.
        ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"
        ipRegex = QRegExp("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$")
        ipValidator = QRegExpValidator(ipRegex, self)
        self.ui.ip_address_input.setValidator(ipValidator)
        self.ui.netmask_input.setValidator(ipValidator)
        self.ui.gateway_input.setValidator(ipValidator)

    def connect(self, highlight=None):
        sec_ver = self.ui.security_version_combo.currentIndex()
        pop = self.ui.pop_text.text()

        try:
            self.security = self.prov_api.get_security(sec_ver, pop)
            self.prov_transport = self.prov_api.get_transport("softap", None)
            self.session = self.prov_api.establish_session(self.prov_transport, self.security)
            if self.session:
                self.ui.pushButton.setText('Connected')
                self.ui.pushButton.setStyleSheet("background-color : green")
            else:
                self.ui.pushButton.setText('Error')
        except Exception:
            self.showdialog()
            return

        self.ui.connection_type_combo.setEnabled(self.session)
        self.ui.static_ip_checkbox.setEnabled(self.session)
        self.ui.port_input.setEnabled(self.session)
        self.ui.Download.setEnabled(self.session)

        scanned_aps = self.prov_api.scan_wifi_APs('softap', self.prov_transport, self.security)
        if self.ui.available_wifis.count() != 0:
            self.ui.available_wifis.clear()
        for ap in scanned_aps:
            self.ui.available_wifis.addItem(ap['ssid'])

    def download_configuration(self):
        if self.ui.connection_type_combo.currentText() == 'Wi-Fi':
            ssid = self.ui.available_wifis.currentText()
            password = self.ui.wifi_password.text()

            try:
                if self.prov_api.send_wifi_config(self.prov_transport, self.security, ssid, password):
                    self.prov_api.apply_wifi_config(self.prov_transport, self.security)
            except Exception:
                return

        custom_info = self.get_custom_data()
        if self.prov_api.custom_config(self.prov_transport, self.security, custom_info, 2):
            self.ui.Download.setStyleSheet("background-color : green")
            self.ui.Download.setText("Success")
        else:
            self.ui.Download.setStyleSheet("background-color : red")
            self.ui.Download.setText("Failed")
            return

    def showdialog(self):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Critical)

        msg.setInformativeText("Could not connect to the server!")
        msg.setWindowTitle("Error!")
        msg.setDetailedText("Please check connection.\n" + 
                            "Please check security.\n" + 
                            "Please check password.")
        msg.setStandardButtons(QMessageBox.Ok)
        msg.exec_()

    def toggle_static_ip_inputs(self, state):
        if state > 0 and self.ui.connection_type_combo.currentIndex() == 0:
            self.ui.ip_address_input.setEnabled(True)
            self.ui.netmask_input.setEnabled(True)
            self.ui.gateway_input.setEnabled(True)
        else:
            self.ui.ip_address_input.setEnabled(False)
            self.ui.netmask_input.setEnabled(False)
            self.ui.gateway_input.setEnabled(False)

    def toggle_connection_type(self, state):
        if state == 'Wi-Fi':
            self.ui.wifi_password.setEnabled(True)
            self.ui.available_wifis.setEnabled(True)
        else:
            self.ui.wifi_password.setEnabled(False)
            self.ui.available_wifis.setEnabled(False)

    def get_custom_data(self):
        if not self.ui.port_input.text().strip():
            port = 4840
        else:
            port = self.ui.port_input.text()
        custom_info = "{},{},".format(self.ui.connection_type_combo.currentIndex(), port)
        if self.ui.static_ip_checkbox.isChecked():
            custom_info = custom_info + "{},{},{}".format(self.ui.ip_address_input.text(), self.ui.netmask_input.text(), self.ui.gateway_input.text())
        else:
            custom_info = custom_info + "{},{},{}".format(0,0,0)
        return custom_info


if __name__ == "__main__":
    app = QApplication(sys.argv)
    client = Window()
    client.show()
    sys.exit(app.exec_())
