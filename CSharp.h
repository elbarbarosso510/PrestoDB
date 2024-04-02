#pragma once
#include "mono.h"
#include "InfoDefines.h"

static SharpDll _SHARP_DLL;


template<typename T>
class SharpList
{
public:
    int Count()
    {
        return read<int>((ULONG64)this + 0x18);
    }
    T* ListPtr()
    {
        ULONG64 tmp = read<ULONG64>((ULONG64)this + 0x10);
        return (T*)(tmp ? tmp + 0x20 : NULL);
    }
    T get(int index)
    {
        ULONG64 ptr = (ULONG64)ListPtr() + (index * sizeof(T));
        return read<T>(ptr);
    }
};

template<typename T>
class QuestList
{
public:
    SharpList<T>* list()
    {
        return read<SharpList<T>*>((ULONG64)this + 80);
    }
};
template<typename T>
class SharpArray
{
public:
    int Count()
    {
        return read<int>((ULONG64)this + 0x18);
    }
    T* ListPtr()
    {
        ULONG64 tmp = this ? (ULONG64)this : 0;
        return (T*)(tmp ? tmp + 0x20 : NULL);
    }
    T get(int index)
    {
        ULONG64 ptr = (ULONG64)ListPtr() + (index * sizeof(T));
        return read<T>(ptr);
    }
    bool IsVaild()
    {
        return this != NULL &&
            !IsBadReadPtr(this,0x28)&&
            !IsBadReadPtr(this->ListPtr(),this->Count()*sizeof(T));
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
    SharpArray<KeyValuePair>* entries() { return read<SharpArray<KeyValuePair>*>((ULONG64)this + 0x18); }
    int Count() { return read<int>((ULONG64)this + 0x40); }
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
    SharpArray<int>* Buckets() { return read<SharpArray<int>*>((ULONG64)this + 0x10); }
    SharpArray<Slot>* Slots() { return read<SharpArray<Slot>*>((ULONG64)this + 0x18); }
    int Count()
    {
        return read<int>((ULONG64)this + 0x30);
    }

};
//static MonoString* (*LocalizedShortName)(MonoString*);
//static MonoString* (*LocalizedName)(MonoString*);
//static MonoString* (*Localized)(MonoString*);
static Defines::EItemType (*GetItemType)(ULONG64 itemType);
static void (*SetCurrent)(void*, float, bool) = NULL;
static MonoString* (*Localized)(MonoString*, MonoString*) = NULL;
static MonoString* (*LocalizedName)(MonoString*) = NULL;
static void* (*_GetAllVisibleItems)(void*) = NULL;
static void* GetType = NULL;
static void* ThermalVision_Type = NULL;
static void* NightVision_Type = NULL;
static void* Minefield_Type = NULL;
static void* TriggerWithId_Type = NULL;
static void* PlaceItemTrigger_Type = NULL;
static void* QuestTrigger_Type = NULL;
static void* ExperienceTrigger_Type = NULL;
static std::unordered_map<std::wstring, std::wstring> LocalizedShortNameTable = std::unordered_map<std::wstring, std::wstring>();
static std::unordered_map<std::wstring, std::wstring> LocalizedNameTable = std::unordered_map<std::wstring, std::wstring>();
//wstring GetLocalizedShortName(MonoString* id)
//{
//    auto res = LocalizedShortNameTable.find(id->chars);
//    if (res != LocalizedShortNameTable.end())
//    {
//        return res->second;
//    }
//    else
//    {
//        wstring newval = LocalizedShortName(id)->chars;
//        LocalizedShortNameTable[id->chars] = newval;
//        return newval;
//    }
//}
//wstring GetLocalized(MonoString* id)
//{
//    auto res = LocalizedNameTable.find(id->chars);
//    if (res != LocalizedNameTable.end())
//    {
//        return res->second;
//    }
//    else
//    {
//        wstring newval = LocalizedShortName(id)->chars;
//        LocalizedNameTable[id->chars] = newval;
//        return newval;
//    }
//}
static Defines::EItemType Object_GetType(void* obj)
{
    return GetItemType((ULONG64)mono_runtime_invoke(GetType, obj, NULL, NULL));
}
class CSharp
{
public:
    static ULONG64 GetTypeFromName(const char* _typename)
    {
        static ULONG64(*Type_GetType)(MonoString * type_name) = NULL;
        if (!Type_GetType)
        {
            auto metaClass = mono_class_from_name(mono_get_corlib(), "System", "Type");
            auto __method = mono_class_get_method_from_name(metaClass, "GetType", 1);
            MONOSETMETHOD(Type_GetType, __method);
            printf("Type_GetType=0x%X\n", Type_GetType);
        }
        return Type_GetType(mono_string_new(mono_get_root_domain(), _typename));
    }
    static void* GetCSharpType(void* image, const char* _classname)
    {
        void* monoType = mono_reflection_type_from_name((char*)_classname, image);
        if (monoType)
            return mono_type_get_object(mono_get_root_domain(), monoType);
        else
            return NULL;
    }
    CSharp()
    {
        mono_thread_attach(mono_get_root_domain());

        //_SHARP_DLL = LoadNETLibraryFromMemory(AC_dll, sizeof(AC_dll), false);
        //printf("csdll.assembly=%X;\tcsdll.image=%X\n", _SHARP_DLL.assembly, _SHARP_DLL.image);
        //if (_SHARP_DLL.assembly && _SHARP_DLL.image)
        //{
        //    printf("INIT...\n");
        //    auto klass = mono_class_from_name(_SHARP_DLL.image, "HK", "Helper");
        //    auto __method = mono_class_get_method_from_name(klass, "LocalizedShortName", 1);
        //    MONOSETMETHOD(LocalizedShortName, __method);
        //    __method = mono_class_get_method_from_name(klass, "LocalizedName", 1);
        //    MONOSETMETHOD(LocalizedName, __method);
        //    __method = mono_class_get_method_from_name(klass, "Localized", 1);
        //    MONOSETMETHOD(Localized, __method);
        //    printf("INIT->OK\n");
        //}
        auto object_class = mono_class_from_name(mono_get_corlib(), "System", "Object");
        GetType = mono_class_get_method_from_name(object_class, "GetType", 0);
        printf("-----------------------GetType=0x%X\n", GetType);

        auto ass = mono_domain_assembly_open(mono_get_root_domain(), "Assembly-CSharp");
        auto image = mono_assembly_get_image(ass);

        auto klass =FindClassForClassHasMethod(image, "get_SummaryLevel");
        auto __method = mono_class_get_method_from_name(klass, "SetCurrent", 2);
        MONOSETMETHOD(SetCurrent, __method);
        printf("----------------------SetCurrent=0x%X\n", SetCurrent);

        klass = mono_class_from_name(image, "EFT.InventoryLogic", "Item");
        auto fcs = FindFunction(klass, "GetAllVisibleItems");
        MONOSETMETHOD(_GetAllVisibleItems, fcs[0]);
        klass = FindClassForClassHasMethod(image, "LocalizedShortName");
        fcs = FindFunction(klass, "LocalizedName");
        MONOSETMETHOD(LocalizedName, fcs[1]);
        fcs = FindFunction(klass, "Localized");
        MONOSETMETHOD(Localized, fcs[0]);
        printf("----------------------LocalizedName=0x%X\n", LocalizedName);

        klass = mono_class_from_name(image, "EFT.UI.DragAndDrop", "ItemViewFactory");
        __method = mono_class_get_method_from_name(klass, "GetItemType", 1);
        MONOSETMETHOD(GetItemType, __method);
        printf("-----------------------GetItemType=0x%X\n", GetItemType);

        Minefield_Type = GetCSharpType(image, "EFT.Interactive.Minefield");
        ThermalVision_Type = GetCSharpType(image, "ThermalVision");
        NightVision_Type = GetCSharpType(image, "BSG.CameraEffects.NightVision");

        PlaceItemTrigger_Type = GetCSharpType(image, "EFT.Interactive.PlaceItemTrigger");
        TriggerWithId_Type = GetCSharpType(image, "BSG.Interactive.TriggerWithId");
        QuestTrigger_Type = GetCSharpType(image, "BSG.Interactive.QuestTrigger");
        ExperienceTrigger_Type = GetCSharpType(image, "BSG.Interactive.ExperienceTrigger");
    }
};