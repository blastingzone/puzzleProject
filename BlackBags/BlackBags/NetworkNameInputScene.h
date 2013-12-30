#pragma once
#include "scene.h"

struct IDWriteFactory;

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
	void			SetObjectSize();

	void InputPlayerName(int wParam);

private:
	bool CreateResource();

	void ErrorHandling();
	std::wstring KeyMap(int virtualKeyCode);

	/*	���� ȭ�� �����Ͽ� ���缭 m_TextFormat ����  */
	void RefreshTextSize();
	void CalcStartPosition();

	// ����ڿ��� �Է¹޴� �̸�
	std::wstring				m_PlayerName;

	bool m_MouseoverFlag;

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;

	// Render �� ��ü���� ����ϴ� ������ ��ǥ
	D2D1_SIZE_F				m_StartPosition;

	// ĳ���� �̸��� ǥ���ϴ� �ؽ�Ʈ ����
	float					m_PlayerNameTextSize;
	IDWriteTextFormat*		m_PlayerNameTextFormat;

	ID2D1SolidColorBrush*	m_pPlayerNameBrush;

	ID2D1Bitmap*			m_pTitleImage;
	ID2D1Bitmap*			m_pNexthImage;
	ID2D1Bitmap*			m_pNextSelectedImage;

	float m_NameTextBoxWidth;
	float m_NameTextBoxHeight;

	float m_TitlePositionH;
	float m_TitlePositionV;
	float m_TitleWidth;
	float m_TitleHeight;

	float m_ButtonPositionH;
	float m_ButtonPositionV;
	float m_ButtonWidth;
	float m_ButtonHeight;
};

