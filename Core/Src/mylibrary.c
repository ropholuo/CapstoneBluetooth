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
uint8_t timer_count = 0, buffer_index = 0;

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
void Indivial_med(char arr[], char target[])
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
			strncpy(med, start, end-start+1);
			strncpy(target[count],day,3);
			strcat(target[count],med);
		}
	}
}

// stores every weekday schedule into 2d array schdule[]
void Manage_strings(char arr[])
{
	char weekday[7][3] = { "mon", "tue", "wed", "thu", "fri", "sat", "sun"};
	int post = 0, pre;
	for(int i=0; i<=6; i++)
	{
		char *pos = strstr(arr, weekday[i]);
    	if (pos != NULL)
    	{
    		pre = pos - arr;
    		post = pre - post;
    	}
    	strncpy(schedule[i],arr,post);
    	post = pre;
	}
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

