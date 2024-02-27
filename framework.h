// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files

#include "resource.h"


#define _WINSOCKAPI_

#include <time.h>
#include <comdef.h>
#include <iphlpapi.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <mmsystem.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wininet.h>
#include <ExDisp.h>
#include <MsXml6.h>
#include <atlbase.h>

#include <wrl.h>

#include <wrl/client.h>

#include <CommCtrl.h>

#include <d3d12.h>
//#include "d3dx12.h"
#include <D3d12SDKLayers.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <DirectXMath.h>

#include <pix.h>

#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <dwrite.h>

#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3DAudio.h>

#include <xapofx.h>

#include <Mmdeviceapi.h>

#include <Functiondiscoverykeys_devpkey.h>

using namespace DirectX;

using namespace Microsoft::WRL;

#pragma comment(lib, "msxml6.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "Ws2_32.lib")

#pragma once

constexpr auto _PiDiv180 = 0.0174532925f;

constexpr auto _180DivPi = 57.295779513082320876798154814105f;