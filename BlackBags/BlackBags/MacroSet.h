#pragma once

template <typename T>
inline void SafeDelete( T* &p )
{
	if ( p != nullptr )
	{
		delete p;
		p = nullptr;
	}
}

//사실 상 하는 일 없는 애
template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p!= nullptr )
	{
		//p->Release();
		p = nullptr;
	}
}

#define SINGLETON(type) \
	public: \
	static type* GetInstance() {static type s; return &s;}

struct Coordinate
{
	int m_PosX;
	int m_PosY;
};