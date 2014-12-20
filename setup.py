from distutils.core import setup
from distutils.extension import Extension

from Cython.Build import cythonize

# TODO: _asm keyword does not work with 64-bit architecture. Check if we need an x64 alternative! (http://msdn.microsoft.com/de-de/library/45yd4tzz.aspx) 

fromSource = False

extensions = None
if (fromSource):
    # TODO: add DebugPrintOut, FormatReader, ImageGrabber, ImageGrapperThread, RawImage, videoDevices and Media_Foundation as sources
    extensions = [Extension(  name="pyVideoInput",
                    sources=['pyVideoInput.pyx', 'videoInputMF/videoInput.cpp'],
                    language="c++",
                    include_dirs= ['videoInputMF'],
                    library_dirs= [],
                    libraries= [],
                    extra_compile_args=['/EHsc']
                    )  
    ]
else:
    extensions = [Extension(  name="pyVideoInput",
                sources=['pyVideoInput.pyx'],
                language="c++",
                extra_objects= ['videoInput.lib'],
                include_dirs= ['videoInputMF'],
                library_dirs= ['videoInputMF/x64/Release'])
    ]
    

setup(  name="pyVideoInput",
        version="0.0.1",
        author= 'Michael Barz',
        license = 'MIT',
        description="Python bindings for videoInput library.",
        ext_modules=cythonize(extensions)
)