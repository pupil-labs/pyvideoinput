from distutils.core import setup


data = [('', ['videoInput/Release/_videoInput.lib', 'videoInput/Release/_videoInput.pyd'])]

setup(name='pyvideoinput',
      version='0.1',
      py_modules = ['videoInput'],
      data_files=data
      )