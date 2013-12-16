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
	//�� MessageBox �Լ��� �۵����� �ʴ���??
	//�ذ� : MB_DEFAULT_DESKTOP_ONLY�� �߰����ش�!
	std::wstring errorText = L"Error Code : ";
	errorText.append( std::to_wstring( GetLastError() ) );
	MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
	// ������ ����
	PostQuitMessage(-1);
}
