#include "mylibrary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char buffer[1000];
char bufferarr[100];
int testnum;
int apm;
char schedule[7][200];
char mdate[3], mtime[5], nbin[1], med[15], npill[1];
uint16_t timer_count = 0, buffer_index = 0;

// rearrange the strings in case the strings received is disordered
void Arrange_strings(char arr[])
{
	char part[200];
	int start;
	char *pos = strstr(arr, "mon");
	if(pos != NULL)
	{
		start = pos - arr;
	strncpy(bufferarr,arr+start,strlen(arr)-start+1);
	strncpy(part,arr,start);
	strcat(bufferarr,part);
	}
}

//  mon{\"mBinNumb\":\"1\",\"mMedName\":\"test\",\"mNumbPills\":\"2\",\"mTime\":\"6:48 PM\"}
//  mon{"mBinNumb":"1","mMedName":"Tylenol","mNumbPills":"1",'mTime":"3:00 PM"}
// grab data if the string format is as above
void Process_string(char arr[])
{
	  strncpy (mdate, arr, 3);
	  strncpy (nbin, arr+16, 1);

	  uint8_t j=0, i=31;
	  while(arr[i] != ',')
	  {
		  i++;
		  j++;
	  }

	  strncpy (med, arr+31, j-1);
	  strncpy (npill, arr+46+j, 1);
	  if(arr[62+j]==':')
	  {
		  strncpy (mtime, arr+58+j, 4);
		  if(arr[66+j]=='A')
		  {
			  apm = 0;
		  }else
		  {
			  apm = 1;
		  }
	  }else
	  {
		  strncpy (mtime, arr+58+j, 5);
		  if(arr[67+j]=='A')
		  {
			  apm = 0;
		  }else
		  {
			  apm = 1;
		  }
	  }

}

// arr[] is the overall string possibly including several med, target[] is a 2d array
// which stores each med
void Individual_med(char arr[], char target[])
{
	char day[3], med[100];
	strncpy(day, arr, 3);
	int start, end, count=-1;
	for(int i=0;i<=strlen(arr);i++)
	{
		if(arr[i]=='{')
		{
			start = i;
			count++;
		}
		if(arr[i]=='}')
		{
			end = i;
			strncpy(med, arr+start, end-start+1);
			strncpy(target,day,3);
			strcat(target,med);
			//strncpy(target[count],day,3);
			//strcat(target[count],med);
		}
	}
}

// stores every weekday schedule into 2d array schdule[]
void Manage_strings()
{
	//char weekday[7][3] = { "mon", "tue", "wed", "thu", "fri", "sat", "sun"};
	int pre = 0;
	//char *pos;	//char *pointer = buffer;
	for(int i=0; i<=strlen(buffer); i++)
	{
		//pos = strstr(pointer, weekday[i]);
    	if (buffer[i]=='t' && buffer[i+1]=='u' && buffer[i+2]=='e')
    	{
    		strncpy(schedule[0],buffer,i);
    		pre = i;
    	}
    	if (buffer[i]=='w' && buffer[i+1]=='e' && buffer[i+2]=='d')
    	{
    		strncpy(schedule[1],buffer+pre,i-pre);
    		pre = i;
    	}
    	if (buffer[i]=='t' && buffer[i+1]=='h' && buffer[i+2]=='u')
    	{
    		strncpy(schedule[2],buffer+pre,i-pre);
    		pre = i;
    	}
    	if (buffer[i]=='f' && buffer[i+1]=='r' && buffer[i+2]=='i')
    	{
    		strncpy(schedule[3],buffer+pre,i-pre);
    		pre = i;
    	}
    	if (buffer[i]=='s' && buffer[i+1]=='a' && buffer[i+2]=='t')
    	{
    		strncpy(schedule[4],buffer+pre,i-pre);
    		pre = i;
    	}
    	if (buffer[i]=='s' && buffer[i+1]=='u' && buffer[i+2]=='n')
    	{
    		strncpy(schedule[5],buffer+pre,i-pre);
    		pre = i;
    	}
	}
	strncpy(schedule[6],buffer+pre,strlen(buffer));

}

// check whether 2 strings are the same
uint8_t string_compare(char array1[], char array2[], uint16_t length)
{
	 uint8_t comVAR=0, i;
	 for(i=0;i<length;i++)
	   	{
	   		  if(array1[i]==array2[i])
	   	  		  comVAR++;
	   	  	  else comVAR=0;
	   	}
	 if (comVAR==length)
		 	return 1;
	 else 	return 0;
}

// for testing
void Message_handler()
{
	if(string_compare(buffer, "Display", strlen("Display")))
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Test 2 success.\n", strlen("Test 2 success.\n"), 500);
	}else
	if(string_compare(buffer, "Test 2", strlen("Test 2")))
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Test 2 success.\n", strlen("Test 2 success.\n"), 500);
	}else
	if(string_compare(buffer, "Test", strlen("Test")))
	{
		testnum = 100;
	}else
	{
		strcat(buffer, "\n");
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 500);
	}

	memset(buffer, 0, sizeof(buffer));
	buffer_index = 0;
	timer_count = 0;
}

