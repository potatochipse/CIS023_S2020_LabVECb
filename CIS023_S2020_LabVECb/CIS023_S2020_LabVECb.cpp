// Module:		CIS023_S2020_LabVECb.cpp
// Author:		Miguel Antonio Logarta
// Date:		April 21, 2020
// Purpose:		Demonstrate and understanding of vectors
//				Create a vector node using input data.
//				Append a node to a vector.
//


#include "framework.h"
#include "CIS023_S2020_LabVECb.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

string strDataFile = "CIS023_S2020_LabVECb_Data.csv"; // input data filespec

HWND hWnd_Input;								// input field
HWND hWnd_Name;									// output fields
HWND hWnd_Dec;									// .
HWND hWnd_Hex;									// .
HWND hWnd_Oct;									// .
HWND hWnd_Bin;									// .
HWND hWnd_Save;									// Button

HWND hWnd_ListBox;								// handle to listbox

vector<Radix> vNode;							// vector list of Radix nodes

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void LoadFile();								// load file into linked list
void LoadListbox(Radix*);						// add an node to the listbox

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIS023S2020LABVECB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIS023S2020LABVECB));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIS023S2020LABVECB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIS023S2020LABVECB);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 325, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
		case BN_CLICKED:	// capture button click
			if (LOWORD(lParam) == (WORD)hWnd_Save)
			{
				// Note: i have no idea what I'm doing
				bool bNotEmpty = false;					// Bool to check if all fields are empty

				// 5 edit boxes, 5 tchars to take in, 5 strings to store those converted tchars
				HWND hWnd_array[5] = { hWnd_Name, 
					hWnd_Dec, 
					hWnd_Hex, 
					hWnd_Oct, 
					hWnd_Bin };
				TCHAR szIn[5][TCHAR_SIZE];
				string strIn[5];

				// Take in user input
				for (int i = 0; i < 5; i++)
				{
					GetWindowText(hWnd_array[i], szIn[i], TCHAR_SIZE);		// Get tchar from current edit box
					if (szIn[i][0] != '\0')									// If the tchar is not empty
					{
						bNotEmpty = true;									// That means that not all the fields are empty
						TCHARtoString(szIn[i], &strIn[i]);					// Get current tchar and convert it to string
					}
				}

				// If the user typed something in
				if (bNotEmpty) {
					Radix newNode(strIn[0],					// Name
						strIn[1],							// Decimal
						strIn[2],							// Hexadecimal
						strIn[3],							// Octaldecimal
						strIn[4]);							// Binary

					vNode.push_back(newNode);				// Add to the end of the vector
					for (auto& vObj : vNode)				// loop through list
						LoadListbox(&vObj);					// Show output
				}
			}
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
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

	case WM_CREATE:
	{
		// label
		CreateWindow(TEXT("STATIC"),
			TEXT("List of values from data file"),
			WS_VISIBLE | WS_CHILD,
			400, 10, 340, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("Name                    Dec    Hex  Oct     Bin"),
			WS_VISIBLE | WS_CHILD,
			400, 40, 340, 20, hWnd, NULL, NULL, NULL);

		// listbox
		hWnd_ListBox = CreateWindow(TEXT("LISTBOX"),
			NULL,
			WS_VISIBLE | WS_CHILD |
			WS_VSCROLL | WS_BORDER,
			400, 60, 350, 200, hWnd, NULL, NULL, NULL);

		// set listbox font to fixed width
		HFONT hFont = (HFONT)GetStockObject(OEM_FIXED_FONT);
		SendMessage(hWnd_ListBox, WM_SETFONT, WPARAM(hFont), NULL);


		// prompt
		CreateWindow(TEXT("STATIC"),
			TEXT("Enter a set of new values"),
			WS_VISIBLE | WS_CHILD,
			10, 10, 340, 20, hWnd, NULL, NULL, NULL);

		// labels
		CreateWindow(TEXT("STATIC"),
			TEXT("Name:"),
			WS_VISIBLE | WS_CHILD,
			10, 50, 110, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"),
			TEXT("Decimal:"),
			WS_VISIBLE | WS_CHILD,
			10, 80, 110, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"),
			TEXT("Hexadecimal:"),
			WS_VISIBLE | WS_CHILD,
			10, 110, 110, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"),
			TEXT("Octal:"),
			WS_VISIBLE | WS_CHILD,
			10, 140, 110, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"),
			TEXT("Binary:"),
			WS_VISIBLE | WS_CHILD,
			10, 170, 110, 20, hWnd, NULL, NULL, NULL);


		// output static controls
		hWnd_Name = CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | WS_CHILD,
			130, 50, 170, 20, hWnd, NULL, NULL, NULL);
		hWnd_Dec = CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | WS_CHILD,
			130, 80, 170, 20, hWnd, NULL, NULL, NULL);
		hWnd_Hex = CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | WS_CHILD,
			130, 110, 170, 20, hWnd, NULL, NULL, NULL);
		hWnd_Oct = CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | WS_CHILD,
			130, 140, 170, 20, hWnd, NULL, NULL, NULL);
		hWnd_Bin = CreateWindow(TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | WS_CHILD,
			130, 170, 170, 20, hWnd, NULL, NULL, NULL);

		// find button
		hWnd_Save = CreateWindow(TEXT("BUTTON"),
			TEXT("Save"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			150, 220, 70, 30, hWnd, NULL, NULL, NULL);


		// put your name in the window title
		SetWindowText(hWnd, TEXT("CIS 023, Lab 17b, Miguel Antonio Logarta"));

		// load file into linked list
		LoadFile();

	}
	break;



	case WM_DESTROY:

		vNode.empty();						// empty vector list

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


