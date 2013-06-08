//
// Encrypt and Decrypt - Using Windows Crypto API
// 
#pragma warning(disable:4715)
#pragma warning(disable:4996)
#pragma warning(disable:4995)
#pragma warning(disable:4018)
#include <windows.h>
#include <wincrypt.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include "Pass.h"
typedef struct {
	BYTE digest[16];
	HCRYPTHASH hHash;
} MD5Context;
inline void md5fun(wchar_t * enstr,wchar_t * md5l)
{
	DWORD dwCount = 16;
	BYTE b,c;
	int j,n=0;
	MD5Context pm;
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash);
	CryptHashData(pm.hHash, (PBYTE)enstr, wcslen(enstr), 0);
	CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0);
	int i;
	for(i = 0; i < 16; i++){
		b = pm.digest[i];
		for(j = 4; j >= 0; j -= 4)
		{
			c = ((char)(b >> j) & 0x0F);
			if(c < 10) c += '0';
			else c = ('a' + (c - 10));
			swprintf(&md5l[n],L"%c", c);
			n++;
		}
	}
};

inline int encrypt(wchar_t * da,wchar_t * str)
{
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
	DWORD dwCount = 16;
	MD5Context pm;
	if(!CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return 0;
	memset(&pm, 0, sizeof(MD5Context));
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash))
		return 0;
	if(!CryptHashData(pm.hHash, (PBYTE)da, wcslen(da), 0))
		return 0;
	if(!CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0))
		return 0;	
	if(!CryptDeriveKey(hCryptProv, CALG_RC4,pm.hHash,CRYPT_EXPORTABLE,&hKey))
		return 0;
	DWORD le =wcslen(str);
	DWORD a=le*2;
	if(!CryptEncrypt(hKey,0,TRUE,0,(PBYTE)str,&a,a))
		return 0;
}

inline int decrypt(wchar_t * da,wchar_t * str)
{
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
	DWORD dwCount = 16;
	MD5Context pm;
	if(!CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return 0;
	memset(&pm, 0, sizeof(MD5Context));
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash))
		return 0;
	if(!CryptHashData(pm.hHash, (PBYTE)da, wcslen(da), 0))
		return 0;
	if(!CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0))
		return 0;	
	if(!CryptDeriveKey(hCryptProv, CALG_RC4,pm.hHash,CRYPT_EXPORTABLE,&hKey))
		return 0;
	DWORD le =wcslen(str);
	DWORD a=le*2;
	if(!CryptDecrypt(hKey,0,TRUE,0,(PBYTE)str,&a))
		return 0;
}