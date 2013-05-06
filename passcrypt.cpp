//
// MD5 Hashing - Using Windows Crypto API
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
//extern char md5[32];
typedef struct {
	BYTE digest[16];
	HCRYPTHASH hHash;
} MD5Context;
//extern wchar_t md5l[33];
//extern HCRYPTPROV hCryptProv;
//char md5[32];
//extern HCRYPTPROV hCryptProv;
BOOL CryptStartup(HCRYPTPROV *);
//void CryptCleanup(HCRYPTPROV *);
void MD5Init(MD5Context *,HCRYPTPROV *);
void MD5Update(MD5Context *, unsigned char const *, unsigned int);
void MD5Final(MD5Context *);

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
//	int i,j,d,de;
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
//	unsigned char b;
	DWORD dwCount = 16;
//	char md5[33];
//	char c;
	MD5Context pm;
//	int n=0;
//	CryptStartup(hCryptProv);
	if(!CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return 0;
	memset(&pm, 0, sizeof(MD5Context));
//	MD5Init(&pm,hCryptProv);
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash))
		return 0;
//	MD5Update(&pm,da,strlen((const char *)da));
	if(!CryptHashData(pm.hHash, (PBYTE)da, wcslen(da), 0))
		return 0;
//	MD5Final(&pm);
	if(!CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0))
		return 0;
/*	for(i = 0; i < 16; i++){
		b = pm.digest[i];
		for(j = 4; j >= 0; j -= 4){
			c = ((char)(b >> j) & 0x0F);
			if(c < 10) c += '0';
			else c = ('a' + (c - 10));
			sprintf(&md5[n],"%c", c);
			n++;
		}
	}
*/	
	if(!CryptDeriveKey(hCryptProv, CALG_RC4,/*(HCRYPTHASH)md5*/pm.hHash,CRYPT_EXPORTABLE,&hKey))
		return 0;
	DWORD le =wcslen(str);
//	char strc[]="abcde";
/*	char * strc=new char[le];
	for(int n=0;n<le;n++)
	{
		wctomb(strc,str[n]);
		strc++;
	}
	DWORD sizeDest;
	CryptEncrypt(hKey,0,TRUE,0,NULL,&sizeDest,le);*/
//	char * buff = new char[le];
//	wchar_t * buffTemp = new wchar_t[le];
//	wctomb(buff,str,le);
//	buff[le]='\0';
//	memset(buffTemp,0,le);
//	memcpy(buffTemp,str,le*2);
//	buffTemp[le]=0;
	DWORD a=le*2;
//	buffTemp[5]='\0';
	if(!CryptEncrypt(hKey,0,TRUE,0,(PBYTE)str,&a,a))
		return 0;
/*	CryptEncrypt(hKey, NULL, LAST_DATA, 0, pData, &dwLen, 1024)
//	CryptCleanup(*hCryptProv);
//	return buffTemp;
	if(!CryptDecrypt(hKey,0,TRUE,0,(PBYTE)str,&a))
		return 0;*/
}

inline int decrypt(wchar_t * da,wchar_t * str)
{
//	int i,j,d,de;
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
//	unsigned char b;
	DWORD dwCount = 16;
//	char md5[33];
//	char c;
	MD5Context pm;
//	int n=0;
//	CryptStartup(hCryptProv);
	if(!CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return 0;
	memset(&pm, 0, sizeof(MD5Context));
//	MD5Init(&pm,hCryptProv);
	if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash))
		return 0;
//	MD5Update(&pm,da,strlen((const char *)da));
	if(!CryptHashData(pm.hHash, (PBYTE)da, wcslen(da), 0))
		return 0;
//	MD5Final(&pm);
	if(!CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0))
		return 0;
/*	for(i = 0; i < 16; i++){
		b = pm.digest[i];
		for(j = 4; j >= 0; j -= 4){
			c = ((char)(b >> j) & 0x0F);
			if(c < 10) c += '0';
			else c = ('a' + (c - 10));
			sprintf(&md5[n],"%c", c);
			n++;
		}
	}
*/	
	if(!CryptDeriveKey(hCryptProv, CALG_RC4,/*(HCRYPTHASH)md5*/pm.hHash,CRYPT_EXPORTABLE,&hKey))
		return 0;
	DWORD le =wcslen(str);
