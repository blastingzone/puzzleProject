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
	virtual bool	isConnected();
	virtual void	connect();

	//tile class
	virtual Owner	getOwner();
	virtual void	setOwner(Owner inputOwner);
	virtual Item	getItem();
	virtual void	setItem(Item inputItem);
	virtual int		getScore();
};

