#pragma once

//���߿� ���� ��� �� ���� Ŭ�������� ��� �� ���� �����ؼ� ����� �� �ֵ��� �� ��
enum Owner{
	NOBODY = 0,
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4
};

enum Item{
	NOTHING,
	GOLD,
	TRASH
};

class CMapObj
{
public:
	CMapObj(void);
	virtual ~CMapObj(void);

public :
	//line class
	virtual bool	isConnected() = 0;
	virtual void	connect() = 0;

	//tile class
	virtual Owner	getOwner() = 0;
	virtual void	setOwner(Owner inputOwner) = 0;
	virtual Item	getItem() = 0;
	virtual void	setItem(Item inputItem) = 0;
	virtual int		getScore() = 0;
};

