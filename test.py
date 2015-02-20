import videoInput as vi
import numpy as np

print vi.VERSION

print "LOADED VIDEOINPUT LIB"

vin = vi.videoInput_getInstance()
assert isinstance(vin, vi.videoInput)

devices = vi.DeviceList()
vin.getListOfDevices(devices)
for d in devices:
    assert isinstance(d, vi.Device)
    print d.friendlyName, d.symbolicName
    dev = d
dev = devices[1]
assert isinstance(dev, vi.Device)
print "LOADED CAPTURE DEVICES" 

deviceSettings = vi.DeviceSettings()
deviceSettings.symbolicLink = dev.symbolicName
deviceSettings.indexStream = 0
deviceSettings.indexMediaType = 195
captureSettings = vi.CaptureSettings()
captureSettings.readMode = vi.ReadMode.SYNC
captureSettings.videoFormat = vi.CaptureVideoFormat.RGB24

stream = dev.listStream[0]
assert isinstance(stream, vi.Stream)
mediaType = stream.listMediaType[195]
for mt in stream.listMediaType:
    print mt.width, mt.height, mt.MF_MT_FRAME_RATE

assert isinstance(mediaType, vi.MediaType)
frame = np.zeros((mediaType.height, mediaType.width,3), dtype=np.uint8)
frame.shape = (mediaType.height* mediaType.width*3)

res = vin.setupDevice(deviceSettings, captureSettings)
if res != 0:
    exit
print "SUCCESSFULLY SETUP DEVICE"

readSetting = vi.ReadSetting()
readSetting.symbolicLink = deviceSettings.symbolicLink
readSetting.setNumpyArray(frame)

frame.shape = (mediaType.height, mediaType.width,-1)


import cv2
from time import time
ts = time()
while 1:
    now =time()
    dt = ts-now
    ts = now
    try:
        print 1/dt
    except:
        pass
    res = vin.readPixels(readSetting)
    cv2.imshow('image',frame[:,:,:])
    if cv2.waitKey(1) == 27:
        break
    
res = vin.closeDevice(deviceSettings)
if res != 0:
    exit
print "SUCCESSFULLY CLOSED DEVICE"

