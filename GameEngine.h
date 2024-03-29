#pragma once
#include <vector>
#include <unordered_map>
#include "GameState.h"
#include "Utils/List.h"
#include "Utils/Tuple.h"
#include "Vector3.h"
#include "Utils/Utils.h"
#include "SDK.h"
#include "mono.h"
using namespace std;
static SIZE window_size = { 1920,1080 };
#define THISOFFSET(x) mread<ULONG64>((ULONG64)this + x)
#define THISOFFSETTYPE(x,t) mread<t>((ULONG64)this + x)

#define CLASSPROPERTY(t,n,o) \
PROPERTY(t, n);\
GET(t, n) { return mread<t>((ULONG64)this + o); }\
SET(t, n) { mwrite<t>((ULONG64)this + o, value); }

#define CLASSPROPERTY_READ(t,n,o) \
READONLY_PROPERTY(t, n);\
GET(t, n) { return mread<t>((ULONG64)this + o); }

enum EBones : int
{
    None11 = 0,
    IKController,
    Mesh,
    Vest_0,
    Vest_1,
    backpack,
    backpack_0,
    backpack_1,
    backpack_2,
    razgruz,
    razgruz_0,
    razgruz_1,
    razgruz_2,
    Root_Joint,
    Pelvis,
    LThigh1,
    LThigh2,
    LCalf,
    LFoot,
    LToe,
    RThigh1,
    RThigh2,
    RCalf,
    RFoot,
    RToe,
    Bear_Feet,
    USEC_Feet,
    BEAR_feet_1,
    weapon_holster_pistol,
    Spine1,
    Gear1,
    Gear2,
    Gear3,
    Gear4,
    Gear4_1,
    Gear5,
    Spine2,
    Spine3,
    IK_S_LForearm1,
    IK_S_LForearm2,
    IK_S_LForearm3,
    IK_S_LPalm,
    IK_S_LDigit11,
    IK_S_LDigit12,
    IK_S_LDigit13,
    IK_S_LDigit21,
    IK_S_LDigit22,
    IK_S_LDigit23,
    IK_S_LDigit31,
    IK_S_LDigit32,
    IK_S_LDigit33,
    IK_S_LDigit41,
    IK_S_LDigit42,
    IK_S_LDigit43,
    IK_S_LDigit51,
    IK_S_LDigit52,
    IK_S_LDigit53,
    XYZ,
    LCollarbone_anim,
    RCollarbone_anim,
    RCollarbone_anim_XYZ,
    Weapon_root_3rd_anim,
    XYZ_1,
    Bend_Goal_Left,
    Bend_Goal_Right,
    Bend_Goal_Right_XYZ_1,
    Ribcage,
    IK_LForearm1,
    IK_LForearm2,
    IK_LForearm3,
    IK_LPalm,
    IK_LDigit11,
    IK_LDigit12,
    IK_LDigit13,
    IK_LDigit21,
    IK_LDigit22,
    IK_LDigit23,
    IK_LDigit31,
    IK_LDigit32,
    IK_LDigit33,
    IK_LDigit41,
    IK_LDigit42,
    IK_LDigit43,
    IK_LDigit51,
    IK_LDigit52,
    IK_LDigit53,
    Camera_animated,
    CameraContainer,
    Cam,
    LCollarbone,
    LUpperarm,
    LForearm1,
    LForearm2,
    LForearm3,
    LPalm,
    LDigit11,
    LDigit12,
    LDigit13,
    LDigit21,
    LDigit22,
    LDigit23,
    LDigit31,
    LDigit32,
    LDigit33,
    LDigit41,
    LDigit42,
    LDigit43,
    LDigit51,
    LDigit52,
    LDigit53,
    RCollarbone,
    RUpperarm,
    RForearm1,
    RForearm2,
    RForearm3,
    RPalm,
    RDigit11,
    RDigit12,
    RDigit13,
    RDigit21,
    RDigit22,
    RDigit23,
    RDigit31,
    RDigit32,
    RDigit33,
    RDigit41,
    RDigit42,
    RDigit43,
    RDigit51,
    RDigit52,
    RDigit53,
    Weapon_root,
    Neck,
    Head,
    Backpack,
    weapon_holster,
    weapon_holster1,
    Camera_animated_3rd
};
static std::initializer_list<std::initializer_list<EBones>> EBoneConnections =
{
    {EBones::Pelvis, EBones::LThigh1},
    {EBones::LThigh1, EBones::LThigh2},
    {EBones::LThigh2, EBones::LCalf},
    {EBones::LCalf, EBones::LFoot},
    {EBones::LFoot, EBones::LToe},
    {EBones::Pelvis, EBones::RThigh1},
    {EBones::RThigh1, EBones::RThigh2},
    {EBones::RThigh2, EBones::RCalf},
    {EBones::RCalf, EBones::RFoot},
    {EBones::RFoot, EBones::RToe},
    {EBones::Pelvis, EBones::Spine1},
    {EBones::Spine1, EBones::Spine2},
    {EBones::Spine2, EBones::Spine3},
    {EBones::Spine3, EBones::Neck},
    {EBones::Neck, EBones::Head},
    {EBones::Spine3, EBones::LCollarbone},
    {EBones::LCollarbone, EBones::LForearm1},
    {EBones::LForearm1, EBones::LForearm2},
    {EBones::LForearm2, EBones::LForearm3},
    {EBones::LForearm3, EBones::LPalm},
    {EBones::LPalm, EBones::LDigit11},
    {EBones::LDigit11, EBones::LDigit12},
    {EBones::LDigit12, EBones::LDigit13},
    {EBones::LPalm, EBones::LDigit21},
    {EBones::LDigit21, EBones::LDigit22},
    {EBones::LDigit22, EBones::LDigit23},
    {EBones::LPalm, EBones::LDigit31},
    {EBones::LDigit31, EBones::LDigit32},
    {EBones::LDigit32, EBones::LDigit33},
    {EBones::LPalm, EBones::LDigit41},
    {EBones::LDigit41, EBones::LDigit42},
    {EBones::LDigit42, EBones::LDigit43},
    {EBones::LPalm, EBones::LDigit51},
    {EBones::LDigit51, EBones::LDigit52},
    {EBones::LDigit52, EBones::LDigit53},
    {EBones::Spine3, EBones::RCollarbone},
    {EBones::RCollarbone, EBones::RForearm1},
    {EBones::RForearm1, EBones::RForearm2},
    {EBones::RForearm2, EBones::RForearm3},
    {EBones::RForearm3, EBones::RPalm},
    {EBones::RPalm, EBones::RDigit11},
    {EBones::RDigit11, EBones::RDigit12},
    {EBones::RDigit12, EBones::RDigit13},
    {EBones::RPalm, EBones::RDigit11},
    {EBones::RDigit21, EBones::RDigit22},
    {EBones::RDigit22, EBones::RDigit23},
    {EBones::RPalm, EBones::RDigit11},
    {EBones::RDigit31, EBones::RDigit32},
    {EBones::RDigit32, EBones::RDigit33},
    {EBones::RPalm, EBones::RDigit11},
    {EBones::RDigit41, EBones::RDigit42},
    {EBones::RDigit42, EBones::RDigit43},
    {EBones::RPalm, EBones::RDigit11},
    {EBones::RDigit51, EBones::RDigit52},
    {EBones::RDigit52, EBones::RDigit53}
};
struct TransformAccessReadOnly
{
    ULONGLONG	pTransformData;
    int			index;
};
struct TransformData
{
    uint64_t pTransformArray;
    uint64_t pTransformIndices;
};
struct Matrix34
{
    Vector4 vec0;
    Vector4 vec1;
    Vector4 vec2;
};
static Vector3 ProcessBone(uint64_t pTransform)
{
    __m128 result;

    const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
    const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
    const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

    TransformAccessReadOnly pTransformAccessReadOnly = mread<TransformAccessReadOnly>(pTransform + 0x38);
    if (!pTransformAccessReadOnly.pTransformData) return { 0,0,0 };
    unsigned int index = mread<unsigned int>(pTransform + 0x40);
    TransformData transformData = mread<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

    SIZE_T sizeMatriciesBuf = sizeof(Matrix34) * index + sizeof(Matrix34);
    SIZE_T sizeIndicesBuf = sizeof(int) * index + sizeof(int);


    if (transformData.pTransformArray && transformData.pTransformIndices)
    {
        result = mread<__m128>(transformData.pTransformArray + 0x30 * index);
        int transformIndex = mread<int>(transformData.pTransformIndices + 0x4 * index);

        while (transformIndex >= 0)
        {
            Matrix34 matrix34 = mread<Matrix34>(transformData.pTransformArray + 0x30 * transformIndex);

            __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
            __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
            __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
            __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
            __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
            __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
            __m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

            result = _mm_add_ps(
                _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(
                            _mm_sub_ps(
                                _mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
                                _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
                        _mm_mul_ps(
                            _mm_sub_ps(
                                _mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
                                _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
                    _mm_add_ps(
                        _mm_mul_ps(
                            _mm_sub_ps(
                                _mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
                                _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
                        tmp7)), *(__m128*)(&matrix34.vec0));

            transformIndex = mread<int>(transformData.pTransformIndices + 0x4 * transformIndex);
        }
    }
    return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

struct MonoObject {
    void* vtable;
    void* synchronisation;
};
class MonoString {
public:
    CLASSPROPERTY_READ(int, length, 0x10);
    READONLY_PROPERTY(wchar_t*, chars);
    GET(wchar_t*, chars)
    {
        return (wchar_t*)mread_buffer((ULONG64)this + 0x14, (this->length + 1) * sizeof(wchar_t));
    }
    bool IsVaild()
    {
        return this != NULL &&
            this->length > 0 &&
            this->length < 0x1000;
    }
};
template<typename T>
class SharpList
{
public:
    int Count()
    {
        return mread<int>((ULONG64)this + 0x18);
    }
    T* ListPtr()
    {
        ULONG64 tmp = mread<ULONG64>((ULONG64)this + 0x10);
        return (T*)(tmp ? tmp + 0x20 : NULL);
    }
    T get(int index)
    {
        ULONG64 ptr = (ULONG64)ListPtr() + (index * sizeof(T));
        return mread<T>(ptr);
    }
};

template<typename T>
class QuestList
{
public:
    SharpList<T>* list()
    {
        return mread<SharpList<T>*>((ULONG64)this + 80);
    }
};
template<typename T>
class SharpArray
{
public:
    int Count()
    {
        return mread<int>((ULONG64)this + 0x18);
    }
    T* ListPtr()
    {
        ULONG64 tmp = this ? (ULONG64)this : 0;
        return (T*)(tmp ? tmp + 0x20 : NULL);
    }
    T get(int index)
    {
        int s = sizeof(T);
        ULONG64 ptr = (ULONG64)ListPtr() + (index * sizeof(T));
        return mread<T>(ptr);
    }
    bool IsVaild()
    {
        return this != NULL;
    }
};
template<typename TKey, typename TValue>
class SharpDictionary
{
    typedef struct KeyValuePair
    {
        int hashCode;
        int next;
        TKey key;
        TValue value;
    };
public:
    SharpArray<KeyValuePair>* entries() { return mread<SharpArray<KeyValuePair>*>((ULONG64)this + 0x18); }
    int Count() { return mread<int>((ULONG64)this + 0x40); }
};
template<typename T>
class SharpHashSet
{
public:
    typedef struct Slot
    {
        int hashCode;
        int next;
        T value;
    };
    char ukn[0x10];
    SharpArray<int>* Buckets() { return mread<SharpArray<int>*>((ULONG64)this + 0x10); }
    SharpArray<Slot>* Slots() { return mread<SharpArray<Slot>*>((ULONG64)this + 0x18); }
    int Count()
    {
        return mread<int>((ULONG64)this + 0x30);
    }

};
struct GameObjectManager
{
    ULONG64 LastTaggedNode; // 0x0
    ULONG64 TaggedNodes; // 0x8
    ULONG64 LastMainCameraTaggedNode; // 0x10
    ULONG64 MainCameraTaggedNodes; // 0x18
    ULONG64 LastActiveNode; // 0x20
    ULONG64 ActiveNodes; // 0x28
};
struct BaseObject
{
    ULONG64 previousObjectLink; // 0x0
    ULONG64 nextObjectLink; // 0x8
    ULONG64 obj; // 0x10   (to Offsets.GameObject)
};
struct CamerasArray
{
    uint64_t	Cameras; // 0x0
    INT64		SizeMin;
    INT64		SizeCur; // 0x10
    INT64		SizeMax;
};

enum EQuestType
{
    EQuestType_PickUp,
    EQuestType_Elimination,
    EQuestType_Discover,
    EQuestType_Completion,
    EQuestType_Exploration,
    EQuestType_Levelling,
    EQuestType_Experience,
    EQuestType_Standing,
    EQuestType_Loyalty,
    EQuestType_Merchant,
    EQuestType_Skill,
    EQuestType_Multi,
    EQuestType_WeaponAssembly
};
class Object
{
public:
    template<typename T>
    static List<T> FindObjectsOfTypeName(const char* name)
    {
        List<T> result = List<T>();
        ULONG64 GOM = mread<ULONG64>(_GameObjectManager);
        auto obj_tmp = mread<uint64_t>(GOM + 0x28);
        auto obj_last = mread<uint64_t>(GOM + 0x20);
        if (obj_tmp != 0) {
            while (true) {
                Object* object_ptr = mread<Object*>(obj_tmp + 0x10);
                uint64_t object_name_ptr = mread<uint64_t>((ULONG64)object_ptr + 0x60);
                if (char* tmp_name = mread_buffer(object_name_ptr, 128)) {
                    ULONG64 _tmp = mread<uint64_t>(object_ptr + 0x30);
                    if (_tmp) {
                        _tmp = mread<uint64_t>(_tmp + 0x18);
                        if (_tmp) {
                            _tmp = mread<uint64_t>(_tmp + 0x28);
                            if (_tmp) {
                                if (!strcmp(((Object*)_tmp)->ClassName, name)) {
                                    result.Add((T)_tmp);
                                }
                            }
                        }
                    }
                }
                if (obj_tmp == obj_last) {
                    break;
                }
                obj_tmp = mread<uint64_t>(obj_tmp + 0x8);
            }
        }
        return result;
    }
    template<typename T>
    static List<T> FindObjectsOfTypeNames(initializer_list< const char*> names)
    {
        List<T> result = List<T>();
        ULONG64 GOM = mread<ULONG64>(_GameObjectManager);
        auto obj_tmp = mread<uint64_t>(GOM + 0x28);
        auto obj_last = mread<uint64_t>(GOM + 0x20);
        if (obj_tmp != 0) {
            while (true) {
                Object* object_ptr = mread<Object*>(obj_tmp + 0x10);
                uint64_t object_name_ptr = mread<uint64_t>((ULONG64)object_ptr + 0x60);
                if (char* tmp_name = mread_buffer(object_name_ptr, 128)) {
                    ULONG64 _tmp = mread<uint64_t>(object_ptr + 0x30);
                    if (_tmp) {
                        _tmp = mread<uint64_t>(_tmp + 0x18);
                        if (_tmp) {
                            _tmp = mread<uint64_t>(_tmp + 0x28);
                            if (_tmp) {
                                for (int i = 0; i < names.size(); i++) {
                                    if (!strcmp(((Object*)_tmp)->ClassName, names.begin()[i])) {
                                        result.Add((T)_tmp);
                                    }
                                }
                            }
                        }
                    }
                }
                if (obj_tmp == obj_last) {
                    break;
                }
                obj_tmp = mread<uint64_t>(obj_tmp + 0x8);
            }
        }
        return result;
    }
    READONLY_PROPERTY(char*, ClassName);
    GET(char*, ClassName)
    {
        return (char*)mread_buffer(mread<ULONG64>(mread<ULONG64>(mread<ULONG64>((ULONG64)this)) + 0x48), 128);
    }
    READONLY_PROPERTY(char*, NameSpace);
    GET(char*, NameSpace)
    {
        return (char*)mread_buffer(mread<ULONG64>(mread<ULONG64>(mread<ULONG64>((ULONG64)this)) + 0x50), 128);
    }
};
class Transform
{
    Vector3 ProcessBone(uint64_t pTransform)
    {
        __m128 result;

        const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
        const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
        const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

        TransformAccessReadOnly pTransformAccessReadOnly = mread<TransformAccessReadOnly>(pTransform + 0x38);
        if (!pTransformAccessReadOnly.pTransformData) return { 0,0,0 };
        unsigned int index = mread<unsigned int>(pTransform + 0x40);
        TransformData transformData = mread<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

        SIZE_T sizeMatriciesBuf = sizeof(Matrix34) * index + sizeof(Matrix34);
        SIZE_T sizeIndicesBuf = sizeof(int) * index + sizeof(int);


        if (transformData.pTransformArray && transformData.pTransformIndices)
        {
            result = mread<__m128>(transformData.pTransformArray + 0x30 * index);
            int transformIndex = mread<int>(transformData.pTransformIndices + 0x4 * index);

            while (transformIndex >= 0)
            {
                Matrix34 matrix34 = mread<Matrix34>(transformData.pTransformArray + 0x30 * transformIndex);

                __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
                __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
                __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
                __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
                __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
                __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
                __m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

                result = _mm_add_ps(
                    _mm_add_ps(
                        _mm_add_ps(
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
                        _mm_add_ps(
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
                            tmp7)), *(__m128*)(&matrix34.vec0));

                transformIndex = mread<int>(transformData.pTransformIndices + 0x4 * transformIndex);
            }
        }
        return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
    }
public:
    READONLY_PROPERTY(Vector3, position);
    GET(Vector3, position)
    {
#ifdef _WINDLL
        return ProcessBone((ULONG64)this);
#else
        return ProcessBone((ULONG64)this);
        //uint64_t LocalVisualState = mread<ULONG64>((ULONG64)this + 0x38);
        //return mread<Vector3>(LocalVisualState + 0x90);
#endif // _WINDLL
    }
    READONLY_PROPERTY(Vector3, position_from_visual_state);
    GET(Vector3, position_from_visual_state)
    {
        auto VisualState = mread<ULONG64>((ULONG64)this + 0x28);
        VisualState = mread<ULONG64>(VisualState + 0x10);
        return ProcessBone(VisualState);
    }
    READONLY_PROPERTY(ULONG64, VisualState);
    GET(ULONG64, VisualState)
    {
        return mread<ULONG64>((ULONG64)this + 0x38);
    }
};
class GameObject : public Object
{
public:
    READONLY_PROPERTY(Transform*, transform);
    GET(Transform*, transform)
    {
        uint64_t LocalobjClass = mread<ULONG64>((ULONG64)this + 0x30);
        return mread<Transform*>(LocalobjClass + 0x8);
    }
    void* GetComponentByName(const char* type_name)
    {
        uint64_t obj = mread<uint64_t>((uint64_t)this + 0x30);
        int count = mread<uint32_t>(obj + 0x40) + 2;
        for (int i = 0; i < count; i++)
        {
            uint64_t object = mread<uint64_t>(obj + i * 0x10 + 0x8);
            object = mread<uint64_t>(object + 0x28);
            uint64_t v = mread<uint64_t>(object);
            v = mread<uint64_t>(v);
            v = mread<uint64_t>(v + 0x48);
            char name[100] = { 0 };
            mread_buffer(name, v, 64);
            if (!strcmp(type_name, name))
            {
                return (void*)object;
            }
        }
        return NULL;
    }
};
class Component : public Object
{
public:
    READONLY_PROPERTY(Transform*, transform);
    GET(Transform*, transform)
    {
        uint64_t LocalobjClass = mread<ULONG64>((ULONG64)this + 0x30);
        return mread<Transform*>(LocalobjClass + 0x8);
    }
    GameObject* gameObject()
    {
        uint64_t LocalFirst = mread<ULONG64>((ULONG64)this + 0x10);
        uint64_t LocalGameObj = mread<ULONG64>(LocalFirst + 0x30);
        return (GameObject*)(LocalGameObj);
    }
};
class ThermalVision
{
public:
    CLASSPROPERTY(bool, On, OFFSET::ThermalVision_On);
    CLASSPROPERTY(bool, IsNoisy, OFFSET::ThermalVision_IsNoisy);
    CLASSPROPERTY(bool, IsFpsStuck, OFFSET::ThermalVision_IsFpsStuck);
    CLASSPROPERTY(bool, IsMotionBlurred, OFFSET::ThermalVision_IsMotionBlurred);
    CLASSPROPERTY(bool, IsGlitch, OFFSET::ThermalVision_IsGlitch);
    CLASSPROPERTY(bool, IsPixelated, OFFSET::ThermalVision_IsPixelated);
    //CLASSPROPERTY(bool, ZeroedUnsharpRadius, OFFSET::ThermalVision_ZeroedUnsharpRadius);

};
class NightVision
{
public:
    CLASSPROPERTY(bool, On, OFFSET::NightVision__on);
};
class BaseItem : public Object
{
public:

    CLASSPROPERTY_READ(MonoString*, Id, 16);
    CLASSPROPERTY_READ(MonoString*, _tpl, 24);
    CLASSPROPERTY_READ(MonoString*, parentId, 32);
    CLASSPROPERTY_READ(MonoString*, slotId, 40);
};

class ItemTemplate : public Object
{
public:
    CLASSPROPERTY_READ(MonoString*, Id, OFFSET::ItemTemplate__id);
    CLASSPROPERTY_READ(MonoString*, Name, OFFSET::ItemTemplate_Name);
    CLASSPROPERTY_READ(MonoString*, ShortName, OFFSET::ItemTemplate_ShortName);
    CLASSPROPERTY_READ(MonoString*, Description, OFFSET::ItemTemplate_Description);
    CLASSPROPERTY_READ(bool, QuestItem, OFFSET::ItemTemplate_QuestItem);
    CLASSPROPERTY_READ(bool, MergesWithChildren, OFFSET::ItemTemplate_MergesWithChildren);
    CLASSPROPERTY_READ(int, BackgroundColor, OFFSET::ItemTemplate_BackgroundColor);
    CLASSPROPERTY_READ(int, Width, OFFSET::ItemTemplate_Width);
    CLASSPROPERTY_READ(int, Height, OFFSET::ItemTemplate_Height);
    CLASSPROPERTY_READ(int, CreditsPrice, OFFSET::ItemTemplate_CreditsPrice);
    CLASSPROPERTY_READ(int, LootExperience, OFFSET::ItemTemplate_LootExperience);
    CLASSPROPERTY_READ(int, ExamineExperience, OFFSET::ItemTemplate_ExamineExperience);
    CLASSPROPERTY_READ(int, RepairCost, OFFSET::ItemTemplate_RepairCost);
    CLASSPROPERTY_READ(int, RepairSpeed, OFFSET::ItemTemplate_RepairSpeed);
    CLASSPROPERTY_READ(int, AnimationVariantsNumber, OFFSET::ItemTemplate_AnimationVariantsNumber);
    CLASSPROPERTY_READ(int, DiscardLimit, OFFSET::ItemTemplate_DiscardLimit);
    CLASSPROPERTY_READ(SharpList<ItemTemplate*>*, Children, OFFSET::ItemTemplate__children);
    CLASSPROPERTY_READ(SharpList<ItemTemplate*>*, CompatibleItems, OFFSET::ItemTemplate__compatibleItems);
    CLASSPROPERTY_READ(SharpArray<BaseItem*>*, Items, OFFSET::ItemTemplate__items);
};

class Item : public Object
{
public:
    CLASSPROPERTY_READ(int, StackObjectsCount, OFFSET::Item_StackObjectsCount);
    CLASSPROPERTY_READ(ItemTemplate*, Template, 64);
    bool IsValid()
    {
        return this != NULL && this->Template;
    }
};
class ItemCollection : public Object
{
public:
    SharpDictionary<Item*, ULONG64>* dictionary()
    {
        return (SharpDictionary<Item*, ULONG64>*)THISOFFSET(16);
    }
    SharpList<Item*>* Items()
    {
        return (SharpList<Item*>*)THISOFFSET(24);
    }
};
class Grid : public Object
{
public:
    ItemCollection* Collection()
    {
        return (ItemCollection*)THISOFFSET(64);
    }
};
class Slot : public Object
{
public:
    Item* ContainedItem() { return THISOFFSETTYPE(OFFSET::Slot__ContainedItem_k__BackingField, Item*); }
    MonoString* Id() { return THISOFFSETTYPE(OFFSET::Slot_Id, MonoString*); }
};
class LootItemClass : public Item
{
public:
    SharpArray<Grid*>* Grids()
    {
        return (SharpArray<Grid*>*)THISOFFSET(OFFSET::LootItemClass_Grids);
    }
    SharpArray<Slot*>* Slots()
    {
        return (SharpArray<Slot*>*)THISOFFSET(OFFSET::LootItemClass_Slots);
    }
    bool IsContainerVaild()
    {
        return (this->Grids() && this->Grids()->Count() > 0 && this->Grids()->Count() < 0x100) ||
            (this->Slots() && this->Slots()->Count() > 0 && this->Slots()->Count() < 0x100);

    }
};
class TraderControllerClass : public Object
{
public:
    LootItemClass* RootItem()
    {
        return (LootItemClass*)THISOFFSET(192);
    }
};
class LootItem : public Component
{
public:
    MonoString* TemplateId()
    {
        return (MonoString*)THISOFFSET(OFFSET::LootItem_TemplateId);
    }
    ULONG64 Profile()
    {
        return (ULONG64)THISOFFSET(0x10);
    }
    Item* item()
    {
        return (Item*)THISOFFSET(176);
    }
    bool IsValid()
    {
        return this != NULL;
    }
    TraderControllerClass* Owner()
    {
        return (TraderControllerClass*)THISOFFSET(OFFSET::LootItem_ItemOwner);
    }
};
class Corpse : public LootItem
{
public:
    ULONG64 PlayerBody()
    {
        return THISOFFSET(OFFSET::Corpse_PlayerBody);
    }
};
class LootableContainer : public Object
{
public:
    TraderControllerClass* Owner()
    {
        return (TraderControllerClass*)THISOFFSET(OFFSET::LootableContainer_ItemOwner);
    }
};
enum EPlayerSide
{
    Usec = 1,
    Bear,
    Savage = 4
};
enum EMemberCategory
{
    Default = 0,
    Developer = 1,
    UniqueId = 2,
    Trader = 4,
    Group = 8,
    System = 0x10,
    ChatModerator = 0x20,
    ChatModeratorWithPermanentBan = 0x40,
    UnitTest = 0x80,
    Sherpa = 0x100,
    Emissary = 0x200
};
enum EquipmentSlot
{
    FirstPrimaryWeapon,
    SecondPrimaryWeapon,
    Holster,
    Scabbard,
    _Backpack,
    SecuredContainer,
    TacticalVest,
    ArmorVest,
    Pockets,
    Eyewear,
    FaceCover,
    Headwear,
    Earpiece,
    Dogtag,
    ArmBand
};
const wchar_t* CategoryNames(EMemberCategory c)
{
    switch (c)
    {
    case Default:return L"默认";
    case Developer:return L"开发";
    case UniqueId:return L"唯一";
    case Trader:return L"交易员";
    case Group:return L"组成员";
    case System:return L"系统";
    case ChatModerator:return L"主持";
    case ChatModeratorWithPermanentBan:return L"权限主持";
    case UnitTest:return L"测试";
    case Sherpa:return L"夏尔巴人";
    case Emissary:return L"间谍";
    default:return L"未知";
    }
}

class PlayerInfo : public Object
{
public:
    int Experience()
    {
        return THISOFFSETTYPE(136, int);
    }
    int Level()
    {
        const Tuple<int, int> LevExperience[] =
        {
            {1,0},{2,1000},{3,4020},
            {4,8440},{5,14260},{6,21480},
            {7,30030},{8,39940},{9,51210},
            {10,63730},{11,77570},{12,92720},
            {13,111890},{14,134680},{15,161140},
            {16,191420},{17,225200},{18,262370},
            {19,302490},{20,345760},{21,391650},
            {22,440450},{23,492370},{24,547900},
            {25,609070},{26,675920},{27,748480},
            {28,826790},{29,910890},{30,1000810},
            {31,1096600},{32,1198280},{33,1309260},
            {34,1429580},{35,1559330},{36,1698540},
            {37,1847280},{38,2005600},{39,2173580},
            {40,2351260},{41,2538700},{42,2735970},
            {43,2946590},{44,3170640},{45,3408210},
            {46,3659370},{47,3924200},{48,4202790},
            {49,4495210},{50,4801560},{51,5121900},
            {52,5456320},{53,5809670},{54,6182070},
            {55,6573620},{56,6984430},{57,7414620},
            {58,7864290},{59,8333550},{60,8831060},
            {61,9360630},{62,9928580},{63,10541850},
            {64,11206300},{65,11946980},{66,12789150},
            {67,13820530},{68,15229490},{69,17206070},
            {70,19706070},{71,22706070},{72,26206070},
            {73,30206070},{74,34706070},{75,39706070},
            {76,45206070},{77,51206070},{78,58206070},
            {79,68206070},{80,78206070}
        };
        int exp = this->Experience();
        for (int i = 0; i < sizeof(LevExperience) / sizeof(LevExperience[0]); i++)
        {
            if (exp < LevExperience[i].Item2) return LevExperience[i].Item1 - 1;
        }
        return 80;
    }
    MonoString* NickName()
    {
        return THISOFFSETTYPE(16, MonoString*);
    }
    MonoString* MainProfileNickname()
    {
        return THISOFFSETTYPE(24, MonoString*);
    }
    EPlayerSide Side()
    {
        return THISOFFSETTYPE(112, EPlayerSide);
    }
    int RegistrationDate()
    {
        return THISOFFSETTYPE(116, int);
    }
    double SavageLockTime()
    {
        return THISOFFSETTYPE(120, double);
    }
    MonoString* GroupId()
    {
        return THISOFFSETTYPE(32, MonoString*);
    }
    MonoString* EntryPoint()
    {
        return THISOFFSETTYPE(48, MonoString*);
    }
    __int64 NicknameChangeDate()
    {
        return THISOFFSETTYPE(120, __int64);
    }
    MonoString* GameVersion()
    {
        return THISOFFSETTYPE(56, MonoString*);
    }
    MonoString* Voice()
    {
        return THISOFFSETTYPE(64, MonoString*);
    }
    bool IsStreamerModeAvailable()
    {
        return THISOFFSETTYPE(136, bool);
    }
    EMemberCategory MemberCategory()
    {
        return THISOFFSETTYPE(140, EMemberCategory);
    }
};
class RecodeKey
{
public:
    SharpHashSet<MonoString*>* Set()
    {
        return mread<SharpHashSet<MonoString*>*>((ULONG64)this + 16);
    }
};
class RecodeCounters
{
public:
    SharpDictionary<RecodeKey*, __int64>* Counters()
    {
        return mread<SharpDictionary<RecodeKey*, __int64>*>((ULONG64)this + 16);
    }
};
class Stats
{
public:
    RecodeCounters* OverallCounters() { return THISOFFSETTYPE(24, RecodeCounters*); }
};
enum EQuestStatus
{
    Locked,
    AvailableForStart,
    Started,
    AvailableForFinish,
    Success,
    Fail,
    FailRestartable,
    MarkedAsFailed,
    Expired
};
class Quest
{
public:
    CLASSPROPERTY_READ(MonoString*, Id, 16);
    CLASSPROPERTY_READ(MonoString*, LocationId, 24);
    CLASSPROPERTY_READ(int, Level, 104);
    CLASSPROPERTY_READ(bool, Restartable, 108);
    CLASSPROPERTY_READ(MonoString*, TraderId, 32);
    CLASSPROPERTY_READ(MonoString*, Image, 40);
    CLASSPROPERTY_READ(MonoString*, TemplateId, 48);

};
class Condition : public Object
{
public:
    CLASSPROPERTY_READ(MonoString*, id, 16);
    CLASSPROPERTY_READ(float, value, 48);
    QuestList<Condition*>* ChildConditions()
    {
        return mread<QuestList<Condition*>*>((ULONG64)this + OFFSET::Condition_ChildConditions);
    }
};
class ConditionOneTarget : public Condition
{
public:
    CLASSPROPERTY_READ(MonoString*, target, OFFSET::ConditionMultipleTargets_target);
};
class ConditionMultipleTargets : public Condition
{
public:
    CLASSPROPERTY_READ(SharpArray<MonoString*>*, target, OFFSET::ConditionOneTarget_target);
};
class ConditionVisitPlace : public ConditionOneTarget
{
};
class ConditionZone : public ConditionMultipleTargets
{
public:
    CLASSPROPERTY_READ(MonoString*, zoneId, OFFSET::ConditionZone_zoneId);
};
class ConditionInZone : public Condition
{
public:
    CLASSPROPERTY_READ(SharpArray<MonoString*>*, zoneIds, OFFSET::ConditionInZone_zoneIds);
};
class ConditionLocation : public Condition
{
public:
    CLASSPROPERTY_READ(SharpArray<MonoString*>*, target, OFFSET::ConditionLocation_target);

};
class ConditionInfo
{
public:
    SharpDictionary<EQuestStatus, QuestList<Condition*>*>* Conditions()
    {
        return mread<SharpDictionary<EQuestStatus, QuestList<Condition*>*>*>((ULONG64)this + 64);
    }
    CLASSPROPERTY_READ(MonoString*, Id, 16);
};
template<typename T>
class ConditionList
{
public:
    CLASSPROPERTY_READ(SharpList<T>*, _list, 80);
};
class Counter
{
public:
    CLASSPROPERTY_READ(MonoString*, Id, 16);
    CLASSPROPERTY_READ(ConditionList<Condition*>*, conditions, 24);
};
class ConditionCounterCreator : public Condition
{
public:
    CLASSPROPERTY_READ(EQuestType, type, OFFSET::ConditionCounterCreator_type);
    CLASSPROPERTY_READ(Counter*, counter, OFFSET::ConditionCounterCreator_counter);
};
class QuestData
{
public:
    CLASSPROPERTY_READ(MonoString*, Id, 16);
    CLASSPROPERTY_READ(int, StartTime, 48);
    CLASSPROPERTY_READ(EQuestStatus, Status, 52);
    CLASSPROPERTY_READ(SharpHashSet<MonoString*>*, CompletedConditions, 32);
    CLASSPROPERTY_READ(ConditionInfo*, Template, 40);
};
class Profile : public Object
{
public:
    PlayerInfo* Info()
    {
        return THISOFFSETTYPE(OFFSET::Profile_Info, PlayerInfo*);
    }
    Stats* stats() { return THISOFFSETTYPE(0xE8, Stats*); }
    CLASSPROPERTY_READ(SharpList<QuestData*>*, QuestsData, OFFSET::Profile_QuestsData);

};

class BifacialTransform : public Object
{
public:
    bool _accumulatePositionAndRotation() { return THISOFFSETTYPE(OFFSET::BifacialTransform__accumulatePositionAndRotation, bool); }
    Vector3 _accumulatedPosition() { return THISOFFSETTYPE(OFFSET::BifacialTransform__accumulatedPosition, Vector3); }
    Transform* Original()
    {
        return THISOFFSETTYPE(OFFSET::BifacialTransform_Original, Transform*);
    }
    Vector3 position()
    {
        auto original = this->Original();
        return ProcessBone((ULONG64)original);
    }
    Vector3 positionEx()
    {
        auto original = this->Original();
        return mread<Vector3>((ULONG64)original + 0x10);
    }
    bool IsVaild()
    {
        return this->Original() != NULL;
    }
};
class PlayerBones : public Object
{
public:
    Transform* Weapon_Root_Third()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Weapon_Root_Third, Transform*);
    }
    Transform* Weapon_Root_Anim()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Weapon_Root_Anim, Transform*);
    }
    Transform* Neck()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Neck, Transform*);
    }
    Transform* Weapon_reference_point()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Weapon_reference_point, Transform*);
    }
    Vector3 R_Neck()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_R_Neck, Vector3);
    }
    Vector3 R_Head()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_R_Head, Vector3);
    }
    SharpArray<Transform*>* Shoulders()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Shoulders, SharpArray<Transform*>*);
    }
    SharpArray<Transform*>* Shoulders_Anim()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Shoulders_Anim, SharpArray<Transform*>*);
    }
    SharpArray<Transform*>* Upperarms()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Upperarms, SharpArray<Transform*>*);
    }
    SharpArray<Transform*>* Forearms()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Forearms, SharpArray<Transform*>*);
    }
    Transform* LeftPalm()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftPalm, Transform*);
    }
    Transform* RightPalm()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RightPalm, Transform*);
    }
    Transform* LootRaycastOrigin()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LootRaycastOrigin, Transform*);
    }
    Transform* RootJoint()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RootJoint, Transform*);
    }
    Transform* HolsterPrimary()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_HolsterPrimary, Transform*);
    }
    Transform* HolsterPrimaryAlternative()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_HolsterPrimaryAlternative, Transform*);
    }
    Transform* HolsterSecondary()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_HolsterSecondary, Transform*);
    }
    Transform* HolsterSecondaryAlternative()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_HolsterSecondaryAlternative, Transform*);
    }
    Transform* ScabbardTagillaMelee()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_ScabbardTagillaMelee, Transform*);
    }
    Transform* HolsterPistol()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_HolsterPistol, Transform*);
    }
    Transform* LeftLegHolsterPistol()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftLegHolsterPistol, Transform*);
    }
    SharpArray<Transform*>* BendGoals()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_BendGoals, SharpArray<Transform*>*);
    }
    Transform* KickingFoot()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_KickingFoot, Transform*);
    }
    SharpArray<Transform*>* FovSpecialTransforms()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_FovSpecialTransforms, SharpArray<Transform*>*);
    }
    BifacialTransform* WeaponRoot()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_WeaponRoot, BifacialTransform*);
    }
    BifacialTransform* Ribcage()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Ribcage, BifacialTransform*);
    }
    BifacialTransform* Head()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Head, BifacialTransform*);
    }
    BifacialTransform* LeftShoulder()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftShoulder, BifacialTransform*);
    }
    BifacialTransform* RightShoulder()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RightShoulder, BifacialTransform*);
    }
    BifacialTransform* LeftThigh2()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftThigh2, BifacialTransform*);
    }
    BifacialTransform* RightThigh2()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RightThigh2, BifacialTransform*);
    }
    BifacialTransform* BodyTransform()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_BodyTransform, BifacialTransform*);
    }
    BifacialTransform* AnimatedTransform()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_AnimatedTransform, BifacialTransform*);
    }
    BifacialTransform* Pelvis()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Pelvis, BifacialTransform*);
    }
    BifacialTransform* LeftThigh1()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftThigh1, BifacialTransform*);
    }
    BifacialTransform* RightThigh1()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RightThigh1, BifacialTransform*);
    }
    BifacialTransform* Spine3()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Spine3, BifacialTransform*);
    }
    Vector3 Offset()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Offset, Vector3);
    }
    BifacialTransform* Fireport()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Fireport, BifacialTransform*);
    }
    BifacialTransform* LeftMultiBarrelFireport()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_LeftMultiBarrelFireport, BifacialTransform*);
    }
    BifacialTransform* RightMultiBarrelFireport()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_RightMultiBarrelFireport, BifacialTransform*);
    }
    float ThirdPersonAuthority()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_ThirdPersonAuthority, float);
    }
    Transform* Spine1()
    {
        return THISOFFSETTYPE(OFFSET::PlayerBones_Spine1, Transform*);
    }
};

