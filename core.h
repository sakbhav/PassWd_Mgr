#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
wchar_t pass[100];
int symch[100],symchcou=0;
bool strcontain(int check)
{
	for(int i=0;i<symchcou;i++)
	{
		if(symch[i]==check)
			return true;
	}
	return false;
}
wchar_t * core(int got,int nget,int sget,char nset[],char set[],int tim,char sym[])
{
	symchcou=0;
	int counter = 0,a,num,nnum,nsym,deb=0,b=0,icou=0;
	num = strlen(set);
	num--;
	nsym = strlen(sym);
	nnum = strlen(nset);
	nnum--;
	srand(tim);
	while(counter<got)
	{
		a=rand()%num;
		pass[counter]=set[a];
        counter++;
	};
	counter=0;
	while(counter<nget)
	{
		a=rand()%got;
		while(pass[a]=='1'||pass[a]=='2'||pass[a]=='3'||pass[a]=='4'||pass[a]=='5'||pass[a]=='6'||pass[a]=='7'||pass[a]=='8'||pass[a]=='9'||pass[a]=='0')
		{
			a=rand()%got;
		};
		b=rand()%nnum;
		pass[a]=nset[b];
		counter++;
	};
	counter=0;
	while(counter<sget)
	{
		a=rand()%got;
		while(pass[a]=='1'||pass[a]=='2'||pass[a]=='3'||pass[a]=='4'||pass[a]=='5'||pass[a]=='6'||pass[a]=='7'||pass[a]=='8'||pass[a]=='9'||pass[a]=='0'||strcontain(a))
		{
			a=rand()%got;
		};
		b=rand()%nsym;
		pass[a]=sym[b];
		symch[symchcou]=a;
		symchcou++;
		counter++;
	};
	pass[got]=0;
    return pass;
}
