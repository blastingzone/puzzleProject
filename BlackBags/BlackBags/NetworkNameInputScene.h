#pragma once
#include "scene.h"
class CNetworkNameInputScene :
	public CScene
{
public:
	CNetworkNameInputScene(void);
	~CNetworkNameInputScene(void);

	void GoNextScene();

	virtual bool	Init();
	virtual void	Render();
	virtual void	EventHandle(Coordinate mouseCoordinate);
	virtual void	MouseOver(Coordinate mouseCoordinate);
	virtual void	PlayBGM();
	virtual void	StopBGM();
	virtual void	ResizeClient();

	void CreateResource();

	void InputPlayerName(int wParam);

private:
	void ErrorHandling();
	std::wstring KeyMap(int virtualKeyCode);

	// ����ڿ��� �Է¹޴� �̸�
	std::wstring				m_PlayerName;

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;

	// Render �� ��ü���� ����ϴ� ������ ��ǥ
	D2D1_SIZE_F				m_StartPosition;

	// ĳ���� �̸��� ǥ���ϴ� �ؽ�Ʈ ����
	float					m_PlayerNameTextSize;
	IDWriteTextFormat*		m_PlayerNameTextFormat;

	ID2D1SolidColorBrush*	m_pPlayerNameBrush;
};

