#include<time.h>
#include<stdio.h>
#include<malloc.h>

bool timchk(struct tm * given,int reqday,int reqmon,int reqyear)
{
    int diffday,diffmon,diffyear;
	time_t localtim;
//	struct tm * given = (struct tm *)malloc(sizeof(struct tm));
	struct tm * local = (struct tm *)malloc(sizeof(struct tm));
	time(&localtim);
	local = localtime(&localtim);
/*	printf("Enter the date: ");
	scanf("%d",&given->tm_mday);
	printf("Enter the month: ");
	scanf("%d",&given->tm_mon);
	printf("Enter the year: ");
	scanf("%d",&given->tm_year);
	char * mon[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	printf("%d-%s-%d",local->tm_mday,mon[local->tm_mon],local->tm_year+1900);*/
	if(local->tm_mday >= given->tm_mday)
	diffday = local->tm_mday - given->tm_mday;
	else
	{
	    if(local->tm_mon==1||local->tm_mon==3||local->tm_mon==5||local->tm_mon==7||local->tm_mon==8||local->tm_mon==10)
            local->tm_mday=+31;
        else if(local->tm_mon==4||local->tm_mon==6||local->tm_mon==9||local->tm_mon==11)
            local->tm_mday=+30;
        else if(local->tm_mon==2)
            local->tm_mday=+28;
        else if(local->tm_mon==0)
        {
            local->tm_mday=+31;
            local->tm_year--;
            local->tm_mon=12;
        }
        local->tm_mon--;
        diffday = local->tm_mday - given->tm_mday;
	}
	if(local->tm_mon >= given->tm_mon)
	diffmon = local->tm_mon - given->tm_mon;
	else
	{
	    local->tm_mon=+12;
	    local->tm_year--;
	};
	diffyear = local->tm_year - given->tm_year;
	if(diffyear>reqyear)
		return 0;
	else if(diffyear==reqyear)
	{
		if(diffmon>reqmon)
		return 0;
		else if(diffmon==reqmon)
		{
			if(diffday>reqday)
			return 0;
		}
	}
	return 1;
//	printf("\n%d-%d-%d",diffday,diffmon,diffyear+1900);
}
