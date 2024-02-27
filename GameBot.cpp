// GameBot.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GameBot.h"

#include "CGameBot.h"
#include "CErrorLog.h"
#include "CModelManager.h"
#include "CString.h"
#include "CWavefrontManager.h"

constexpr auto MAX_LOADSTRING = 100;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CGameBot* m_gameBot;
CErrorLog* m_errorLog;
CModelManager* m_modelManager;
CWavefrontManager* m_wavefrontManager;

class CCommandLine
{
public:

    WCHAR name[32];
    WCHAR ip[17];
    WCHAR port[6];

    CCommandLine()
    {
        wcscpy_s(name, 32, L"Botty");
        wcscpy_s(ip, 17, L"127.0.0.1");
        wcscpy_s(port, 6, L"26105");
    }
};

CCommandLine commandLine;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

    // Parse the command line sent from the Game Manager to obtain the clients account information
    swscanf_s(lpCmdLine, L"%s %s %s", commandLine.name, 32, commandLine.ip, 32, commandLine.port, 6);

    // TODO: Place code here.
    CString* cmdName = new CString(commandLine.name);
    cmdName->Concat(".txt");

    m_errorLog = new CErrorLog(cmdName->GetText());

    delete cmdName;

    CString* cmdLine = new CString(lpCmdLine);
    cmdLine->Concat("\n");

    m_errorLog->WriteError(true, cmdLine->GetText());


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEBOT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEBOT));

    MSG msg = {};

    // Main message loop:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);

            DispatchMessage(&msg);
        }
        else
        {
            while (m_gameBot->m_isActiveUpdate)
            {
            }

            while (m_gameBot->m_isActiveActivity)
            {
            }

            m_gameBot->m_isActiveRender = true;


            m_gameBot->m_isActiveRender = false;
        }
    }


    delete m_gameBot;
    delete m_wavefrontManager;
    delete m_modelManager;
    delete m_errorLog;


    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEBOT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEBOT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   m_modelManager = new CModelManager(m_errorLog);

   m_wavefrontManager = new CWavefrontManager(m_errorLog);

   m_gameBot = new CGameBot(m_errorLog, nullptr, nullptr, m_modelManager, m_wavefrontManager);

   m_gameBot->m_hWnd = hWnd;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

                return 0;

                break;
            }
            case IDM_EXIT:
            {
                DestroyWindow(hWnd);

                return 0;

                break;
            }
            case IDM_CAMERA:
            {

                return 0;

                break;
            }
            case IDM_CONNECT:
            {
                CString* name = new CString(commandLine.name);

                m_gameBot->SetLogin(name->GetText());

                CString* ip = new CString(commandLine.ip);
                CString* port = new CString(commandLine.port);

                ip->Concat(" ");
                ip->Concat(port->GetText());

                m_gameBot->Connect(ip->GetText());

                delete port;
                delete ip;
                delete name;

                return 0;

                break;
            }
            case IDM_DISCONNECT:
            {
                m_gameBot->Disconnect();

                m_gameBot->DestroyEnvironment();

                return 0;

                break;
            }
            case IDM_JOIN:
            {
                CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_JOIN,
                    nullptr, 0,
                    (void*)m_gameBot->m_localClient);

                m_gameBot->Send(n);

                delete n;

                return 0;

                break;
            }
            case IDM_LOAD:
            {
                CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_LOAD_ENVIRONMENT,
                    (void*)"terrain\\vertices.txt", 20,
                    (void*)m_gameBot->m_localClient);

                m_gameBot->Send(n);

                delete n;

                return 0;

                break;
            }

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
