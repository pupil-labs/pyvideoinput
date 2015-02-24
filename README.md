pyvideoinput
============

pyvideoinput is a wrapper of a x64 version of the C++ library for capturing video streams presented by Evgeny Pereguda on CodeProject:
http://www.codeproject.com/Articles/776058/Capturing-Live-video-from-Web-camera-on-Windows-an

## Instalation

    git clone http://github.com/pupil-labs/pyvideoinput
    open command line and navigate to local git folder
    run "setup.py install"


## Example code

`test.py` imports the module and runs it. Requires OpenCV.

## Build Library

* Setup environment variables for SWIG on Windows: http://www.swig.org/Doc3.0/SWIGDocumentation.html#Windows_installation (see chapter 3.2.1.4)
* Open and build the VS solution (SWIG gets executed as pre-build event)
* Install pyvideoinput.
* Finished.