class SlotInfo : public Object
{
public:
    Slot* ContainedSlot() { return THISOFFSETTYPE(16, Slot*); }
};
class SlotView : public Object
{
public:
    SharpList<SlotInfo*>* Slots() { return THISOFFSETTYPE(24, SharpList<SlotInfo*>*); }
};
class Skeleton : public Object
{
public:
    SharpList<Transform*>* Values() { return THISOFFSETTYPE(OFFSET::Skeleton__values, SharpList<Transform*>*); }
    SharpDictionary<MonoString*, Transform*>* Bones()
    {
        return mread<SharpDictionary<MonoString*, Transform*>*>((ULONG64)this + OFFSET::Skeleton_Bones);
    }
};
class PlayerBody : public Object
{
public:
    PlayerBones* Bones() { return THISOFFSETTYPE(OFFSET::PlayerBody_PlayerBones, PlayerBones*); }
    Transform* MeshTransform() { return THISOFFSETTYPE(OFFSET::PlayerBody__meshTransform, Transform*); }
    bool Active() { return THISOFFSETTYPE(OFFSET::PlayerBody__active, bool); }
    SlotView* SlotViews() { return THISOFFSETTYPE(OFFSET::PlayerBody_SlotViews, SlotView*); }
    Skeleton* SkeletonRootJoint() { return THISOFFSETTYPE(OFFSET::PlayerBody_SkeletonRootJoint, Skeleton*); }
};
class BodyPartStateDictionary
{
public:
};
class HealthController : public Object
{
public:
    ULONG64 BodyPartStates()
    {
        return mread<ULONG64>((ULONG64)this + 104);
    }
};
typedef struct HealthInfo
{
    float max;
    float current;
};
class StaminaInfo : public Object
{
public:
    PROPERTY(float, Current);
    GET(float, Current) { return mread<float>((ULONG64)this + 72); }
    SET(float, Current) { mwrite<float>((ULONG64)this + 72, value); }
};
class Physical : public Object
{
public:
    StaminaInfo* Stamina() { return THISOFFSETTYPE(56, StaminaInfo*); }
    StaminaInfo* HandsStamina() { return THISOFFSETTYPE(64, StaminaInfo*); }
    StaminaInfo* Oxygen() { return THISOFFSETTYPE(72, StaminaInfo*); }
};
class ShotEffector : public Object
{
public:
    PROPERTY(float, Intensity);
    GET(float, Intensity) { return mread<float>((ULONG64)this + OFFSET::ShotEffector_Intensity); }
    SET(float, Intensity) { mwrite<float>((ULONG64)this + OFFSET::ShotEffector_Intensity, value); }
    PROPERTY(float, Stiffness);
    GET(float, Stiffness) { return mread<float>((ULONG64)this + OFFSET::ShotEffector_Stiffness); }
    SET(float, Stiffness) { mwrite<float>((ULONG64)this + OFFSET::ShotEffector_Stiffness, value); }

