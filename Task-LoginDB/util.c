/*
 * util.c
 *
 * Created: 26/10/2023 12:57:54 PM
 *  Author: Kirollos
 */ 

#include "util.h"

// Convert a string to an integer
s32 my_atoi(const u8* str)
{
	s32 ret = 0;
	const u8* ptr = str;
	while(ptr != '\0')
	{
		if(*ptr > '9' || *ptr < '0') break;
		ret = (ret*10) + (*ptr - '0');
		ptr++;
	}
	return ret;
}