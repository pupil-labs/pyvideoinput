cdef extern from 'wchar.h':
    ctypedef struct WCHAR:
        pass

cdef extern from 'Windows.h':
    ctypedef struct GUID:
        pass
    
DEF VI_VERSION  =   0.200
DEF VI_MAX_CAMERAS = 20
DEF VI_NUM_TYPES  =  19
DEF VI_NUM_FORMATS = 18

DEF VI_COMPOSITE= 0
DEF VI_S_VIDEO =  1
DEF VI_TUNER   =  2
DEF VI_USB     =  3
DEF VI_1394    =  4

DEF VI_NTSC_M   = 0
DEF VI_PAL_B  =  1
DEF VI_PAL_D  =  2
DEF VI_PAL_G  =  3
DEF VI_PAL_H  =  4
DEF VI_PAL_I  =  5
DEF VI_PAL_M  =  6
DEF VI_PAL_N  =  7
DEF VI_PAL_NC =   8
DEF VI_SECAM_B =   9
DEF VI_SECAM_D =   10
DEF VI_SECAM_G =   11
DEF VI_SECAM_H =   12
DEF VI_SECAM_K =   13
DEF VI_SECAM_K1  =  14
DEF VI_SECAM_L  =  15
DEF VI_NTSC_M_J  =  16
DEF VI_NTSC_433  =  17

DEF VI_MEDIASUBTYPE_RGB24   =0
DEF VI_MEDIASUBTYPE_RGB32   =1
DEF VI_MEDIASUBTYPE_RGB555  =2
DEF VI_MEDIASUBTYPE_RGB565  =3
DEF VI_MEDIASUBTYPE_YUY2    =4
DEF VI_MEDIASUBTYPE_YVYU    =5
DEF VI_MEDIASUBTYPE_YUYV    =6
DEF VI_MEDIASUBTYPE_IYUV    =7
DEF VI_MEDIASUBTYPE_UYVY    =8
DEF VI_MEDIASUBTYPE_YV12    =9
DEF VI_MEDIASUBTYPE_YVU9    =10
DEF VI_MEDIASUBTYPE_Y411    =11
DEF VI_MEDIASUBTYPE_Y41P    =12
DEF VI_MEDIASUBTYPE_Y211    =13
DEF VI_MEDIASUBTYPE_AYUV    =14
DEF VI_MEDIASUBTYPE_Y800    =15
DEF VI_MEDIASUBTYPE_Y8      =16
DEF VI_MEDIASUBTYPE_GREY    =17
DEF VI_MEDIASUBTYPE_MJPG    =18
    
from libcpp.vector cimport vector
from libcpp.string cimport string
cdef extern from '<videoInput/videoInput.h>':
    ctypedef struct ICaptureGraphBuilder2:
        pass
    ctypedef struct IGraphBuilder: 
        pass
    ctypedef struct IBaseFilter: 
        pass
    ctypedef struct IAMCrossbar: 
        pass
    ctypedef struct IMediaControl:
        pass
    ctypedef struct ISampleGrabber: 
        pass
    ctypedef struct IMediaEventEx: 
        pass
    ctypedef struct IAMStreamConfig: 
        pass
    #ctypedef struct _AMMediaType: 
    #    pass
    ctypedef struct AM_MEDIA_TYPE: 
        pass
    
    cdef cppclass SampleGrabberCallback:
        pass
    
    cdef cppclass videoDevice:
        videoDevice() except +
        void setSize(int w, int h)
        void NukeDownstream(IBaseFilter *pBF)
        void destroyGraph()

        int videoSize
        int width
        int height
        int tryWidth
        int tryHeight
        
        ICaptureGraphBuilder2 *pCaptureGraph
        IGraphBuilder *pGraph        
        IMediaControl *pControl                
        IBaseFilter *pVideoInputFilter         
        IBaseFilter *pGrabberF
        IBaseFilter * pDestFilter
        IAMStreamConfig *streamConf
        ISampleGrabber * pGrabber              
        AM_MEDIA_TYPE * pAmMediaType

        IMediaEventEx * pMediaEvent
        
        GUID videoType
        long formatType
        
        SampleGrabberCallback * sgCallback

        bint tryDiffSize
        bint useCrossbar
        bint readyToCapture
        bint sizeSet
        bint setupStarted
        bint specificFormat
        bint autoReconnect
        int  nFramesForReconnect
        unsigned long nFramesRunning
        int  connection
        int     storeConn
        int  myID
        long requestedFrameTime 

        char     nDeviceName[255]
        WCHAR     wDeviceName[255]
        
        unsigned char * pixels
        char * pBuffer
        
    cdef cppclass videoInput:
        videoInput() except +
        
        @staticmethod
        void setVerbose(bint _verbose)
        @staticmethod
        void setComMultiThreaded(bint bMulti)
        @staticmethod
        int listDevices(bint silent = false)
        @staticmethod
        vector[string] getDeviceList() 
        @staticmethod
        char * getDeviceName(int deviceID)
        @staticmethod
        int getDeviceIDFromName(char * name)
        
        void setUseCallback(bint)
        
        void setIdealFramerate(int deviceID, int idealFramerate)

        void setAutoReconnectOnFreeze(int deviceNumber, bint doReconnect, int numMissedFramesBeforeReconnect)

        bint setupDevice(int deviceID)
        bint setupDevice(int deviceID, int w, int h)

        bint setupDevice(int deviceID, int connection)
        bint setupDevice(int deviceID, int w, int h, int connection)

        bint setFormat(int deviceNumber, int format)
        void setRequestedMediaSubType(int mediatype)

        bint isFrameNew(int deviceID)

        bint isDeviceSetup(int deviceID)

        unsigned char * getPixels(int deviceID, bint flipRedAndBlue = true, bint flipImage = false)

        bint getPixels(int id, unsigned char * pixels, bint flipRedAndBlue = true, bint flipImage = false)

        void showSettingsWindow(int deviceID)

        bint setVideoSettingFilter(int deviceID, long Property, long lValue, long Flags = NULL, bint useDefaultValue = false)
        bint setVideoSettingFilterPct(int deviceID, long Property, float pctValue, long Flags = NULL)
        bint getVideoSettingFilter(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue)

        bint setVideoSettingCamera(int deviceID, long Property, long lValue, long Flags = NULL, bint useDefaultValue = false)
        bint setVideoSettingCameraPct(int deviceID, long Property, float pctValue, long Flags = NULL)
        bint getVideoSettingCamera(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue)

        int  getWidth(int deviceID)
        int  getHeight(int deviceID)
        int  getSize(int deviceID)

        void stopDevice(int deviceID)

        bint restartDevice(int deviceID)

        int  devicesFound

        long propBrightness
        long propContrast
        long propHue
        long propSaturation
        long propSharpness
        long propGamma
        long propColorEnable
        long propWhiteBalance
        long propBacklightCompensation
        long propGain

        long propPan
        long propTilt
        long propRoll
        long propZoom
        long propExposure
        long propIris
        long propFocus
        
cdef videoInput *vidIn = new videoInput()