    PROPERTY(Vector2, RecoilStrengthXy);
    GET(Vector2, RecoilStrengthXy) { return mread<Vector2>((ULONG64)this + OFFSET::ShotEffector_RecoilStrengthXy); }
    SET(Vector2, RecoilStrengthXy) { mwrite<Vector2>((ULONG64)this + OFFSET::ShotEffector_RecoilStrengthXy, value); }

    PROPERTY(Vector2, RecoilStrengthZ);
    GET(Vector2, RecoilStrengthZ) { return mread<Vector2>((ULONG64)this + OFFSET::ShotEffector_RecoilStrengthZ); }
    SET(Vector2, RecoilStrengthZ) { mwrite<Vector2>((ULONG64)this + OFFSET::ShotEffector_RecoilStrengthZ, value); }
};
class MotionEffector : public Object
{
public:
    PROPERTY(float, Intensity);
    GET(float, Intensity) { return mread<float>((ULONG64)this + OFFSET::MotionEffector_Intensity); }
    SET(float, Intensity) { mwrite<float>((ULONG64)this + OFFSET::MotionEffector_Intensity, value); }

    PROPERTY(Vector3, SwayFactors);
    GET(Vector3, SwayFactors) { return mread<Vector3>((ULONG64)this + OFFSET::MotionEffector_SwayFactors); }
    SET(Vector3, SwayFactors) { mwrite<Vector3>((ULONG64)this + OFFSET::MotionEffector_SwayFactors, value); }
};

