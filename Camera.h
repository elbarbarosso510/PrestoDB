#pragma once
#include <Windows.h>
#include "Vector3.h"
#include "mem.h"
typedef struct D3DXMATRIXX
{
    struct {
        FLOAT _11; FLOAT _21;  FLOAT  _31; FLOAT _41;
        FLOAT _12; FLOAT _22;  FLOAT _32;  FLOAT _42;
        FLOAT _13; FLOAT _23;  FLOAT _33;  FLOAT _43;
        FLOAT _14; FLOAT _24;  FLOAT _34;  FLOAT _44;
    };
} D3DXMATRIXX, * LPD3DXMATRIXX;