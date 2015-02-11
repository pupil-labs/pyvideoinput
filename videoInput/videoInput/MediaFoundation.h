#pragma once

#include <basetsd.h>
#include <minwindef.h>
#include <Unknwnbase.h>


#include "BoxMode.h"
#include "videoInput.h"


struct IMFActivate;
struct IMFMediaSource;
struct IMFPresentationDescriptor;
struct IMFStreamDescriptor;
struct IMFMediaSession;
struct IMFMediaType;
struct IMFAttributes;
struct IMFSampleGrabberSinkCallback;
struct IMFTopology;
struct IMFTopologyNode;
struct IMFMediaEventGenerator;
struct IMFAsyncCallback;

enum MF_TOPOLOGY_TYPE;


class MediaFoundation
{
public:

	static MediaFoundation& getInstance();

	ResultCode::Result getInitializationState();

	ResultCode::Result enumDevices(IMFActivate ***ppDevices, UINT32 &count);
	
	ResultCode::Result readFriendlyName(IMFActivate *pDevice, std::wstring &friendlyName);

	ResultCode::Result readSymbolicLink(IMFActivate *pDevice, std::wstring &symbolicLink);
	
	ResultCode::Result getSource(IMFActivate *pActivate, IMFMediaSource **ppDevice);
	
	ResultCode::Result getSorceBySymbolicLink(wstring symbolicLink, IMFMediaSource **ppDevice);

	ResultCode::Result createPresentationDescriptor(IMFMediaSource *pDevice, IMFPresentationDescriptor **ppPD);

	ResultCode::Result getAmountStreams(IMFPresentationDescriptor * pPD, DWORD &streamCount);

	ResultCode::Result getStreamDescriptorByIndex(DWORD index, IMFPresentationDescriptor *pPD, BOOL &isSelected, IMFStreamDescriptor **pSD);
	
	ResultCode::Result enumMediaType(IMFStreamDescriptor *pSD, std::vector<MediaType> &listMediaType);

	ResultCode::Result selectStream(DWORD streamDescriptorIndex, IMFPresentationDescriptor *pPD);

	ResultCode::Result deselectStream(DWORD streamDescriptorIndex, IMFPresentationDescriptor *pPD);

	ResultCode::Result setCurrentMediaType(IMFStreamDescriptor *pSD, unsigned long  dwFormatIndex);

	ResultCode::Result getCurrentMediaType(IMFStreamDescriptor *pSD, IMFMediaType **ppMediaType);

	ResultCode::Result createSession(IMFMediaSession **ppSession);

	ResultCode::Result createMediaType(IMFMediaType **ppMediaType);

	ResultCode::Result setGUID(IMFAttributes *pMediaType, REFGUID guidKey, REFGUID guidValue);

	ResultCode::Result setUINT32(IMFAttributes *pMediaType, REFGUID guidKey, UINT32 value);

	ResultCode::Result createSampleGrabberSinkActivate(IMFMediaType *pMediaType, IMFSampleGrabberSinkCallback *pSampleGrabberSinkCallback, IMFActivate **pActivate);

	ResultCode::Result createTopology(IMFTopology **ppTopology);

	ResultCode::Result createTopologyNode(MF_TOPOLOGY_TYPE nodeType, IMFTopologyNode **ppNode);

	ResultCode::Result setUnknown(IMFAttributes *pAttributes, REFGUID guidKey, IUnknown *pUnknown);

	ResultCode::Result setObject(IMFTopologyNode *pTopologyNode, IUnknown *pObject);

	ResultCode::Result addNode(IMFTopology *pTopology, IMFTopologyNode *pTopologyNode);

	ResultCode::Result connectOutputNode(IMFTopologyNode *pTopologyNode, IMFTopologyNode *pOutputTopologyNode);

	ResultCode::Result setTopology(IMFMediaSession *pSession, IMFTopology *pTopology);

	ResultCode::Result beginGetEvent(IMFMediaEventGenerator *pMediaEventGenerator, IMFAsyncCallback *pCallback, IUnknown *punkState);


	

private:
	MediaFoundation(void);
	~MediaFoundation(void);
	MediaFoundation(const MediaFoundation&);
	MediaFoundation& operator=(const MediaFoundation&);

	ResultCode::Result state;

CAPTUREMANAGER_BOX

	ResultCode::Result initializeCOM();

	ResultCode::Result initializeMF();

	ResultCode::Result shutdown();

};

