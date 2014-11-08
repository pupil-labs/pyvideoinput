cimport videoInput as vid

cdef class VideoInput:
    cdef vid.videoInput *vi
    
    #def VideoInput(self):
    #    self.vi = new vid.videoInput()