class ForceEffector : public Object
{
public:
    PROPERTY(float, Intensity);
    GET(float, Intensity) { return mread<float>((ULONG64)this + OFFSET::ForceEffector_Intensity); }
    SET(float, Intensity) { mwrite<float>((ULONG64)this + OFFSET::ForceEffector_Intensity, value); }
};
class BreathEffector : public Object
{
public:
    PROPERTY(float, Intensity);
    GET(float, Intensity) { return mread<float>((ULONG64)this + OFFSET::BreathEffector_Intensity); }
    SET(float, Intensity) { mwrite<float>((ULONG64)this + OFFSET::BreathEffector_Intensity, value); }
};
class WalkEffector : public Object
{
public:
    PROPERTY(float, Intensity);
    GET(float, Intensity) { return mread<float>((ULONG64)this + OFFSET::WalkEffector_Intensity); }
    SET(float, Intensity) { mwrite<float>((ULONG64)this + OFFSET::WalkEffector_Intensity, value); }
};
class ProceduralWeaponAnimation : public Object
{
public:
    PROPERTY(Vector3, AimSwayMax);
    GET(Vector3, AimSwayMax) { return mread<Vector3>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMax); }
    SET(Vector3, AimSwayMax) { mwrite<Vector3>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMax, value); }

    PROPERTY(Vector3, AimSwayMin);
    GET(Vector3, AimSwayMin) { return mread<Vector3>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMin); }
    SET(Vector3, AimSwayMin) { mwrite<Vector3>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMin, value); }

    PROPERTY(float, AimSwayStartsThreshold);
    GET(float, AimSwayStartsThreshold) { return mread<float>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayStartsThreshold); }
    SET(float, AimSwayStartsThreshold) { mwrite<float>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayStartsThreshold, value); }

    PROPERTY(float, AimSwayMaxThreshold);
    GET(float, AimSwayMaxThreshold) { return mread<float>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMaxThreshold); }
    SET(float, AimSwayMaxThreshold) { mwrite<float>((ULONG64)this + OFFSET::ProceduralWeaponAnimation_AimSwayMaxThreshold, value); }

    ShotEffector* Shootingg() { return THISOFFSETTYPE(OFFSET::ProceduralWeaponAnimation_Shootingg, ShotEffector*); }
    MotionEffector* MotionReact() { return THISOFFSETTYPE(OFFSET::ProceduralWeaponAnimation_MotionReact, MotionEffector*); }
    ForceEffector* ForceReact() { return THISOFFSETTYPE(OFFSET::ProceduralWeaponAnimation_ForceReact, ForceEffector*); }
    BreathEffector* Breath() { return THISOFFSETTYPE(OFFSET::ProceduralWeaponAnimation_Breath, BreathEffector*); }
    WalkEffector* Walk() { return THISOFFSETTYPE(OFFSET::ProceduralWeaponAnimation_Walk, WalkEffector*); }
};
class MovementContext : public Object
{
public:
    CLASSPROPERTY(short, PhysicalCondition, 720);
    CLASSPROPERTY(Vector2, Rotation, 588);

};
class Chamber
{
public:
    CLASSPROPERTY_READ(bool, IsUsed, 120);

};
class StackSlot
{
public:
    CLASSPROPERTY_READ(int, MaxCount, OFFSET::StackSlot_MaxCount);
    CLASSPROPERTY_READ(SharpList<Item*>*, Items, OFFSET::StackSlot__items);
    READONLY_PROPERTY(int, Count);
    GET(int, Count)
    {
        if (this->Items->Count() == 0)
        {
            return 0;
        }
        int num = 0;
        for (int i = 0; i < this->Items->Count(); i++)
        {
            num += this->Items->get(i)->StackObjectsCount;
        }
        return num;
    }

};
class WeaponMagazine : public Item
{
public:
    CLASSPROPERTY_READ(StackSlot*, Cartridges, 144);

};
class AmmoTemplate : public ItemTemplate
{
public:
    CLASSPROPERTY(float, InitialSpeed, OFFSET::AmmoTemplate_InitialSpeed);

};
class Weapon : public Item
{
public:
    CLASSPROPERTY_READ(SharpArray<AmmoTemplate*>*, ShellsInChambers, OFFSET::Weapon__shellsInChambers);
    CLASSPROPERTY_READ(SharpArray<Slot*>*, Chambers, OFFSET::Weapon_Chambers);
    int ShellsInWeaponCount()
    {
        int num = 0;
        if (this->ShellsInChambers)
        {
            for (int i = 0; i < this->ShellsInChambers->Count(); i++)
            {
                num += this->ShellsInChambers->get(i) == NULL ? 0 : 1;
            }
        }
        return num;
    }
    int ChamberAmmoCount()
    {
        int num = 0;
        if (this && this->Chambers && this->Chambers->IsVaild())
        {
            if (this->Chambers->Count() > 0 && this->Chambers->Count() <= 20)
            {
                for (int i = 0; i < this->Chambers->Count(); i++)
                {
                    auto chamber = this->Chambers->get(i);
                    if (chamber)
                    {
                        auto ContainedItem = chamber->ContainedItem();
                        if (ContainedItem)
                        {
                            if (!((Chamber*)ContainedItem)->IsUsed)
                            {
                                num += 1;
                            }
                        }
                    }
                }
            }
        }
        return num;
    }
    AmmoTemplate* FirstAmmoInWeapol()
    {
        if (this && this->Chambers && this->Chambers->IsVaild())
        {
            if (this->Chambers->Count() > 0 && this->Chambers->Count() <= 20)
            {
                for (int i = 0; i < this->Chambers->Count(); i++)
                {
                    auto chamber = this->Chambers->get(i);
                    if (chamber)
                    {
                        auto ContainedItem = chamber->ContainedItem();
                        if (ContainedItem)
                        {
                            if (!((Chamber*)ContainedItem)->IsUsed)
                            {
                                return (AmmoTemplate*)ContainedItem->Template;
                            }
                        }
                    }
                }
            }
        }
        return NULL;
    }

