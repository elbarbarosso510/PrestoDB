#pragma once
#include <Windows.h>
typedef enum {
	MONO_IMAGE_OK,
	MONO_IMAGE_ERROR_ERRNO,
	MONO_IMAGE_MISSING_ASSEMBLYREF,
	MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;
typedef enum {
	MONO_TYPE_END = 0x00,       /* End of List */
	MONO_TYPE_VOID = 0x01,
	MONO_TYPE_BOOLEAN = 0x02,
	MONO_TYPE_CHAR = 0x03,
	MONO_TYPE_I1 = 0x04,
	MONO_TYPE_U1 = 0x05,
	MONO_TYPE_I2 = 0x06,
	MONO_TYPE_U2 = 0x07,
	MONO_TYPE_I4 = 0x08,
	MONO_TYPE_U4 = 0x09,
	MONO_TYPE_I8 = 0x0a,
	MONO_TYPE_U8 = 0x0b,
	MONO_TYPE_R4 = 0x0c,
	MONO_TYPE_R8 = 0x0d,
	MONO_TYPE_STRING = 0x0e,
	MONO_TYPE_PTR = 0x0f,       /* arg: <type> token */
	MONO_TYPE_BYREF = 0x10,       /* arg: <type> token */
	MONO_TYPE_VALUETYPE = 0x11,       /* arg: <type> token */
	MONO_TYPE_CLASS = 0x12,       /* arg: <type> token */
	MONO_TYPE_VAR = 0x13,	   /* number */
	MONO_TYPE_ARRAY = 0x14,       /* type, rank, boundsCount, bound1, loCount, lo1 */
	MONO_TYPE_GENERICINST = 0x15,	   /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
	MONO_TYPE_TYPEDBYREF = 0x16,
	MONO_TYPE_I = 0x18,
	MONO_TYPE_U = 0x19,
	MONO_TYPE_FNPTR = 0x1b,	      /* arg: full method signature */
	MONO_TYPE_OBJECT = 0x1c,
	MONO_TYPE_SZARRAY = 0x1d,       /* 0-based one-dim-array */
	MONO_TYPE_MVAR = 0x1e,       /* number */
	MONO_TYPE_CMOD_REQD = 0x1f,       /* arg: typedef or typeref token */
	MONO_TYPE_CMOD_OPT = 0x20,       /* optional arg: typedef or typref token */
	MONO_TYPE_INTERNAL = 0x21,       /* CLR internal type */

	MONO_TYPE_MODIFIER = 0x40,       /* Or with the following types */
	MONO_TYPE_SENTINEL = 0x41,       /* Sentinel for varargs method signature */
	MONO_TYPE_PINNED = 0x45,       /* Local var that points to pinned object */

	MONO_TYPE_ENUM = 0x55        /* an enumeration */
} MonoTypeEnum;
typedef enum {
	MONO_TOKEN_MODULE = 0x00000000,
	MONO_TOKEN_TYPE_REF = 0x01000000,
	MONO_TOKEN_TYPE_DEF = 0x02000000,
	MONO_TOKEN_FIELD_DEF = 0x04000000,
	MONO_TOKEN_METHOD_DEF = 0x06000000,
	MONO_TOKEN_PARAM_DEF = 0x08000000,
	MONO_TOKEN_INTERFACE_IMPL = 0x09000000,
	MONO_TOKEN_MEMBER_REF = 0x0a000000,
	MONO_TOKEN_CUSTOM_ATTRIBUTE = 0x0c000000,
	MONO_TOKEN_PERMISSION = 0x0e000000,
	MONO_TOKEN_SIGNATURE = 0x11000000,
	MONO_TOKEN_EVENT = 0x14000000,
	MONO_TOKEN_PROPERTY = 0x17000000,
	MONO_TOKEN_MODULE_REF = 0x1a000000,
	MONO_TOKEN_TYPE_SPEC = 0x1b000000,
	MONO_TOKEN_ASSEMBLY = 0x20000000,
	MONO_TOKEN_ASSEMBLY_REF = 0x23000000,
	MONO_TOKEN_FILE = 0x26000000,
	MONO_TOKEN_EXPORTED_TYPE = 0x27000000,
	MONO_TOKEN_MANIFEST_RESOURCE = 0x28000000,
	MONO_TOKEN_GENERIC_PARAM = 0x2a000000,
	MONO_TOKEN_METHOD_SPEC = 0x2b000000,
	MONO_TOKEN_STRING = 0x70000000,
	MONO_TOKEN_NAME = 0x71000000,
	MONO_TOKEN_BASE_TYPE = 0x72000000
}MonoTokenType;
typedef enum {
	MONO_TABLE_MODULE,
	MONO_TABLE_TYPEREF,
	MONO_TABLE_TYPEDEF,
	MONO_TABLE_FIELD_POINTER,
	MONO_TABLE_FIELD,
	MONO_TABLE_METHOD_POINTER,
	MONO_TABLE_METHOD,
	MONO_TABLE_PARAM_POINTER,
	MONO_TABLE_PARAM,
	MONO_TABLE_INTERFACEIMPL,
	MONO_TABLE_MEMBERREF, /* 0xa */
	MONO_TABLE_CONSTANT,
	MONO_TABLE_CUSTOMATTRIBUTE,
	MONO_TABLE_FIELDMARSHAL,
	MONO_TABLE_DECLSECURITY,
	MONO_TABLE_CLASSLAYOUT,
	MONO_TABLE_FIELDLAYOUT, /* 0x10 */
	MONO_TABLE_STANDALONESIG,
	MONO_TABLE_EVENTMAP,
	MONO_TABLE_EVENT_POINTER,
	MONO_TABLE_EVENT,
	MONO_TABLE_PROPERTYMAP,
	MONO_TABLE_PROPERTY_POINTER,
	MONO_TABLE_PROPERTY,
	MONO_TABLE_METHODSEMANTICS,
	MONO_TABLE_METHODIMPL,
	MONO_TABLE_MODULEREF, /* 0x1a */
	MONO_TABLE_TYPESPEC,
	MONO_TABLE_IMPLMAP,
	MONO_TABLE_FIELDRVA,
	MONO_TABLE_UNUSED6,
	MONO_TABLE_UNUSED7,
	MONO_TABLE_ASSEMBLY, /* 0x20 */
	MONO_TABLE_ASSEMBLYPROCESSOR,
	MONO_TABLE_ASSEMBLYOS,
	MONO_TABLE_ASSEMBLYREF,
	MONO_TABLE_ASSEMBLYREFPROCESSOR,
	MONO_TABLE_ASSEMBLYREFOS,
	MONO_TABLE_FILE,
	MONO_TABLE_EXPORTEDTYPE,
	MONO_TABLE_MANIFESTRESOURCE,
	MONO_TABLE_NESTEDCLASS,
	MONO_TABLE_GENERICPARAM, /* 0x2a */
	MONO_TABLE_METHODSPEC,
	MONO_TABLE_GENERICPARAMCONSTRAINT,
	MONO_TABLE_UNUSED8,
	MONO_TABLE_UNUSED9,
	MONO_TABLE_UNUSED10,
	/* Portable PDB tables */
	MONO_TABLE_DOCUMENT, /* 0x30 */
	MONO_TABLE_METHODBODY,
	MONO_TABLE_LOCALSCOPE,
	MONO_TABLE_LOCALVARIABLE,
	MONO_TABLE_LOCALCONSTANT,
	MONO_TABLE_IMPORTSCOPE,
	MONO_TABLE_STATEMACHINEMETHOD,
	MONO_TABLE_CUSTOMDEBUGINFORMATION

#define MONO_TABLE_LAST MONO_TABLE_CUSTOMDEBUGINFORMATION
#define MONO_TABLE_NUM (MONO_TABLE_LAST + 1)

} MonoMetaTableEnum;

typedef struct MonoVTable MonoVTable;
typedef struct _MonoThreadsSync MonoThreadsSync;
struct _MonoObject {
	MonoVTable* vtable;
	MonoThreadsSync* synchronisation;
};

typedef struct _MonoObject MonoObject;
struct _MonoString {
	MonoObject object;
	int32_t length;
	wchar_t chars[1];
	bool IsVaild()
	{
		return this != NULL &&
			!IsBadReadPtr(this, sizeof(_MonoString)) &&
			this->length > 0 &&
			this->length < 0x1000 &&
			!IsBadReadPtr(this->chars, sizeof(this->length * sizeof(wchar_t)));
	}
};
typedef struct _MonoString MonoString;
typedef void (*MonoFunc) (void* data, void* user_data);

static MonoString* (*mono_string_new)(void* domain, const char* text) = NULL;
static void* (*mono_get_root_domain)(void)=NULL;
static void* (*mono_type_get_object)(void* domain, void* type)=NULL;
static void* (*mono_class_get_type)(void* klass)=NULL;
static void* (*mono_object_get_class)(MonoObject* obj)=NULL;
static void* (*mono_class_get_method_from_name)(void* klass, const char* name, int param_count)=NULL;
static void* (*mono_class_from_name)(void* image, const char* name_space, const char* name)=NULL;
static void* (*mono_get_corlib)(void)=NULL;
static void* (*mono_array_new)(void* domain, void* eclass, uintptr_t n)=NULL;
static void* (*mono_runtime_invoke)(void* method, void* obj, void** params, MonoObject** exc)=NULL;
static void* (*mono_reflection_type_get_type)(void* reftype)=NULL;
static void* (*mono_class_from_mono_type)(void* type)=NULL;
static void* (*mono_domain_assembly_open)(void* domain, const char* name)=NULL;
static void* (*mono_assembly_get_image)(void* assembly)=NULL;
static void* (*mono_class_get_field_from_name)(void* klass, const char* name)=NULL;
static void* (*mono_reflection_type_from_name)(char* name, void* image)=NULL;
static void* (*mono_class_vtable)(void* domain, void* klass)=NULL;
static void* (*mono_class_get)(void* image, UINT32 type_token)=NULL;
static void* (*mono_class_get_fields)(void* klass, void** iter)=NULL;
static void* (*mono_image_open_from_data)(char* data, UINT32 data_len, bool need_copy, void* status)=NULL;
static void* (*mono_assembly_load_from)(void* image, const char* fname, void* status)=NULL;
static void* (*mono_class_get_methods)(void* klass, void** iter)=NULL;
static const char* (*mono_field_get_name)(void* field) = NULL;
static const char* (*mono_class_get_name)(void* klass) = NULL;
static const void* (*mono_image_get_table_info)(void* image, int table_id) = NULL;
static const char* (*mono_method_get_name)(void* method) = NULL;
static UINT32(*mono_class_get_type_token)(void* klass) = NULL;
static UINT32(*mono_class_get_field_token)(void* field) = NULL;
static UINT32(*mono_field_get_offset)(void* field) = NULL;
static int(*mono_table_info_get_rows)(const void* table) = NULL;
static int (*mono_jit_exec)(void* domain, void* assembly, int argc, char* argv[]) = NULL;
static void (*mono_assembly_foreach)(MonoFunc func, void* user_data) = NULL;
static void (*mono_thread_attach)(void* domain) = NULL;
static void* (*mono_compile_method)(void* method) = NULL;
static void* (*mono_class_get_field)(void* klass, UINT32 field_token) = NULL;
static char* (*mono_array_addr_with_size)(void* array, int size, unsigned __int64 idx) = NULL;
static char* (*mono_method_full_name)(void* method, bool signature) = NULL;
static void (*mono_field_static_set_value)(void* vt, void* field, void* value);
static void (*mono_field_static_get_value)(void* vt, void* field, void* value);

#define mono_array_addr(array,type,index) ((type*)mono_array_addr_with_size ((array), sizeof (type), (index)))
#define mono_array_set(array,type,index,value)	\
	do {	\
		type *__p = (type *) mono_array_addr ((array), type, (index));	\
		*__p = (value);	\
	} while (0)
static void InitMono()
{
#define INIT_MONO_METHOD(x) x = (decltype(x))GetProcAddress(hmodule, #x);printf(#x##"=0x%p\n",x);
	auto hmodule = LoadLibraryA("mono-2.0-bdwgc.dll");
	if (hmodule)
	{

		INIT_MONO_METHOD(mono_string_new);
		INIT_MONO_METHOD(mono_get_root_domain);
		INIT_MONO_METHOD(mono_type_get_object);
		INIT_MONO_METHOD(mono_class_get_type);
		INIT_MONO_METHOD(mono_object_get_class);
		INIT_MONO_METHOD(mono_class_get_method_from_name);
		INIT_MONO_METHOD(mono_class_from_name);
		INIT_MONO_METHOD(mono_get_corlib);
		INIT_MONO_METHOD(mono_array_new);
		INIT_MONO_METHOD(mono_runtime_invoke);
		INIT_MONO_METHOD(mono_reflection_type_get_type);
		INIT_MONO_METHOD(mono_class_from_mono_type);
		INIT_MONO_METHOD(mono_domain_assembly_open);
		INIT_MONO_METHOD(mono_assembly_get_image);
		INIT_MONO_METHOD(mono_class_get_field_from_name);
		INIT_MONO_METHOD(mono_reflection_type_from_name);
		INIT_MONO_METHOD(mono_class_vtable);
		INIT_MONO_METHOD(mono_class_get);
		INIT_MONO_METHOD(mono_class_get_fields);
		INIT_MONO_METHOD(mono_image_open_from_data);
		INIT_MONO_METHOD(mono_assembly_load_from);
		INIT_MONO_METHOD(mono_class_get_methods);
		INIT_MONO_METHOD(mono_field_get_name);
		INIT_MONO_METHOD(mono_class_get_name);
		INIT_MONO_METHOD(mono_image_get_table_info);
		INIT_MONO_METHOD(mono_method_get_name);
		INIT_MONO_METHOD(mono_class_get_type_token);
		INIT_MONO_METHOD(mono_class_get_field_token);
		INIT_MONO_METHOD(mono_field_get_offset);
		INIT_MONO_METHOD(mono_table_info_get_rows);
		INIT_MONO_METHOD(mono_jit_exec);
		INIT_MONO_METHOD(mono_assembly_foreach);
		INIT_MONO_METHOD(mono_thread_attach);
		INIT_MONO_METHOD(mono_compile_method);
		INIT_MONO_METHOD(mono_class_get_field);
		INIT_MONO_METHOD(mono_array_addr_with_size);
		INIT_MONO_METHOD(mono_method_full_name);
		INIT_MONO_METHOD(mono_field_static_set_value);
		INIT_MONO_METHOD(mono_field_static_get_value);
	}
}