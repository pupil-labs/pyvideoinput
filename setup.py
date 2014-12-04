from distutils.core import setup
from distutils.extension import Extension

from Cython.Build import cythonize

fromSource = False

extensions = None
if (fromSource):
    extensions = [Extension(  name="pyVideoInput",
                    sources=['pyVideoInput.pyx', 'videoInput/videoInputSrcAndDemos/libs/videoInput/videoInput.cpp'],
                    language="c++",
                    include_dirs= ['videoInput/videoInputSrcAndDemos/libs/'],
                    library_dirs= ['videoInput/videoInputSrcAndDemos/libs/DShow/lib/', 'videoInput/videoInputSrcAndDemos/libs/glfw', 'videoInput/videoInputSrcAndDemos/libs/extra/'],
                    libraries= ['ddraw', 'dxguid', 'ole32', 'strmiids', 'uuid', 'glfw'],
                    extra_compile_args=['/EHsc', '/Zc:wchar_t'],
                    extra_link_args=['/MACHINE:X64', '/NOLOGO', '/LTCG'])  
    ]
else:
    extensions = [Extension(  name="pyVideoInput",
                sources=['pyVideoInput.pyx'],
                language="c++",
                extra_objects= ['videoInput.lib'],
                include_dirs= ['videoInput/videoInputSrcAndDemos/libs/'],
                library_dirs= ['videoInput/compiledLib/vs2012/x64/'])
    ]

setup(  name="pyVideoInput",
        version="0.0.1",
        author= 'Michael Barz',
        license = 'MIT',
        description="Python bindings for videoInput library.",
        ext_modules=cythonize(extensions)
)