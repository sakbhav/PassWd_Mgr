#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <wincrypt.h>
#include <string.h>
typedef struct {
	BYTE digest[17];
	HCRYPTHASH hHash;
} MD5Context;

void md5fun(wchar_t *,wchar_t *);
wchar_t * encryptaes(wchar_t *,wchar_t *);
wchar_t * decryptaes(wchar_t *,wchar_t *);