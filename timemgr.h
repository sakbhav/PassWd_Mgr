/*	Copyright 2013 Saket Srivastava
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


#include<time.h>
#include<stdio.h>
#include<malloc.h>

bool timchk(struct tm * given,int reqday,int reqmon,int reqyear)
{
    int diffday,diffmon,diffyear;
	time_t localtim;
	struct tm * local = (struct tm *)malloc(sizeof(struct tm));
	time(&localtim);
	local = localtime(&localtim);
	if(local->tm_mday >= given->tm_mday)
	diffday = local->tm_mday - given->tm_mday;
	else
	{
	    if(local->tm_mon==1||local->tm_mon==3||local->tm_mon==5||local->tm_mon==7||local->tm_mon==8||local->tm_mon==10)
            local->tm_mday+=31;
        else if(local->tm_mon==4||local->tm_mon==6||local->tm_mon==9||local->tm_mon==11)
            local->tm_mday+=30;
        else if(local->tm_mon==2)
            local->tm_mday+=28;
        else if(local->tm_mon==0)
        {
            local->tm_mday+=31;
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
	    local->tm_mon+=12;
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
}
