cimport videoInput

cdef class VideoInput:
    cdef videoInput.videoInput *thisptr
    
    def __cinit__(self):
        self.thisptr[0] = videoInput.getInstance()
        
    def __dealloc__(self):
        del self.thisptr
        
    def fun(self):
        #&vid.videoInput.getInstance()
        print "test1"

    def test(self):
        print "test2" 


cap = VideoInput()
print cap.test()