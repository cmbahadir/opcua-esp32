from distutils.core import setup

setup(
    name="ServerConfigurator",
    version="1.0.0",
    description="Selftronic OPC UA Server Configurator Tool",
    long_description="Check the docs at https:://selftronics.com",
    long_description_content_type="text/markdown",
    url="https://github.com/cmbahadir/opcua-esp32",
    author="Selftronics",
    author_email="info@selftronics.com",
    license="MIT",
    classifiers=[
        "License :: OSI Approved :: Apache License",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
    ],
    packages=["esp_prov"],
    include_package_data=True,
    install_requires=[
        "google", "PyQt5", "PyQt5-sip", "cryptography", "future", "protobuf", "dbus-python"
    ],
    entry_points={"console_scripts": ["ServerConfigurator=ServerConfigurator.__main__:main"]},
)