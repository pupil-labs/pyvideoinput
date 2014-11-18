cimport videoInput as vid

cdef class VideoInput:
    cdef vid.videoInput *vi
    
    #def test(self):
    #    return self.vi.listDevices()
    #    #self.vi = new vid.videoInput()
    
    def getDeviceList(self):
        self.vi.listDevices()
        
    #def VideoInput(self):
    #    self.vi = new vid.videoInput()
    
    def test(self):
        print "test"


if __name__ == '__main__':
    cap = VideoInput()
    print vid.VI_VERSION
    #print cap.test()