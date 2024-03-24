#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "MonoRuntime.h"

#define MONOMETHOD(t,x) (decltype(t))mono_compile_method(x);
#define MONOSETMETHOD(t,x) t= MONOMETHOD(t,x)

using namespace std;
//typedef struct MonoObject
//{
//    void* vtable;
//    void* synchronisation;
//};
//typedef struct MonoArrayBounds
//{
//    mono_array_size_t length;
//    mono_array_lower_bound_t lower_bound;
//} ;
//typedef struct MonoArray
//{
//    MonoObject obj;
//    MonoArrayBounds* bounds;
//    mono_array_size_t max_length;
//    ULONG64 vector[MONO_ZERO_LEN_ARRAY];
//};
static vector<void*> GetAllClass(void* image)
{
    vector<void*> result = vector<void*>();
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++) {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        result.push_back(klass);
    }
    return result;
}
static vector<void*> GetClassFilds(void* klass)
{
    vector<void*> result = vector<void*>();
    void* iter = NULL;
    void* raw_field = NULL;
    while ((raw_field = mono_class_get_fields(klass, &iter)) != NULL)
    {
        result.push_back(raw_field);
    }
    return result;
}
static vector<void*> GetClassFunctions(void* klass)
{
    vector<void*> result = vector<void*>();
    void* iter = NULL;
    void* method = NULL;
    while ((method = mono_class_get_methods(klass, &iter)) != NULL)
    {
        result.push_back(method);
    }
    return result;
}
static void* FindClass(void* image, const char* classname)
{
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++) {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        void* iter = NULL;
        auto name = mono_class_get_name(klass);
        if (strcmp(name, classname) == 0)
        {
            return klass;
        }
    }
    return nullptr;
}
static void* FindClass(void* image, const unsigned char* classname, int namelen)
{
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++) {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        void* iter = NULL;
        auto name = mono_class_get_name(klass);
        if (!IsBadReadPtr((void*)name, namelen) && memcmp(name, classname, namelen) == 0)
        {
            return klass;
        }
    }
    return nullptr;
}
static void* FindClassForClassHasMethod(void* image, const char* methodname)
{
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++)
    {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        void* iter = NULL;
        void* method;
        while (method = mono_class_get_methods(klass, &iter))
        {
            auto m_name = mono_method_get_name(method);
            if (strcmp(m_name, methodname) == 0)
            {
                return klass;
            }
        }
    }
    return nullptr;
}
static vector <void*> FindFunction(void* klass, const char* name)
{
    vector <void*> result = vector <void*>();
    auto functions = GetClassFunctions(klass);
    for (auto f : functions)
    {
        auto fname = mono_method_get_name(f);
        if (strcmp(name, fname) == 0)
        {
            result.push_back(f);
        }
    }
    return result;
}
static vector <void*> FindFunction(void* image, const char* kname, const char* procname)
{
    vector <void*> result = vector <void*>();
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++)
    {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        void* iter = NULL;
        auto name = mono_class_get_name(klass);
        if (strcmp(kname, name) == 0)
        {
            auto functions = GetClassFunctions(klass);
            for (auto f : functions)
            {
                auto fname = mono_method_get_name(f);
                if (strcmp(procname, fname) == 0)
                {
                    result.push_back(f);
                }
            }
        }
    }
    return result;
}
static vector <void*> FindFunction(void* image,
    const unsigned char* kname, int classnamelen,
    const unsigned char* _name, int procnamelen)
{
    vector <void*> result = vector <void*>();
    auto t = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    for (int i = 1; i < mono_table_info_get_rows(t); i++)
    {
        auto klass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | (i + 1));
        void* iter = NULL;
        auto name = mono_class_get_name(klass);
        if (!IsBadReadPtr(name, classnamelen) && memcmp(kname, name, classnamelen) == 0)
        {
            auto functions = GetClassFunctions(klass);
            for (auto f : functions)
            {
                auto fname = mono_method_get_name(f);
                if (!IsBadReadPtr(fname, procnamelen) && memcmp(_name, fname, procnamelen) == 0)
                {
                    result.push_back(f);
                }
            }
        }
    }
    return result;
}
static vector <void*> FindFunction(void* klass, const unsigned char* name, int namelen)
{
    vector <void*> result = vector <void*>();
    void* iter = NULL;
    void* raw_field = NULL;
    while ((raw_field = mono_class_get_methods(klass, &iter)) != NULL)
    {
        auto fname = mono_method_get_name(raw_field);
        if (!IsBadReadPtr((void*)fname, namelen) && memcmp(name, fname, namelen) == 0)
        {
            result.push_back(raw_field);
        }
    }
    return result;
}
static void* FindClassFilds(void* klass, const char* name)
{
    vector<void*> result = vector<void*>();
    void* iter = NULL;
    void* raw_field = NULL;
    while ((raw_field = mono_class_get_fields(klass, &iter)) != NULL)
    {
        auto fname = mono_field_get_name(raw_field);
        if (strcmp(fname, name) == 0) return raw_field;
    }
    return nullptr;
}
static void* FindClassFilds(void* klass, const unsigned char* name, int namelen)
{
    vector<void*> result = vector<void*>();
    void* iter = NULL;
    void* raw_field = NULL;
    while ((raw_field = mono_class_get_fields(klass, &iter)) != NULL)
    {
        auto fname = mono_field_get_name(raw_field);
        if (!IsBadReadPtr((void*)fname, namelen) && memcmp(name, fname, namelen) == 0) return raw_field;
    }
    return nullptr;
}
static int GetFieldOffset(void* image, const char* classname, const char* fildname)
{
    void* klass = FindClass(image, classname);
    if (!klass) return -1;
    void* field = FindClassFilds(klass, fildname);
    if (field) return mono_field_get_offset(field);
    return -1;
}

