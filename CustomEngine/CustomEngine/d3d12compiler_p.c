

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for ..\..\..\DirectX-Headers-main\DirectX-Headers-main\include\directx\d3d12compiler.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#include "ndr64types.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "d3d12compiler_h.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _d3d12compiler_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } d3d12compiler_MIDL_TYPE_FORMAT_STRING;

typedef struct _d3d12compiler_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } d3d12compiler_MIDL_PROC_FORMAT_STRING;

typedef struct _d3d12compiler_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } d3d12compiler_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax_2_0 = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax_1_0 = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, (ObjectType *)pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, (ObjectType *)pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol
#endif



extern const d3d12compiler_MIDL_TYPE_FORMAT_STRING d3d12compiler__MIDL_TypeFormatString;
extern const d3d12compiler_MIDL_PROC_FORMAT_STRING d3d12compiler__MIDL_ProcFormatString;
extern const d3d12compiler_MIDL_EXPR_FORMAT_STRING d3d12compiler__MIDL_ExprFormatString;



#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const d3d12compiler_MIDL_PROC_FORMAT_STRING d3d12compiler__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const d3d12compiler_MIDL_TYPE_FORMAT_STRING d3d12compiler__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ID3D12CompilerFactoryChild, ver. 0.0,
   GUID={0xe0d06420,0x9f31,0x47e8,{0xae,0x9a,0xdd,0x2b,0xa2,0x5a,0xc0,0xbc}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: ID3D12CompilerCacheSession, ver. 0.0,
   GUID={0x5704e5e6,0x054b,0x4738,{0xb6,0x61,0x7b,0x0d,0x68,0xd8,0xdd,0xe2}} */


/* Object interface: ID3D12CompilerStateObject, ver. 0.0,
   GUID={0x5981cca4,0xe8ae,0x44ca,{0x9b,0x92,0x4f,0xa8,0x6f,0x5a,0x3a,0x3a}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0003, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: ID3D12Compiler, ver. 0.0,
   GUID={0x8c403c12,0x993b,0x4583,{0x80,0xf1,0x68,0x24,0x13,0x8f,0xa6,0x8e}} */


/* Object interface: ID3D12CompilerFactory, ver. 0.0,
   GUID={0xc1ee4b59,0x3f59,0x47a5,{0x9b,0x4e,0xa8,0x55,0xc8,0x58,0xa8,0x78}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0005, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for ..\..\..\DirectX-Headers-main\DirectX-Headers-main\include\directx\d3d12compiler.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)




#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"
#ifdef __cplusplus
namespace {
#endif


typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;
#ifdef __cplusplus
}
#endif


#include "poppack.h"



/* Standard interface: __MIDL_itf_d3d12compiler_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ID3D12CompilerFactoryChild, ver. 0.0,
   GUID={0xe0d06420,0x9f31,0x47e8,{0xae,0x9a,0xdd,0x2b,0xa2,0x5a,0xc0,0xbc}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: ID3D12CompilerCacheSession, ver. 0.0,
   GUID={0x5704e5e6,0x054b,0x4738,{0xb6,0x61,0x7b,0x0d,0x68,0xd8,0xdd,0xe2}} */


/* Object interface: ID3D12CompilerStateObject, ver. 0.0,
   GUID={0x5981cca4,0xe8ae,0x44ca,{0x9b,0x92,0x4f,0xa8,0x6f,0x5a,0x3a,0x3a}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0003, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: ID3D12Compiler, ver. 0.0,
   GUID={0x8c403c12,0x993b,0x4583,{0x80,0xf1,0x68,0x24,0x13,0x8f,0xa6,0x8e}} */


/* Object interface: ID3D12CompilerFactory, ver. 0.0,
   GUID={0xc1ee4b59,0x3f59,0x47a5,{0x9b,0x4e,0xa8,0x55,0xc8,0x58,0xa8,0x78}} */


/* Standard interface: __MIDL_itf_d3d12compiler_0000_0005, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    d3d12compiler__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x8010274, /* MIDL Version 8.1.628 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif

const CInterfaceProxyVtbl * const _d3d12compiler_ProxyVtblList[] = 
{
    0
};

const CInterfaceStubVtbl * const _d3d12compiler_StubVtblList[] = 
{
    0
};

PCInterfaceName const _d3d12compiler_InterfaceNamesList[] = 
{
    0
};


#define _d3d12compiler_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _d3d12compiler, pIID, n)

int __stdcall _d3d12compiler_IID_Lookup( const IID * pIID, int * pIndex )
{
    UNREFERENCED_PARAMETER(pIID);
    UNREFERENCED_PARAMETER(pIndex);
    return 0;
}

EXTERN_C const ExtendedProxyFileInfo d3d12compiler_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _d3d12compiler_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _d3d12compiler_StubVtblList,
    (const PCInterfaceName * ) & _d3d12compiler_InterfaceNamesList,
    0, /* no delegation */
    & _d3d12compiler_IID_Lookup, 
    0,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

