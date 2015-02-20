from distutils.core import setup


data = [('', ['videoInput/x64/Release/_videoInput.lib', 'videoInput/x64/Release/_videoInput.pyd'])]

setup(name='pyvideoinput',
      version='0.1',
      py_modules = ['videoInput'],
      data_files=data
      )