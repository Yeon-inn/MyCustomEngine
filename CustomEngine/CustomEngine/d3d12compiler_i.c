

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ID3D12CompilerFactoryChild,0xe0d06420,0x9f31,0x47e8,0xae,0x9a,0xdd,0x2b,0xa2,0x5a,0xc0,0xbc);


MIDL_DEFINE_GUID(IID, IID_ID3D12CompilerCacheSession,0x5704e5e6,0x054b,0x4738,0xb6,0x61,0x7b,0x0d,0x68,0xd8,0xdd,0xe2);


MIDL_DEFINE_GUID(IID, IID_ID3D12CompilerStateObject,0x5981cca4,0xe8ae,0x44ca,0x9b,0x92,0x4f,0xa8,0x6f,0x5a,0x3a,0x3a);


MIDL_DEFINE_GUID(IID, IID_ID3D12Compiler,0x8c403c12,0x993b,0x4583,0x80,0xf1,0x68,0x24,0x13,0x8f,0xa6,0x8e);


MIDL_DEFINE_GUID(IID, IID_ID3D12CompilerFactory,0xc1ee4b59,0x3f59,0x47a5,0x9b,0x4e,0xa8,0x55,0xc8,0x58,0xa8,0x78);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