    int ShellsInChamberCount()
    {
        int num = 0;
        if (this->Chambers)
        {
            for (int i = 0; i < this->Chambers->Count(); i++)
            {
                num +=
                    this->Chambers->get(i) == NULL ? 0 :
                    this->Chambers->get(i)->ContainedItem() == NULL ? 0 :
                    ((Chamber*)this->Chambers->get(i)->ContainedItem())->IsUsed ? 1 : 0;
            }
        }
        return num;
    }
    Slot* GetMagazineSlot()
    {
        LootItemClass* root = (LootItemClass*)this;
        auto slots = root->Slots();
        if (slots)
        {
            if (slots->Count() > 50) return NULL;
            for (int i = 0; i < slots->Count(); i++)
            {
                auto slot = slots->get(i);
                if (slot)
                {
                    auto ContainedItem = slot->ContainedItem();
                    if (ContainedItem && !wcscmp(L"mod_magazine", slot->Id()->chars))
                    {
                        return slot;
                    }
                }
            }
        }
        return NULL;
    }
    WeaponMagazine* GetCurrentMagazine()
    {
        Slot* magazineSlot = this->GetMagazineSlot();
        return magazineSlot != NULL ? (WeaponMagazine*)magazineSlot->ContainedItem() : NULL;
    }
};
class AbstractHandsController
{
public:

};
class ItemHandsController : public AbstractHandsController
{
public:
    CLASSPROPERTY_READ(Item*, item, 96);
};
class EquipmentController
{
public:
    CLASSPROPERTY_READ(SharpArray<Slot*>*, Slots, 128);
};
class Inventory
{
public:
    CLASSPROPERTY_READ(EquipmentController*, Equipment, 16);
};
class InventoryController
{
public:
    CLASSPROPERTY_READ(Inventory*, inventory, 296);
};
class TriggerColliderSearcher
{
#define __TMP__TYPE__1 SharpDictionary<Object*,Object*>*
public:
    CLASSPROPERTY_READ(SharpList<Component*>*, Components, 96);
};
enum class EBodyPart
{
    Head,
    Chest,
    Stomach,
    LeftArm,
    RightArm,
    LeftLeg,
    RightLeg,
    Common
};
struct ValueStruct
{
    static bool Positive(float value)
    {
        return value >= 1E-45f;
    }
public:
    float Current;
    float Maximum;
    float Minimum;
    float OverDamageReceivedMultiplier;
    float Normalized()
    {
        return this->Current / this->Maximum;
    }
    bool AtMinimum()
    {
        return !Positive(this->Current - this->Minimum);
    }
    bool AtMaximum()
    {
        return !Positive(this->Maximum - this->Current);
    }
};
class HealthValue
{
public:
    CLASSPROPERTY(ValueStruct, Value, OFFSET::HealthValue_Value);
};
class BodyPartState
{
public:
    CLASSPROPERTY(bool, IsDestroyed, OFFSET::BodyPartState_IsDestroyed);
    CLASSPROPERTY(HealthValue*, Health, OFFSET::BodyPartState_Health);
};
class Player : public Component
{
public:
    Vector3 GetBonePosition(int id)
    {
        ULONG64 p1 = (ULONG64)this->playerBody()->SkeletonRootJoint()->Values()->get(id);
        auto p2 = mread<ULONG64>(p1 + 0x10);
        return ProcessBone(p2);
    }
    PlayerBones* Bones() { return THISOFFSETTYPE(OFFSET::Player_playerBones_0, PlayerBones*); }
    HealthController* healthController() { return THISOFFSETTYPE(OFFSET::Player__healthController, HealthController*); }
    Profile* profile() { return THISOFFSETTYPE(1312, Profile*); }
    bool IsYourPlayer() { return THISOFFSETTYPE(2114, bool); }
    PlayerBody* playerBody() { return THISOFFSETTYPE(OFFSET::Player__playerBody, PlayerBody*); }
    Physical* physical() { return THISOFFSETTYPE(OFFSET::Player_Physical, Physical*); }
    ProceduralWeaponAnimation* WeaponAnimation() { return THISOFFSETTYPE(416, ProceduralWeaponAnimation*); }
    MovementContext* Movement() { return THISOFFSETTYPE(64, MovementContext*); }
    CLASSPROPERTY_READ(ItemHandsController*, HandsController, OFFSET::Player__handsController);
    CLASSPROPERTY_READ(InventoryController*, inventoryController, OFFSET::Player__inventoryController);
    CLASSPROPERTY_READ(SharpList<MonoString*>*, TriggerZones, OFFSET::Player_TriggerZones);
    CLASSPROPERTY_READ(TriggerColliderSearcher*, triggerColliderSearcher, OFFSET::Player__triggerColliderSearcher);
    HealthInfo GetHealthInfo()
    {
        const HealthInfo _defaule = { 0,0 };
        auto healthController = this->healthController(); if (!healthController) return _defaule;
        auto status = healthController->BodyPartStates(); if (!status) return _defaule;
        SharpDictionary<EBodyPart, BodyPartState*>* statusSharp = (SharpDictionary<EBodyPart, BodyPartState*>*)status;
        float max = 0;
        float curr = 0;
        if (statusSharp)
        {
            auto entries = statusSharp->entries();
            if (entries)
            {
                int count = entries->Count();
                for (int i = 0; i < count; i++)
                {
                    auto val = entries->get(i);
                    auto h = val.value->Health;
                    {
                        curr += h->Value.Current;
                        max += h->Value.Maximum;
                    }
                }
            }
        }
        return HealthInfo{ max , curr };
    }
    float GetKD()
    {
        auto DictCombo = (ULONG64)this->profile()->stats()->OverallCounters()->Counters()->entries();
        int deaths = 0;
        int kills = 0;
        if (DictCombo)
        {
            DictCombo += 0x28;
            int readed = 0;
            for (ULONG64 offs = 0; offs < 500; offs++) // idk
            {
                if (readed >= 2) break;
                ULONG64 CounterKey = mread<ULONG64>(DictCombo + offs * 0x18);
                if (CounterKey)
                {
                    ULONG64 set = mread<ULONG64>(CounterKey + 0x10);
                    if (set)
                    {
                        ULONG64 slots = mread<ULONG64>(set + 0x18);
                        if (slots)
                        {
                            MonoString* info = mread<MonoString*>(slots + 0x28);
                            if (info->IsVaild())
                            {
                                if (wcscmp(info->chars, L"Deaths") == 0)
                                {
                                    deaths = mread< ULONG64>(DictCombo + offs * 0x18 + 8);
                                    readed++;
                                }
                                if (wcscmp(info->chars, L"Kills") == 0)
                                {
                                    kills = mread<int64_t>(DictCombo + offs * 0x18 + 8);
                                    readed++;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (deaths == 0) deaths = 1;
        return float(kills) / float(deaths);
    }
    READONLY_PROPERTY(Vector3, Position);
    GET(Vector3, Position)
    {
        auto playerBody = this->playerBody(); if (!playerBody)return { 0,0,0 };
        auto Bones = playerBody->Bones(); if (!Bones)return { 0,0,0 };
        auto BodyTransform = Bones->BodyTransform(); if (!BodyTransform)return { 0,0,0 };
        return BodyTransform->position();
    }
};

enum EExfiltrationStatus : unsigned char
{
    NotPresent = 1,
    UncompleteRequirements,
    Countdown,
    RegularMode,
    Pending,
    AwaitsManualActivation
};
enum EExfiltrationType
{
    Individual,
    SharedTimer,
    Manual
};
class ExitTriggerSettings
{
public:
    CLASSPROPERTY_READ(MonoString*, Name, OFFSET::ExitTriggerSettings_Name);
    CLASSPROPERTY_READ(MonoString*, EntryPoints, OFFSET::ExitTriggerSettings_EntryPoints);
    CLASSPROPERTY(int, PlayersCount, OFFSET::ExitTriggerSettings_PlayersCount);
    CLASSPROPERTY(int, StartTime, OFFSET::ExitTriggerSettings_StartTime);
    CLASSPROPERTY(float, ExfiltrationTime, OFFSET::ExitTriggerSettings_ExfiltrationTime);
    CLASSPROPERTY(float, Chance, OFFSET::ExitTriggerSettings_Chance);
    CLASSPROPERTY(float, MinTime, OFFSET::ExitTriggerSettings_MinTime);
    CLASSPROPERTY(float, MaxTime, OFFSET::ExitTriggerSettings_MaxTime);
    CLASSPROPERTY(EExfiltrationType, ExfiltrationType, OFFSET::ExitTriggerSettings_ExfiltrationType);

};
class ExfiltrationPoint : public Component
{
public:
    CLASSPROPERTY(EExfiltrationStatus, Status, OFFSET::ExfiltrationPoint__status);
    CLASSPROPERTY_READ(MonoString*, Description, 56);
    CLASSPROPERTY_READ(ExitTriggerSettings*, Settings, OFFSET::ExfiltrationPoint_Settings);
};
class ExfiltrationController
{
public:
    CLASSPROPERTY_READ(SharpArray<ExfiltrationPoint*>*, ExfiltrationPoints, 32);
    CLASSPROPERTY_READ(SharpArray<ExfiltrationPoint*>*, ScavExfiltrationPoints, 40);

};
class Throwable : public Component {};
template<typename T, typename VAL>
class ThrowableListWapper
{
public:
    CLASSPROPERTY_READ(SharpList<VAL>*, _list, 24);
};
class TriggerWithId : public Component
{
public:
    CLASSPROPERTY_READ(MonoString*, Id, OFFSET::TriggerWithId__id);
};
class PlaceItemTrigger : public TriggerWithId {};
class BorderZone : public Component
{
public:
    CLASSPROPERTY(Vector4, _triggerZoneSettings, OFFSET::BorderZone__triggerZoneSettings);
    CLASSPROPERTY(Vector3, _extents, OFFSET::BorderZone__extents);
};
class LocationScene
{
public:
    static SharpList<LocationScene*>* LoadedScenes()
    {
        static ULONG64 LocationScene_LoadedScenes = NULL;
        if (!LocationScene_LoadedScenes)
        {
            LocationScene_LoadedScenes = mread<ULONG64>(mono::find_class(("Assembly-CSharp"), ("LocationScene"))->get_vtable(mono::get_root_domain())->get_static_field_data());
        }
        return (SharpList<LocationScene*>*)(LocationScene_LoadedScenes + OFFSET::LocationScene_LoadedScenes);
    }
    CLASSPROPERTY_READ(SharpArray<BorderZone*>*, BorderZones, OFFSET::LocationScene_BorderZones);
    SharpDictionary<void*, Object*>* AllObjects()
    {
        return mread<SharpDictionary<void*, Object*>*>((ULONG64)this + OFFSET::LocationScene_treeWindSettingsPresets + 8);
    }
};

class Camera : public GameObject
{
public:

    //READONLY_PROPERTY(ULONG64, FPSCamera);
    //GET(ULONG64, FPSCamera)
    //{
    //    return mread<uint64_t>((ULONG64)this + 0x30);
    //}
    READONLY_PROPERTY(ULONG64, View);
    GET(ULONG64, View)
    {
        ULONG64 _V1 = mread<uint64_t>((ULONG64)this + 0x30);
        return mread<uint64_t>(_V1 + 0x18);
    }

    //READONLY_PROPERTY(ULONG64, OpticCamera);
    //GET(ULONG64, OpticCamera)
    //{
    //    ULONG64 _V1 = this->View;
    //    uint64_t camMan = mread<uint64_t>((ULONG64)this + 0x70);
    //    uint64_t optic = mread<uint64_t>(camMan + 0x10);
    //    return mread<uint64_t>(optic + 0x30);
    //}
    //READONLY_PROPERTY(ULONG64, AdsViews);
    //GET(ULONG64, AdsViews)
    //{
    //    return mread<uint64_t>(this->OpticCamera + 0x18);
    //}
    D3DXMATRIXX GetViewss()
    {
        uint64_t camEnt = this->View;
        return mread<D3DXMATRIXX>(camEnt + 0xDC);
    }
    Vector3 W2S(Vector3 origin)
    {
        uint64_t camEnt = this->View;
        D3DXMATRIXX viewss = mread<D3DXMATRIXX>(camEnt + 0xDC);
        Vector3 out;
        D3DXMATRIXX temp = viewss;
        D3DXVECTOR3 translationVector = D3DXVECTOR3(temp._41, temp._42, temp._43);
        D3DXVECTOR3 up = D3DXVECTOR3(temp._21, temp._22, temp._23);
        D3DXVECTOR3 right = D3DXVECTOR3(temp._11, temp._12, temp._13);
        float w = D3DXVec3Dot(&translationVector, (D3DXVECTOR3*)&origin) + temp._44;

        if (w < 0.098f)
            return Vector3(0, 0, 0);
        float y = D3DXVec3Dot(&up, (D3DXVECTOR3*)&origin) + temp._24;
        float x = D3DXVec3Dot(&right, (D3DXVECTOR3*)&origin) + temp._14;

        out.x = (int)(window_size.cx / 2) * (1.f + x / w);
        out.y = (int)(window_size.cy / 2) * (1.f - y / w);
        return out;
    }

};

static std::unordered_map<wstring, int> PlayerQuests = std::unordered_map<wstring, int>();
static List<PlaceItemTrigger*> ItemQuestPlace = List<PlaceItemTrigger*>();
class GameWorld :public Component
{
public:
    SharpList<Player*>* RegisteredPlayers()
    {
        return (SharpList<Player*>*)THISOFFSET(OFFSET::GameWorld_RegisteredPlayers);
    }
    CLASSPROPERTY_READ(MonoString*, MapName, 32);
    SharpList<LootItem*>* LootList()
    {
        return mread<SharpList<LootItem*>*>((ULONG64)this + OFFSET::GameWorld_LootList);
    }
    ExfiltrationController* exfiltrationController()
    {
        return (ExfiltrationController*)THISOFFSET(24);
    };
    ThrowableListWapper<int, Throwable*>* Grenades()
    {
        return (ThrowableListWapper<int, Throwable*>*)THISOFFSET(OFFSET::GameWorld_Grenades);
    }
    SharpArray<BorderZone*>* BorderZones()
    {
        return mread<SharpArray<BorderZone*>*>((ULONG64)this + OFFSET::GameWorld_BorderZones);
    }
    READONLY_PROPERTY(Player*, LocalPlayer);
    GET(Player*, LocalPlayer)
    {
        SharpList<Player*>* players = this->RegisteredPlayers();
        if (!players) return NULL;
        int player_count = players->Count();
        if (player_count > 0 && player_count < 100)
        {
            for (int i = 0; i < player_count; i++)
            {
                Player* player = players->get(i);
                if (!player) continue;
                if (player->IsYourPlayer())
                {
                    return player;
                }
            }
        }
        return NULL;
    }

    void UpdatePlayerQuests(Player* self)
    {
        PlayerQuests.clear();
        auto pf = self->profile();
        auto _localplayer = LocalPlayer; if (!_localplayer) return;
        auto health_self = _localplayer->GetHealthInfo();
        if (health_self.current <= 0.0f) return;
        if (pf)
        {
            auto qs = pf->QuestsData; if (!qs) return;
            for (int n = 0; n < qs->Count(); n++)
            {
                auto quest = qs->get(n); if (!quest) continue;
                if (
                    quest->Status != EQuestStatus::AvailableForFinish
                    // &&quest->Status != EQuestStatus::AvailableForStart
                    && quest->Status != EQuestStatus::Started
                    // && quest->Status != EQuestStatus::FailRestartable
                    ) continue;
                auto info = quest->Template; if (!info) continue;
                auto cds = info->Conditions(); if (!cds) continue;
                auto ent = cds->entries(); if (!ent) continue;
                for (int e = 0; e < ent->Count(); e++)
                {
                    auto cn = ent->get(e);
                    auto val = cn.value; if (!val) continue;
                    auto _list = val->list(); if (!_list) continue;
                    for (int li = 0; li < _list->Count(); li++)
                    {
                        Condition* condition = _list->get(li); if (!condition) continue;
                        auto class_name = condition->ClassName; if (!class_name)continue;
                        string c_name = class_name;
                        if (
                            c_name == "ConditionOneTarget" ||
                            c_name == "ConditionVisitPlace" ||
                            c_name == "ConditionQuest" ||
                            c_name == "ConditionExamineItem")
                        {
                            ConditionOneTarget* test1 = (ConditionOneTarget*)condition;
                            if (test1->target->IsVaild())
                            {
                                PlayerQuests[test1->target->chars] = 1;
                            }
                        }
                        else if
                            (
                                c_name == "ConditionMultipleTargets" ||
                                c_name == "ConditionFindItem" ||
                                c_name == "ConditionHandoverItem")
                        {
                            ConditionMultipleTargets* test2 = (ConditionMultipleTargets*)condition;
                            if (test2->target->IsVaild() && test2->target->Count() < 0x40)
                            {
                                for (int ii = 0; ii < test2->target->Count(); ii++)
                                {
                                    auto item_info = test2->target->get(ii);
                                    if (item_info->IsVaild())
                                    {
                                        PlayerQuests[item_info->chars] = 1;
                                    }
                                }
                            }
                        }
                        else if (c_name == "ConditionZone" ||
                            c_name == "ConditionLeaveItemAtLocation" ||
                            c_name == "ConditionPlaceBeacon" ||
                            c_name == "ConditionItem")
                        {
                            ConditionZone* test3 = (ConditionZone*)condition;
                            if (test3->zoneId->IsVaild())
                            {
                                PlayerQuests[test3->zoneId->chars] = 1;
                            }
                        }
                        else if (c_name == "ConditionCounterCreator")
                        {
                            ConditionCounterCreator* cond = (ConditionCounterCreator*)condition;
                            auto counter = cond->counter;
                            if (!counter)continue;
                            auto conditions = counter->conditions;
                            if (!conditions)continue;
                            if (conditions)
                            {
                                auto list = conditions->_list;
                                if (!list)continue;
                                if (list && list->Count() > 0 && list->Count() < 0x40)
                                {
                                    for (int j = 0; j < list->Count(); j++)
                                    {
                                        auto sub_condition = list->get(j);
                                        if (!sub_condition)continue;
                                        auto c_name = sub_condition->ClassName;
                                        if (!c_name)continue;
                                        string sub_class_name = c_name;
                                        if (sub_class_name == "ConditionInZone")
                                        {
                                            ConditionInZone* cond = (ConditionInZone*)sub_condition;
                                            auto zoneIds = cond->zoneIds;
                                            if (zoneIds->IsVaild())
                                            {
                                                for (int n = 0; n < zoneIds->Count(); n++)
                                                {
                                                    auto zonId = zoneIds->get(n);
                                                    if (zonId->IsVaild())
                                                    {
                                                        PlayerQuests[zonId->chars] = 1;
                                                    }
                                                }
                                            }
                                        }
                                        else if (sub_class_name == "ConditionLocation")
                                        {
                                            ConditionLocation* cond = (ConditionLocation*)sub_condition;
                                            auto target = cond->target;
                                            if (target->IsVaild())
                                            {
                                                if (target->Count() > 0 && target->Count() < 0x10)
                                                {
                                                    for (int n = 0; n < target->Count(); n++)
                                                    {
                                                        auto tname = target->get(n);
                                                        if (tname->IsVaild())
                                                        {
                                                            PlayerQuests[tname->chars] = 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        else if (sub_class_name == "ConditionVisitPlace")
                                        {
                                            ConditionVisitPlace* cond = (ConditionVisitPlace*)sub_condition;
                                            auto tg = cond->target;
                                            if (tg->IsVaild())
                                            {
                                                PlayerQuests[tg->chars] = 1;
                                            }

                                        }
                                    }
                                }
                            }
                            else
                            {
                                //printf("CLASS_NAME=[ %s ]\n", condition->ClassName());
                            }
                        }

                    }
                }

                auto Completed = quest->CompletedConditions;
                if (Completed)
                {
                    auto slot = Completed->Slots();
                    if (slot)
                    {
                        for (int i = 0; i < slot->Count(); i++)
                        {
                            auto tmp = slot->get(i).value;
                            if (tmp->IsVaild())
                            {
                                PlayerQuests[tmp->chars] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
};

static GameWorld* GetGameWorld()
{
    ULONG64 GOM = mread<ULONG64>(_GameObjectManager);
    auto obj_tmp = mread<uint64_t>(GOM + 0x28);
    auto obj_last = mread<uint64_t>(GOM + 0x20);
    if (obj_tmp != 0)
    {
        int dodx = 0;
        while (dodx++ < 2000)
        {
            uint64_t object_ptr = mread<uint64_t>(obj_tmp + 0x10);
            uint64_t object_name_ptr = mread<uint64_t>(object_ptr + 0x60);
            if (char* tmp_name = mread_buffer(object_name_ptr, 128))
            {
                if (strstr(tmp_name, "GameWorld"))
                {
                    ULONG64 _V1 = mread<uint64_t>(object_ptr + 0x30);
                    ULONG64 _V2 = mread<uint64_t>(_V1 + 0x18);
                    GameWorld* _obj = mread<GameWorld*>(_V2 + 0x28);
                    return _obj;
                }
            }
            if (obj_tmp == obj_last)
            {
                break;
            }
            obj_tmp = mread<uint64_t>(obj_tmp + 0x8);
        }
    }
    return NULL;
}
static Camera* GetFPSCamera()
{
    if (uint64_t pCamerasArray = mread<uint64_t>(_CameraManager))
    {
        CamerasArray camry = mread<CamerasArray>(pCamerasArray);

        for (INT64 i = 0; i < camry.SizeCur; i++)
        {
            if (uint64_t pCamera = mread<uint64_t>(camry.Cameras + i * sizeof(uint64_t)))
            {
                bool isActive = mread<bool>(pCamera + 0x39);
                uint64_t object_ptr = mread<uint64_t>(pCamera + 0x30);
                uint64_t object_name_ptr = mread<uint64_t>(object_ptr + 0x60);
                if (char* tmp_name = mread_buffer(object_name_ptr, 128))
                {
                    if (strstr(tmp_name, "FPS Camera"))
                        //if (strstr(tmp_name, "BaseOpticCamera(Clone)"))
                    {
                        if (isActive)
                        {
                            return (Camera*)object_ptr;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

static void AimbotPlayer(Vector3 camerapos, Player* self, Vector3 worldAimPos)
{
    auto calculate_angle = [](const Vector3 from, const Vector3 to) -> Vector2
    {
        auto const diff = from - to;
        auto const length = diff.Length();
        Vector2 ret = { -atan2f(diff.x, -diff.z),asinf(diff.y / length) };
        return ret * 57.29578;
    };
    Vector2 vtr = calculate_angle(camerapos, worldAimPos);
    self->Movement()->Rotation = vtr;
}