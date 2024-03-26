#pragma once
#include <windows.h>
#include <vector>
using namespace std;
#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(const t& value)
#endif // !PROPERTY


template<typename T, class _Alloc = allocator<T>>
class List :public vector<T, _Alloc>
{
public:
    List() {}
    List(T* val, int len)
    {
        for (int i = 0; i < len; i++)
        {
            this->push_back(val[i]);
        }
    }
    List(initializer_list<T> val)
    {
        for (int i = 0; i < val.size(); i++)
        {
            this->push_back(val.begin()[i]);
        }
    }
    List(vector< T> val)
    {
        for (int i = 0; i < val.size(); i++)
        {
            this->push_back(val[i]);
        }
    }
    ~List() {}
    T& operator[](int index)
    {
        return this->data()[index];
    }
    PROPERTY(int, Count);
    GET(int, Count)
    {
        return this->size();
    }
    SET(int, Count)
    {
        this->resize(value);
    }
    void Add(T val)
    {
        this->push_back(val);
    }
    void Clear()
    {
        this->clear();
    }
    void AddRange(T* val, int len)
    {
        int ldx = this->Count;
        this->resize(this->size() + len);
        memcpy(&this->operator[](ldx), &val[0], sizeof(T) * len);
    }
    void AddRange(initializer_list<T> val)
    {
        int ldx = this->Count;
        this->resize(this->size() + val.size());
        memcpy(&this->data()[ldx], &val.begin()[0], sizeof(T) * val.size());
    }
    void AddRange(List<T>& val)
    {
        int ldx = this->Count;
        this->resize(this->size() + val.size());
        memcpy(&this->data()[ldx], &val[0], sizeof(T) * val.size());
    }
    void AddRange(List<T>* val)
    {
        int ldx = this->Count;
        this->resize(this->size() + val->size());
        memcpy(&this->data()[ldx], &val->data()[0], sizeof(T) * val->size());
    }
    void AddRange(vector<T>& val)
    {
        int ldx = this->Count;
        this->resize(this->size() + val.size());
        memcpy(&this->data()[ldx], &val[0], sizeof(T) * val.size());
    }
    void AddRange(vector<T>* val)
    {
        int ldx = this->Count;
        this->resize(this->size() + val->size());
        memcpy(&this->data()[ldx], &val->data()[0], sizeof(T) * val->size());
    }

    void Insert(int index, T val)
    {
        if (index >= this->size())
        {
            this->push_back(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val);
        }
    }
    void Insert(int index, initializer_list<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, vector<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, vector<T>* val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val->begin(), val->end() - 1);
        }
    }
    void Insert(int index, List<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, List<T>* val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val->begin(), val->end() - 1);
        }
    }
    void RemoveAt(int index)
    {
        if (index >= 0 && index < this->size())
        {
            this->erase(this->begin() + index);
        }
    }
    int IndexOf(T value)
    {
        for (int i = 0; i < this->Count; i++)
        {
            if (this->at(i) == value)
            {
                return i;
            }
        }
        return -1;
    }
    bool Contains(T value)
    {
        return IndexOf(value) >= 0;
    }
    int LastIndexOf(T value)
    {
        for (int i = this->Count - 1; i >= 0; i--)
        {
            if (this->at(i) == value)
            {
                return i;
            }
        }
        return -1;
    }
    int Remove(T item)
    {
        int num = 0;
        for (int i = this->Count; i >= 0; i--)
        {
            if (this->data()[i] == item)
            {
                RemoveAt(i);
                num += 1;
            }
        }
        return num;
    }
    vector<T>& GetVector()
    {
        return *(vector<T>*)this;
    }
    T* Pointer()
    {
        return this->data();
    }
    T& get(int i)
    {
        return this->data()[i];
    }
    void set(int i, T val)
    {
        this->data()[i] = val;
    }
    T& First()
    {
        return this->data()[0];
    }
    T& Last()
    {
        return this->data()[this->size() - 1];
    }
};