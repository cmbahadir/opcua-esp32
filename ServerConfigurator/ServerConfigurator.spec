# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['ServerConfigurator.py'],
             pathex=['/home/cmb/virtualenvs/esp/lib/python3.6/site-packages/', '/home/cmb/Workspace/esp/esp-idf/examples/opcua-esp32/ServerConfigurator'],
             binaries=[],
             datas=[],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [],
          name='ServerConfigurator',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          upx_exclude=[],
          runtime_tmpdir=None,
          console=True )
