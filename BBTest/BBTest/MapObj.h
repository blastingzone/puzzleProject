#pragma once

//나중에 점수 계산 및 로직 클래스에서 사용 할 때는 통합해서 사용할 수 있도록 할 것
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

