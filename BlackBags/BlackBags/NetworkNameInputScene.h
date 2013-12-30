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
	void			SetObjectSize();

	void InputPlayerName(int wParam);

private:
	bool CreateResource();

	void ErrorHandling();
	std::wstring KeyMap(int virtualKeyCode);

	/*	현재 화면 스케일에 맞춰서 m_TextFormat 갱신  */
	void RefreshTextSize();
	void CalcStartPosition();

	// 사용자에게 입력받는 이름
	std::wstring				m_PlayerName;

	bool m_MouseoverFlag;

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;

	// Render 할 객체들을 계산하는 기준점 좌표
	D2D1_SIZE_F				m_StartPosition;

	// 캐릭터 이름을 표시하는 텍스트 포맷
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

