

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for ..\..\..\DirectX-Headers-main\DirectX-Headers-main\include\directx\d3dshadercacheregistration.idl:
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

MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheInstallerClient,0xa16ee930,0xd9f6,0x4222,0xa5,0x14,0x24,0x44,0x73,0xe5,0xd2,0x66);


MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheComponent,0xeed1bf00,0xf5c7,0x4cf7,0x88,0x5c,0xd0,0xf9,0xc0,0xcb,0x48,0x28);


MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheApplication,0xfc688ee2,0x1b35,0x4913,0x93,0xbe,0x1c,0xa3,0xfa,0x7d,0xf3,0x9e);


MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheInstaller,0xbbe30de1,0x6318,0x4526,0xae,0x17,0x77,0x66,0x93,0x19,0x1b,0xb4);


MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheExplorer,0x90432322,0x32f5,0x487f,0x92,0x64,0xe9,0x39,0x0f,0xa5,0x8b,0x2a);


MIDL_DEFINE_GUID(IID, IID_ID3DShaderCacheInstallerFactory,0x09b2dfe4,0x840f,0x401a,0x80,0x4c,0x0d,0xd8,0xaa,0xdc,0x9e,0x9f);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



