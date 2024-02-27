#include "CDXGIError.h"

/*
*/
CDXGIError::CDXGIError()
{
	memset(this, 0x00, sizeof(CDXGIError));

	m_defaultError = new CDXGIErrorItem(0x00000000, "DXGI_UKNOWN_ERROR",
		"There is something wrong with the parameters of the call.");

	m_errors = new CLinkList<CDXGIErrorItem>();

	CDXGIErrorItem* dxgiei = new CDXGIErrorItem(0x887A002B, "DXGI_ERROR_ACCESS_DENIED",
		"You tried to use a resource to which you did not have the required access privileges.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0026, "DXGI_ERROR_ACCESS_LOST",
		"The desktop duplication interface is invalid.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0036, "DXGI_ERROR_ALREADY_EXISTS",
		"The desired element already exists.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A002A, "DXGI_ERROR_CANNOT_PROTECT_CONTENT",
		"DXGI can't provide content protection on the swap chain.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0006, "DXGI_ERROR_DEVICE_HUNG",
		"The application's device failed due to badly formed commands sent by the application.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0005, "DXGI_ERROR_DEVICE_REMOVED",
		"The video card has been physically removed from the system, or a driver upgrade for the video card has occurred.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0007, "DXGI_ERROR_DEVICE_RESET",
		"The device failed due to a badly formed command.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0020, "DXGI_ERROR_DRIVER_INTERNAL_ERROR",
		"The driver encountered a problemand was put into the device removed state.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A000B, "DXGI_ERROR_FRAME_STATISTICS_DISJOINT",
		"An event(for example, a power cycle) interrupted the gathering of presentation statistics.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A000C, "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE",
		"The application attempted to acquire exclusive ownership but failed because some other application or device already acquired ownership.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0001, "DXGI_ERROR_INVALID_CALL",
		"The application provided invalid parameter data; this must be debugged and fixed before the application is released.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0003, "DXGI_ERROR_MORE_DATA",
		"The buffer supplied by the application is not big enough to hold the requested data.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A002C, "DXGI_ERROR_NAME_ALREADY_EXISTS",
		"The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0021, "DXGI_ERROR_NONEXCLUSIVE",
		"A global counter resource is in use, and the Direct3D device can't currently use the counter resource.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0022, "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE",
		"The resource or request is not currently available, but it might become available later.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0002, "DXGI_ERROR_NOT_FOUND",
		"When calling GetPrivateData, the GUID passed in is not recognized as one previously passed to SetPrivateData or SetPrivateDataInterface.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0023, "DXGI_ERROR_REMOTE_CE_TO_CLIENT_DISCONNECTED", "Reserved");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0024, "DXGI_ERROR_REMOTE_OUTOFMEMORY", "Reserved");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0029, "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE",
		"The DXGI output(monitor) to which the swap chain content was restricted is now disconnected or changed.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A002D, "DXGI_ERROR_SDK_COMPONENT_MISSING",
		"The operation depends on an SDK component that is missing or mismatched.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0028, "DXGI_ERROR_SESSION_DISCONNECTED",
		"The Remote Desktop Services session is currently disconnected.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0004, "DXGI_ERROR_UNSUPPORTED",
		"The requested functionality is not supported by the device or the driver.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A0027, "DXGI_ERROR_WAIT_TIMEOUT",
		"The time-out interval elapsed before the next desktop frame was available.");
	CDXGIError::AddItem(dxgiei);

	dxgiei = new CDXGIErrorItem(0x887A000A, "DXGI_ERROR_WAS_STILL_DRAWING",
		"The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.");
	CDXGIError::AddItem(dxgiei);
}

/*
*/
CDXGIError::~CDXGIError()
{
	delete m_defaultError;

	delete m_errors;
}

/*
*/
void CDXGIError::AddItem(CDXGIErrorItem* item)
{
	m_errors->Add(item, item->m_nbr);
}

/*
*/
CDXGIErrorItem* CDXGIError::GetError(int error)
{
	CLinkListNode<CDXGIErrorItem>* lln = m_errors->Search(error);

	if (lln)
	{
		return lln->m_object;
	}

	return m_defaultError;
}