static vector<void*> FindClassWhereContainsFunction(void* _image, const char* procname)
{
    vector<void*> result = vector<void*>();
    auto all_classes = GetAllClass(_image);
    for (auto kls : all_classes)
    {
        auto all_function = GetClassFunctions(kls);
        for (auto fks : all_function)
        {
            if (strcmp(mono_method_get_name(fks), procname) == 0)
            {
                result.push_back(kls);
                break;
            }
        }
    }
    return result;
}

typedef struct SharpDll
{
    void* assembly;
    void* image;
};
static SharpDll LoadNETLibraryFromMemory(unsigned char* buffer, int size, bool needcopy = false)
{
    auto domain = mono_get_root_domain();
    if (!domain) return { 0,0 };
    MonoImageOpenStatus status;
    void* image = mono_image_open_from_data((char*)buffer, size, needcopy, &status);
    if (!image) return { 0,0 };
    if (status != MONO_IMAGE_OK) return { 0,0 };
    auto ass = mono_assembly_load_from(image, "", &status);
    SharpDll _dll= { ass ,image };
    char* argv[1];
    argv[0] = (char*)"AE.exe";
    mono_jit_exec(domain, ass, 1, argv);
    return _dll;
}
static void enumerator(void* data, void* user_data)
{
    std::vector<void*>* vec = (std::vector<void*>*)user_data;
    vec->push_back(data);
}
static void* find_class(const char* klass_name, const char* name_space = "")
{
    const auto domain = mono_get_root_domain();

    std::vector<void*> v;
    mono_assembly_foreach(enumerator, &v);

    for (auto assembly : v)
    {
        if (!assembly)
            continue;

        const auto img = mono_assembly_get_image((void*)assembly);
        if (!img)
            continue;

        const auto klass = mono_class_from_name(img, name_space, klass_name);
        if (!klass)
            continue;

        return klass;
    }

    return NULL;
}

static void* method_without_stupid(const std::string namespace_name, const std::string klass_name, const std::string method_name, int argument_number = -1)
{
    std::uintptr_t iter{};

    const auto klass = find_class(klass_name.c_str(), namespace_name.c_str());
    if (!klass)
        return{ };

    const auto method = mono_class_get_method_from_name(klass, method_name.c_str(), argument_number);
    if (!method)
        return { };

    return mono_compile_method(method);
}

#define MONO_METHOD_DIRECT( fn_name, namespace_, class_name, name, arg_count, args ) \
	static auto fn_name = reinterpret_cast< args >( method_without_stupid( namespace_, class_name, name, arg_count ) )

struct sanitized_method_t { std::string ns, kl, mt; };
static auto sanitize_method_name(const std::string name) -> const sanitized_method_t
{
    std::string ns, kl, method_name;

    if (auto ns_break = name.find(("::")))
        ns = name.substr(0, ns_break);

    if (name.find(("::")) == std::string::npos)
        ns = ("");

    if (name.find(("::")))
    {
        kl = name.substr(name.find(("::"), 2) + 2);
        kl = kl.substr(0, kl.find(("."), 0));
    }

    if (name.find(("::")) == std::string::npos)
        kl = name.substr(0, name.find(("."), 0));

    method_name = name.substr(name.find(("."), 0) + 1);

    if (method_name.find(("()")))
        method_name.erase(method_name.find(("()")), 2);

    return { ns, kl, method_name };
}
static auto method(const std::string name, int argument_number = -1) -> const void*
{
    std::uintptr_t iter{};

    const auto st = sanitize_method_name(name);
    const auto klass = find_class(st.kl.c_str(), st.ns.c_str());
    if (!klass)
        return NULL;

    const auto method = mono_class_get_method_from_name(klass, st.mt.c_str(), argument_number);
    if (!method)
        return NULL;

    return mono_compile_method(method);
}
#define MONO_METHOD( fn_name, name, arg_count, args ) \
	static auto fn_name = reinterpret_cast< args >( method( name, arg_count ) )
class c_shader
{
public:
    static auto find(MonoString name) -> c_shader*
    {
        MONO_METHOD(find_shader_fn, ("UnityEngine::Shader.Find()"), -1, c_shader * (*)(MonoString));
        return find_shader_fn(name);
    }
};
class c_material
{
public:
    auto get_shader() -> c_shader* {
        MONO_METHOD(get_shader_fn, ("UnityEngine::Material.get_shader()"), -1, c_shader * (*)(void*));
        return get_shader_fn(this);
    }

    auto set_shader(c_shader* shader) -> void {
        MONO_METHOD(set_shader_fn, ("UnityEngine::Material.set_shader()"), -1, void(*)(void*, void*));
        return set_shader_fn(this, shader);
    }

    auto set_float(MonoString prop, float value) -> void
    {
        MONO_METHOD(set_float_fn, ("UnityEngine::Material.SetFloat()"), -1, void(*)(void*, MonoString, float));
        return set_float_fn(this, prop, value);
    }

    auto set_int(MonoString prop, int value) -> void {
        MONO_METHOD(set_int_fn, ("UnityEngine::Material.SetInt()"), -1, void(*)(void*, MonoString, int));
        return set_int_fn(this, prop, value);
    }

    template<typename t>
    auto set_color(MonoString prop, t color) -> void {
        MONO_METHOD(set_color_fn, ("UnityEngine::Material.SetColor()"), -1, void(*)(void*, MonoString, t));
        return set_color_fn(this, prop, color);
    }

    auto set_texture(MonoString prop, std::uintptr_t texture) -> void {
        MONO_METHOD(set_texture_fn, ("UnityEngine::Material.SetTexture()"), 2, void(*)(void*, MonoString, std::uintptr_t));
        return set_texture_fn(this, prop, texture);
    }
};