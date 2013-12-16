#include "stdafx.h"
#include "SceneObject.h"


CSceneObject::CSceneObject(void)
{
}


CSceneObject::~CSceneObject(void)
{
}

void CSceneObject::ErrorHandling()
{
	//왜 MessageBox 함수가 작동하지 않는지??
	//해결 : MB_DEFAULT_DESKTOP_ONLY를 추가해준다!
	std::wstring errorText = L"Error Code : ";
	errorText.append( std::to_wstring( GetLastError() ) );
	MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
	// 비정상 종료
	PostQuitMessage(-1);
}
