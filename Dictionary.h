#pragma once
#include <unordered_map>
using namespace std;
template<typename _K, typename _V>
class Dictionary
{
private:
	unordered_map<_K, _V> map_val;
public:
	Dictionary()
	{
		map_val = unordered_map<_K, _V>();
	}
	_V& operator[](_K index)
	{
		return map_val[index];
	}
	bool ContainsKey(_K key)
	{
		return map_val.find(key) != map_val.end();
	}
	bool Remove(_K key)
	{
		auto tmp = map_val.find(key);
		if (tmp != map_val.end())
		{
			map_val.erase(tmp);
			return true;
		}
		return false;
	}
};