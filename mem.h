#pragma once
#include <memory>
#include <vector>
template<typename T>
__forceinline T read(__int64 ptr)
{
	return  (ptr && !IsBadReadPtr((void*)ptr, sizeof(T))) ? *(T*)(ptr) : T();
}
template<typename T>
T read_chain(__int64 start, const std::vector<__int64>& displacements)
{
	__int64 tmp = read<__int64>(start + displacements.at(0));
	for (int i = 1; i < displacements.size() - 1; i++)
		tmp = read<__int64>(tmp + displacements.at(i));
	return  read<T>(tmp + displacements.at(displacements.size()-1));
}
template<typename T>
__forceinline T read(void* ptr)
{
	return  (ptr && !IsBadReadPtr(ptr, sizeof(T))) ? *(T*)(ptr) : T();
}
template<typename T>
__forceinline void write(__int64 ptr, T val)
{
	if (ptr && !IsBadWritePtr((void*)ptr, sizeof(T))) *(T*)(ptr) = val;
}
template<typename T>
__forceinline void write(void* ptr, T val)
{
	if (ptr && !IsBadWritePtr((void*)ptr, sizeof(T))) *(T*)(ptr) = val;
}