//	char strc[]="abcde";
/*	char * strc=new char[le];
	for(int n=0;n<le;n++)
	{
		wctomb(strc,str[n]);
		strc++;
	}
	DWORD sizeDest;
	CryptEncrypt(hKey,0,TRUE,0,NULL,&sizeDest,le);*/
//	char * buff = new char[le];
//	wchar_t * buffTemp = new wchar_t[le];
//	wctomb(buff,str,le);
//	buff[le]='\0';
//	memset(buffTemp,0,le);
//	memcpy(buffTemp,str,le*2);
//	buffTemp[le]=0;
	DWORD a=le*2;
//	buffTemp[5]='\0';
/*	if(!CryptEncrypt(hKey,0,TRUE,0,(PBYTE)str,&a,a))
		return 0;*/
//	CryptEncrypt(hKey, NULL, LAST_DATA, 0, pData, &dwLen, 1024)
//	CryptCleanup(*hCryptProv);
//	return buffTemp;
	if(!CryptDecrypt(hKey,0,TRUE,0,(PBYTE)str,&a))
		return 0;
}

/*inline BOOL CryptStartup(HCRYPTPROV * hCryptProv)
{
	if(CryptAcquireContext(hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == 0){
		if(GetLastError() == NTE_EXISTS){
			if(CryptAcquireContext(hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == 0)
				return FALSE;
		}else return FALSE;
	}
	return TRUE;
}

/*inline void CryptCleanup(HCRYPTPROV hCryptProv)
{
	if(hCryptProv) CryptReleaseContext(hCryptProv, 0);
	hCryptProv = 0;
}

inline void MD5Init(MD5Context *ctx,HCRYPTPROV * hCryptProv)
{
	CryptCreateHash(*hCryptProv, CALG_MD5, 0, 0, &ctx->hHash);
}


inline void MD5Update(MD5Context *ctx, unsigned char const *buf, unsigned int len)
{
	CryptHashData(ctx->hHash, buf, len, 0);
}

inline void MD5Final(MD5Context *ctx)
{
	DWORD dwCount = 16;
	CryptGetHashParam(ctx->hHash, HP_HASHVAL, ctx->digest, &dwCount, 0);
/*	if(ctx->hHash) CryptDestroyHash(ctx->hHash);
	ctx->hHash = 0;
}
/*
int main(int argc, char *argv[])se
{
	int i, j;
	FILE *fInput;
	MD5Context md5Hash;
	unsigned char bBuffer[4096];
    unsigned char b;
	char c;
	
	if(argc < 2){
		printf("Usage: checksum [path]\n\te.g: checksum c:\\file.exe\n\n");
		return 0;
    }
	
	if(!CryptStartup()){
		printf("Error: Could not start crypto library.\n\n", argv[1]);
		return 0;
    }
	
	fInput = fopen(argv[1], "rb");
	if(!fInput){
		printf("Error: Failed to open '%s'\n\n", argv[1]);
		CryptCleanup();
		return 0;
    }
	
	memset(&md5Hash, 0, sizeof(MD5Context));
	MD5Init(&md5Hash);
	while(!feof(fInput)){
		unsigned int nCount = fread(bBuffer, sizeof(unsigned char), 4096, fInput);
		MD5Update(&md5Hash, bBuffer, nCount);
	}
	MD5Final(&md5Hash);
	
	fclose(fInput);
	printf("\nChecksum of '%s' is: ", argv[1]);
	for(i = 0; i < 16; i++){
		b = md5Hash.digest[i];
		for(j = 4; j >= 0; j -= 4){
			c = ((char)(b >> j) & 0x0F);
			if(c < 10) c += '0';
			else c = ('a' + (c - 10));
			printf("%c", c);
		}
	}
	printf("\n");
	
	CryptCleanup();
    return 0;
}*/

