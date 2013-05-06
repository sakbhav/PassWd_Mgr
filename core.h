#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
wchar_t pass[100];
wchar_t * core(int got,int nget,int sget,char nset[],char set[],int tim,char sym[])
{
	int counter = 0,a,num,nnum,nsym,deb=0,b=0,icou=0;
	//printf("%s\t%%d",nset,nget);
	while(deb<100)
	{
		pass[deb]=NULL;
		deb++;
	};
    //char set[] ="abcdefghijklmnopqrstuvwxyz";
	num = strlen(set);
	num--;
	nsym = strlen(sym);
	//nsym--;
	nnum = strlen(nset);
	nnum--;
	srand(tim);
	//printf("%d\n",time(NULL));
	while(counter<got)
	{
		a=rand()%num;
		pass[counter]=set[a];
		//printf("%d\t%d\t%c\n",counter+1,a+1,set[a]);
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
		again:
		a=rand()%got;
		while(pass[a]=='1'||pass[a]=='2'||pass[a]=='3'||pass[a]=='4'||pass[a]=='5'||pass[a]=='6'||pass[a]=='7'||pass[a]=='8'||pass[a]=='9'||pass[a]=='0')
		{
			a=rand()%got;
		};
		icou=0;
		while(icou<=got)
		{
			if(pass[a]==sym[icou])
			{
				goto again;
			};
			icou++;
		};
		b=rand()%nsym;
		pass[a]=sym[b];
		counter++;
	};
   // printf("\nThe pass is: %s",pass);
   // deb=sizeof(pass);
   // printf("\n%d",deb);
   // printf("\nThe pass is: %s",pass);
    return pass;
}
