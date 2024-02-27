#include "CWinsockError.h"

/*
*/
CWinsockError::CWinsockError()
{
	memset(this, 0x00, sizeof(CWinsockError));

	m_errors = new CLinkList<CWinsockErrorItem>();

	CWinsockErrorItem* wei = new CWinsockErrorItem(6, "WSA_INVALID_HANDLE", "Specified event object handle is invalid.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(87, "WSA_INVALID_PARAMETER", "One or more parameters are invalid.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(995, "WSA_OPERATION_ABORTED", "Overlapped operation aborted.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(996, "WSA_IO_INCOMPLETE", "Overlapped I/O event object not in signaled state.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(997, "WSA_IO_PENDING", "Overlapped operations will complete later.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10004, "WSAEINTR", "Interrupted function call.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10009, "WSAEBADF", "File handle is not valid.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10013, "WSAEACCES", "Permission denied.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10014, "WSAEFAULT", "Bad address.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10022, "WSAEINVAL", "Invalid argument.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10024, "WSAEMFILE", "Too many open files.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10035, "WSAEWOULDBLOCK", "Resource temporarily unavailable.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10036, "WSAEINPROGRESS", "Operation now in progress.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10037, "WSAEALREADY", "Operation already in progress.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10038, "WSAENOTSOCK", "Socket operation on nonsocket.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10039, "WSAEDESTADDRREQ", "Destination address required.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10040, "WSAEMSGSIZE", "Message too long.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10041, "WSAEPROTOTYPE", "Protocol wrong type for socket.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10042, "WSAENOPROTOOPT", "Bad protocol option.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10043, "WSAEPROTONOSUPPORT", "Protocol not supported.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10044, "WSAESOCKTNOSUPPORT", "Socket type not supported.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10045, "WSAEOPNOTSUPP", "Operation not supported.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10046, "WSAEPFNOSUPPORT", "Protocol family not supported.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10047, "WSAEAFNOSUPPORT", "Address family not supported by protocol family.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10048, "WSAEADDRINUSE", "Address already in use.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10049, "WSAEADDRNOTAVAIL", "Cannot assign requested address.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10050, "WSAENETDOWN", "Network is down.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10051, "WSAENETUNREACH", "Network is unreachable.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10052, "WSAENETRESET", "Network dropped connection on reset.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10053, "WSAECONNABORTED", "Software caused connection abort.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10054, "WSAECONNRESET", "Connection reset by peer.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10055, "WSAENOBUFS", "No buffer space available.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10056, "WSAEISCONN", "Socket is already connected.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10057, "WSAENOTCONN", "Socket is not connected.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10058, "WSAESHUTDOWN", "Cannot send after socket shutdown.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10059, "WSAETOOMANYREFS", "Too many references.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10060, "WSAETIMEDOUT", "Connection timed out.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10061, "WSAECONNREFUSED", "Connection refused.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10062, "WSAELOOP", "Cannot translate name.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10063, "WSAENAMETOOLONG", "Name too long.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10064, "WSAEHOSTDOWN", "Host is down.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10065, "WSAEHOSTUNREACH", "No route to host.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10066, "WSAENOTEMPTY", "Directory not empty.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10067, "WSAEPROCLIM", "Too many processes.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10068, "WSAEUSERS", "User quota exceeded.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10069, "WSAEDQUOT", "Disk quota exceeded.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10070, "WSAESTALE", "Stale file handle reference.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10071, "WSAEREMOTE", "Item is remote.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10091, "WSASYSNOTREADY", "Network subsystem is unavailable.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10092, "WSAVERNOTSUPPORTED", "Winsock.dll version out of range.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10093, "WSANOTINITIALISED", "Successful WSAStartup not yet performed.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10101, "WSAEDISCON", "Graceful shutdown in progress.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10102, "WSAENOMORE", "No more results.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10103, "WSAECANCELLED", "Call has been canceled.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10104, "WSAEINVALIDPROCTABLE", "Procedure call table is invalid.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10105, "WSAEINVALIDPROVIDER", "Service provider is invalid.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10106, "WSAEPROVIDERFAILEDINIT", "Service provider failed to initialize.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10107, "WSASYSCALLFAILURE", "System call failure.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10108, "WSASERVICE_NOT_FOUND", "Service not found.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10109, "WSATYPE_NOT_FOUND", "Class type not found.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10110, "WSA_E_NO_MORE", "No more results.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10111, "WSA_E_CANCELLED", "Call was canceled.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(10112, "WSAEREFUSED", "Database query was refused.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11001, "WSAHOST_NOT_FOUND", "Host not found.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11002, "WSATRY_AGAIN", "Nonauthoritative host not found.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11003, "WSANO_RECOVERY", "This is a nonrecoverable error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11004, "WSANO_DATA", "Valid name, no data record of requested type.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11005, "WSA_QOS_RECEIVERS", "QoS receivers.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11006, "WSA_QOS_SENDERS", "QoS senders.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11007, "WSA_QOS_NO_SENDERS", "No QoS senders.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11008, "WSA_QOS_NO_RECEIVERS", "QoS no receivers.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11009, "WSA_QOS_REQUEST_CONFIRMED", "QoS request confirmed.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11010, "WSA_QOS_ADMISSION_FAILURE", "QoS admission error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11011, "WSA_QOS_POLICY_FAILURE", "QoS policy failure.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11012, "WSA_QOS_BAD_STYLE", "QoS bad style.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11013, "WSA_QOS_BAD_OBJECT", "QoS bad object.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11014, "WSA_QOS_TRAFFIC_CTRL_ERROR", "QoS traffic control error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11015, "WSA_QOS_GENERIC_ERROR", "QoS generic error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11016, "WSA_QOS_ESERVICETYPE", "QoS service type error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11017, "WSA_QOS_EFLOWSPEC", "QoS flowspec error.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11018, "WSA_QOS_EPROVSPECBUF", "Invalid QoS provider buffer.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11019, "WSA_QOS_EFILTERSTYLE", "Invalid QoS filter style.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11020, "WSA_QOS_EFILTERTYPE", "Invalid QoS filter type.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11021, "WSA_QOS_EFILTERCOUNT", "Incorrect QoS filter count.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11022, "WSA_QOS_EOBJLENGTH", "Invalid QoS object length.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11023, "WSA_QOS_EFLOWCOUNT", "Incorrect QoS flow count.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11024, "WSA_QOS_EUNKOWNPSOBJ", "Unrecognized QoS object.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11025, "WSA_QOS_EPOLICYOBJ", "Invalid QoS policy object.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11026, "WSA_QOS_EFLOWDESC", "Invalid QoS flow descriptor.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11027, "WSA_QOS_EPSFLOWSPEC", "Invalid QoS provider - specific flowspec.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11028, "WSA_QOS_EPSFILTERSPEC", "Invalid QoS provider - specific filterspec.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11029, "WSA_QOS_ESDMODEOBJ", "Invalid QoS shape discard mode object.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11030, "WSA_QOS_ESHAPERATEOBJ", "Invalid QoS shaping rate object.");
	CWinsockError::AddItem(wei);

	wei = new CWinsockErrorItem(11031, "WSA_QOS_RESERVED_PETYPE", "Reserved policy QoS element type.");
	CWinsockError::AddItem(wei);
}

/*
*/
CWinsockError::~CWinsockError()
{
	delete m_errors;
}

/*
*/
void CWinsockError::AddItem(CWinsockErrorItem* item)
{
	m_errors->Add(item, item->m_nbr);
}

/*
*/
CWinsockErrorItem* CWinsockError::GetError(int error)
{
	CWinsockErrorItem* item = nullptr;

	CLinkListNode<CWinsockErrorItem>* lln = m_errors->Search(error);

	if (lln != nullptr)
	{
		item = lln->m_object;
	}

	return item;
}