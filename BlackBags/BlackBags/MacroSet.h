#pragma once

template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p!= nullptr )
	{
		//p->Release();
		p = nullptr;
	}
}