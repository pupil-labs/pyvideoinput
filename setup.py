from distutils.core import setup
from distutils.extension import Extension

from Cython.Build import cythonize

extensions = [
    Extension(  name="pyVideoInput",
                sources=['pyVideoInput.pyx'],
                language="c++",
                include_dirs = ['videoInput/videoInputSrcAndDemos/libs/videoInput/'])
]

setup(  name="pyVideoInput",
        version="0.0.1",
        author= 'Michael Barz',
        license = 'MIT',
        description="Python bindings for videoInput library.",
        ext_modules=cythonize(extensions)
)