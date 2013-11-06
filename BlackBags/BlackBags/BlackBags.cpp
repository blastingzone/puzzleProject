﻿// BlackBags.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BlackBags.h"
#include "GameMap.h"
#include "Renderer.h"
#include "Logic.h"
#include <windowsx.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LOADSTRING 100

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__ )
#endif

// Global Variables:
HINSTANCE		hInst;									// current instance
TCHAR			szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR			szWindowClass[MAX_LOADSTRING];			// the main window class name
//CRenderer*		renderer;								// Renderer
//CGameMap*		gameMap;
CLogic*			g_Logic;
RECT			g_ClientRect;								//window client size

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// 콘솔창 생성
	//AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "wb", stdout);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG		msg;
	HACCEL	hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BlackBags, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BlackBags));

	//int a = 0;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//printf("COUNT : %d\n", a);
		
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CRenderer::GetInstance()->ReleaseInstance();
	delete g_Logic;

	FreeConsole();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BlackBags));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BlackBags);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	if (!CRenderer::GetInstance()->Init(hWnd) )
	{
		SendMessage(hWnd, WM_DESTROY, NULL, NULL);
	}
	else 
	{
		g_Logic = new CLogic();
		g_Logic->Init();
	}
	
	//update window하기 전에 렌더러와 맵을 생성하지 않으면 null pointer 참조 연산 발생 가능성 있음
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//client 영역 크기 설정
		SetRect(&g_ClientRect, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
		
		//윈도우 크기를 계산
		AdjustWindowRect(&g_ClientRect, WS_OVERLAPPEDWINDOW, FALSE);
		
		//윈도우 크기를 바꿔주고 클라이언트 영역을 새로 그려준다.
		MoveWindow(hWnd, 0, 0,
			g_ClientRect.right - g_ClientRect.left,
			g_ClientRect.bottom - g_ClientRect.top,
			TRUE); 
		break;
	case WM_LBUTTONDOWN:

		// 마우스 좌표 받아오는 부분
		Coordinate mouseCoordinate;
		mouseCoordinate.m_PosX = GET_X_LPARAM(lParam);
		mouseCoordinate.m_PosY = GET_Y_LPARAM(lParam);
		
		//여기에서 라인을 그을 수 있는 자리인지 확인해서 가능하다면 타이머 리셋하고 맵 업데이트 할 것
		//
		//
		//
		//
		//
		//
		//
		//
		//IsPossible()을 g_Logic으로 뺄 것
		//
		//
		//
		//
		//
		//
		//
		//
		if (g_Logic->IsPossible(g_Logic->CalculateIndex(mouseCoordinate) ) )
		{
			g_Logic->Update(mouseCoordinate);
		}

		break;
	case WM_PAINT:
		CRenderer::GetInstance()->Begin();
		CRenderer::GetInstance()->Clear();
		g_Logic->Render();
		CRenderer::GetInstance()->End();
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
