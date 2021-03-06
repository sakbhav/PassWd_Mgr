﻿/*	Copyright 2013 Saket Srivastava
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*/


#pragma warning(disable:4715)
#pragma warning(disable:4996)
#pragma warning(disable:4995)
#pragma warning(disable:4018)

#include "crypt-AES\Rijndael.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <wincrypt.h>
#include <string.h>
typedef struct {
	BYTE digest[17];
	HCRYPTHASH hHash;
} MD5Context;
 void md5fun(wchar_t * enstr,wchar_t * md5l)
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

extern wchar_t * encryptaes(wchar_t * key,wchar_t * data)
{
	DWORD dwCount = 16;
	CRijndael crypt;
	MD5Context pm;
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash);
	CryptHashData(pm.hHash, (PBYTE)key, wcslen(key), 0);
	CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0);
	int len = wcslen(data);
	int temp,i,pad;
	if(len%16!=0)
		temp=(len/16)+1;
	else
		temp=len/16;
	pad = temp*16;
	char * str = new char[pad+1];
	char * str2 = new char[pad+1];
	for(i=0;i<pad;i++)
		str[i]=wctob(data[i]);
	for(i=len;i<pad;i++)
		str[i]='/';
	str[i]=0;
	crypt.MakeKey((char *)pm.digest,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",16,16);
	crypt.Encrypt(str,str2,pad,0);
	data = new wchar_t[pad+1];
	mbstowcs(data,str2,pad);
	data[pad] = 0;
	return data;
}
wchar_t * decryptaes(wchar_t * key,wchar_t * data)
{
	DWORD dwCount = 16;
	int counter=0;
	int i=0;
	CRijndael crypt;
	MD5Context pm;
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &pm.hHash);
	CryptHashData(pm.hHash, (PBYTE)key, wcslen(key), 0);
	CryptGetHashParam(pm.hHash, HP_HASHVAL, pm.digest, &dwCount, 0);
	int len = wcslen(data);
	char * str = new char[len];
	char * str2 = new char[len];
	wcstombs(str,data,len);
	if(!(len%16))
	{
		crypt.MakeKey((char *)pm.digest,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",16,16);
		crypt.Decrypt(str,str2,len,0);
	}
	for(i=len;str2[i]=='/';i--);
	str2[i+1]=0;
	data = new wchar_t[i+2];
	mbstowcs(data,str2,i);
	for(int j=0;j<i+1;j++)
	{
		if(data[j]==(wchar_t)255)
		{
			if(!counter%5)
			{
				data[j]=L'▲';
				counter++;
			}
			else
			{
				data[j]=L'↔';
				counter++;
			}
		}
	}
	data[i+1] = 0;
	return data;
}
wchar_t * decryptrc4(wchar_t * da,wchar_t * str)
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
	return str;
}