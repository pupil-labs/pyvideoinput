cdef extern from 'wchar.h':
    ctypedef struct WCHAR:
        pass

# http://www.codeproject.com/Tips/559437/Capturing-Video-from-Web-camera-on-Windows-and-by
cdef extern from '<videoInput.h>':
    ctypedef struct MediaType:
        pass

    ctypedef struct CamParametrs:
        pass
        
    cdef cppclass videoInput:
        videoInput() except +
        
        @staticmethod
        videoInput getInstance()
        
        # Closing video device with deviceID
        void closeDevice(unsigned int deviceID);
        
        # Setting callback function for emergency events(for example: removing video device with deviceID) with userData
        #void setEmergencyStopEvent(unsigned int deviceID, void *userData, void(*func)(int, void *));
    
        # Closing all devices
        void closeAllDevices();
    
        # Getting of parametrs of video device with deviceID
        CamParametrs getParametrs(unsigned int deviceID);
    
        # Setting of parametrs of video device with deviceID
        void setParametrs(unsigned int deviceID, CamParametrs parametrs);
    
        # Getting numbers of existence videodevices with listing in consol
        unsigned int listDevices(bint silent = false);
            
        # Getting numbers of formats, which are supported by videodevice with deviceID
        unsigned int getCountFormats(unsigned int deviceID);
    
        # Getting width of image, which is getting from videodevice with deviceID
        unsigned int getWidth(unsigned int deviceID);
    
        # Getting height of image, which is getting from videodevice with deviceID
        unsigned int getHeight(unsigned int deviceID);
    
        # Getting name of videodevice with deviceID
        WCHAR *getNameVideoDevice(unsigned int deviceID);
        
        # Getting interface MediaSource for Media Foundation from videodevice with deviceID
        #IMFMediaSource *getMediaSource(unsigned int deviceID);
        
        # Getting format with id, which is supported by videodevice with deviceID 
        MediaType getFormat(unsigned int deviceID, unsigned int id);
    
        # Checking of existence of the suitable video devices
        bint isDevicesAcceable();
    
        # Checking of using the videodevice with deviceID
        bint isDeviceSetup(unsigned int deviceID);
    
        # Checking of using MediaSource from videodevice with deviceID
        #bint isDeviceMediaSource(unsigned int deviceID);
        
        # Checking of using Raw Data of pixels from videodevice with deviceID
        bint isDeviceRawDataSource(unsigned int deviceID);
    
        # Setting of the state of outprinting info in consol
        void setVerbose(bint state);
        
        # Initialization of video device with deviceID by media type with id
        bint setupDevice(unsigned int deviceID, unsigned int id = 0);
    
        # Initialization of video device with deviceID by wisth w, height h and fps idealFramerate
        bint setupDevice(unsigned int deviceID, unsigned int w, unsigned int h, unsigned int idealFramerate = 30);
    
        # Checking of recivig of new frame from video device with deviceID 
        bint isFrameNew(unsigned int deviceID);
    
        # Writing of Raw Data pixels from video device with deviceID with correction of RedAndBlue flipping flipRedAndBlue and vertical flipping flipImage
        bint getPixels(unsigned int deviceID, unsigned char * pixels, bint flipRedAndBlue = false, bint flipImage = false);