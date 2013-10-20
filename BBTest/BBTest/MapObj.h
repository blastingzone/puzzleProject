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
	bool	isConnected();
	void	connect();

	//tile class
	Owner	getOwner();
	void	setOwner(Owner inputOwner);
	Item	getItem();
	void	setItem(Item inputItem);
	int		getScore();
};

