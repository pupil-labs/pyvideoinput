%module videoInput
%{
	/* Includes the header in the wrapper code */
	#define SWIG_FILE_WITH_INIT
	#include "videoInput.h"
%}

%include "numpy.i"

%init %{
	import_array();
%}

%apply ( unsigned char* INPLACE_ARRAY1, int DIM1 ) {(unsigned char* ndarray, int dim)}

%include "std_vector.i"
%include "std_wstring.i"

namespace std {
	%template(DeviceList) vector<Device>;
	%template(StreamList) vector<Stream>;
	%template(MediaTypeList) vector<MediaType>;
}

%constant double VERSION = 0.1;

/* Parse the header file to generate wrappers */
%include "videoInput.h"