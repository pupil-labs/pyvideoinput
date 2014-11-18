from distutils.core import setup
from distutils.extension import Extension

from Cython.Build import cythonize


extensions = [Extension(  name="pyVideoInput",
                sources=['pyVideoInput.pyx'],
                language="c++",
                extra_objects= ['videoInput.lib', 'videoInputD.lib'],
                include_dirs= ['videoInput/videoInputSrcAndDemos/libs/'],
                library_dirs= ['videoInput/compiledLib/vs2012/'])
]
'''Extension(  name="pyVideoInput",
                sources=['pyVideoInput.pyx', 'videoInput/videoInputSrcAndDemos/libs/videoInput/videoInput.cpp'],
                language="c++",
                include_dirs= ['videoInput/videoInputSrcAndDemos/libs/'],
                library_dirs= ['videoInput/videoInputSrcAndDemos/libs/DShow/lib/', 'videoInput/videoInputSrcAndDemos/libs/extra/', 'videoInput/videoInputSrcAndDemos/libs/glfw/'],
                libraries= ['glfw', 'ddraw', 'strmbasd', 'strmbase', 'strmiids', 'uuid'])   '''

setup(  name="pyVideoInput",
        version="0.0.1",
        author= 'Michael Barz',
        license = 'MIT',
        description="Python bindings for videoInput library.",
        ext_modules=cythonize(extensions)
)