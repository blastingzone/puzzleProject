#pragma once

template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p!= nullptr )
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
inline void SafeArrayDelete( T* &p )
{
	if ( p != nullptr )
	{
		delete[] p;
		p = nullptr;
	}
}