// load the file into the linked list
void LoadFile()
{
	string strName = "";								// temps to hold file data
	string strDec = "";
	string strHex = "";
	string strOct = "";
	string strBin = "";

	ifstream fInput;									// file pointer

	try
	{
		fInput.open(strDataFile);						// open file

		if (!fInput)
			throw 1;

		while (fInput.good())
		{
			// read a line from the file
			getline(fInput, strName, ',');
			getline(fInput, strDec, ',');
			getline(fInput, strHex, ',');
			getline(fInput, strOct, ',');
			getline(fInput, strBin, '\n');

			// create new node
			Radix newNode(strName, strDec, strHex, strOct, strBin);

			// append node to the end of the list 
			vNode.push_back(newNode);

		}

		fInput.close();										// ALWAYS remember to close the file

		// file loaded into vector, now load vector into listbox
		for (auto& vObj : vNode)						// loop through list
			LoadListbox(&vObj);

		// turn off focus on a line in the listbox
		SendMessage(hWnd_ListBox, LB_SETCURSEL, 0, NULL);
		SendMessage(hWnd_ListBox, LB_SETCURSEL, -1, NULL);

		EnableWindow(hWnd_Save, true);						// enable search button
	}
	catch (int)
	{
		string strMsg = "Failed to load file '" +			// error message
			strDataFile + "'";
		TCHAR szMsg[TCHAR_SIZE];							// error message
		StringtoTCHAR(&strMsg, szMsg);

		MessageBox(NULL,									// message box
			szMsg,
			TEXT("File Error"),
			MB_ICONERROR);

		EnableWindow(hWnd_Save, false);						// disable search

	}
}

// add an node to the listbox
void LoadListbox(Radix* node)
{
	TCHAR szOut[TCHAR_SIZE];								// TCHAR to be added to the listbox
	string strValue;										// data from node
	string strWork;											// work string for formatting

	TCHARtoString(node->GetName(), &strWork);				// name at left edge
	strWork.append(15 - strWork.length(), ' ');				// 15 char column

	TCHARtoString(node->GetDec(), &strValue);				// get decimal value
	strWork += strValue;
	strWork.append(5 - strValue.length(), ' ');				// 5 char column

	TCHARtoString(node->GetHex(), &strValue);				// get hexadecimal value
	strWork += strValue;
	strWork.append(4 - strValue.length(), ' ');				// 4 char column

	TCHARtoString(node->GetOct(), &strValue);				// get octal value
	strWork += strValue;
	strWork.append(5 - strValue.length(), ' ');				// 5 char column

	TCHARtoString(node->GetBin(), &strValue);				// get binary value
	strWork += strValue;

	StringtoTCHAR(&strWork, szOut);							// convert work string to TCAHR for output

	// add string to the listbox
	SendMessage(hWnd_ListBox, LB_ADDSTRING, NULL, LPARAM(szOut));

	// set focus to end of the listbox
	SendMessage(hWnd_ListBox,
		LB_SETCURSEL,
		SendMessage(hWnd_ListBox, LB_GETCOUNT, NULL, NULL) - 1,
		NULL);


}
