#! /usr/bin/env python3
import sys, os, time, subprocess

from gui import Ui_ServerConfigurator
from esp_prov import ProvisioningAPI

from PyQt5.QtCore import QProcess
from PyQt5.QtGui import QFont, QStandardItemModel
from PyQt5.QtWidgets import QApplication, QMainWindow, QMenu, QTableWidgetItem, QWidget, QDialog

class Window(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_ServerConfigurator()
        self.prov_api = ProvisioningAPI()
        self.ui.setupUi(self)
        self.font = QFont("Arial", 10, QFont.Serif)
        self.session = False

        self.ui.Download.setEnabled(False)
        self.ui.Download.pressed.connect(self.download_wifi_configuration)
        self.ui.pushButton.pressed.connect(self.connect)
        self.ui.pop_text
    
    def connect(self, highlight=None):
        sec_ver = self.ui.security_version_combo.currentIndex()
        pop = self.ui.pop_text.toPlainText()

        self.security = self.prov_api.get_security(sec_ver, pop)
        self.prov_transport = self.prov_api.get_transport("softap", None)

        try:
            self.session = self.prov_api.establish_session(self.prov_transport, self.security)
        except:
            #TODO: Info: Check the Wi-Fi connection.
            return

        self.ui.Download.setEnabled(self.session)

        scanned_aps = self.prov_api.scan_wifi_APs('softap', self.prov_transport, self.security)
        if self.ui.available_wifis.count() != 0:
            self.ui.available_wifis.clear()
        for ap in scanned_aps:
            self.ui.available_wifis.addItem(ap['ssid'])

    def download_wifi_configuration(self):
        ssid = self.ui.available_wifis.currentText()
        password = self.ui.wifi_password.toPlainText()
        
        try:
            if self.prov_api.send_wifi_config(self.prov_transport, self.security, ssid, password):
                self.prov_api.apply_wifi_config(self.prov_transport, self.security)
            else:
                print("Wifi could not be configured successfully!")
                #TODO: Info: Wi-Fi config could not be sent successfully.
        except:
            #TODO: Info: Error occured during Wi-Fi configuration
            return

    def choose_wifi(self):
        return "Not Implemented"

    def set_wifi_password(self):
        return "Not Implemented"

    def set_pop(self):
        return "Not Implemented"

    def set_security_version(self):
        return "Not Implemented"

    def download(self):
        send_wifi_config()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    client = Window()
    client.show()
    sys.exit(app.exec_())
