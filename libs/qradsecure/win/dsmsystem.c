//------------------------------------------------------------------
//
//                            CONFIDENTIAL
//                     COPYRIGHT (c) DSM 2009
//                         ALL RIGHTS RESERVED
//                         AN UNPUBLISHED WORK
//
//                         PROPERTY OF DSM
//                         MANAUS,  AMAZONAS
//
// This software contains proprietary, trade secret information
// and is the property of DSM.  This software and the
// information contained therein may not be disclosed, used or
// copied in whole or in part without the express, prior, written
// consent of DSM.
//
//
// Module Name:   dsmrequest.c
//
//
//
//  $Revision:   0.00  $
//    $Author:   Marco Antonio Bueno da Silva  $
//      $Date:   May 18 2009 09:23:58  $
//
//------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "dsmsystem.h"
//#include "dsmtrace.h"


int DSM_WaitForSingleEvent( sem_t_dsm *pEvent,
                            int iTimeout )
{
  int iResult;
  
  iTimeout= iTimeout==DSM_WAIT_INFINITE?INFINITE:iTimeout;
  
  iResult = WaitForSingleObject( *pEvent, iTimeout );
  
  return iResult;
}

int DSM_WaitForSingleMutex( pthread_mutex_t_dsm *pMutex,
                            int iTimeout)
{
  int iResult = -1;
  
  iTimeout= iTimeout==DSM_WAIT_INFINITE?INFINITE:iTimeout;
  
  iResult = WaitForSingleObject( *pMutex, iTimeout );
  
  
  return iResult;
}

void DSM_WaitForSingleThread( pthread_t_dsm pThread )
{
    WaitForSingleObject( pThread, 500 );
}


///
/// DSM_CreateMutex
///

int DSM_CreateMutex( pthread_mutex_t_dsm *hMutex )
{
  *hMutex = CreateMutex( NULL, 0, NULL);
  if( *hMutex != NULL )
    return 0;
    
  return -100;
}

int DSM_CreateEvent( sem_t_dsm *pSem,
                     int p1, 
                     int p2 ) 
{
  *pSem = CreateEvent( NULL, 0, 0,  NULL);
  if( *pSem != NULL )
    return 0;
    
  return -100;
}                 


///
/// Equivalent Name in Windows 
///

int DSM_CreateThread( pthread_t_dsm * hThread,
                      void * (*pFunction)(void *), 
                      void *pParameter )
{
  DWORD dwThreadID;

  *hThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )pFunction, pParameter, 0, &dwThreadID );
  
  if( *hThread != NULL )
    return 0;
    
  return -100;

}


int GetMiliSeconds()
{
  SYSTEMTIME stSystemTime;
  
  memset( &stSystemTime, 0, sizeof( stSystemTime ) );

  GetSystemTime( &stSystemTime );
  
  return stSystemTime.wMilliseconds; 
}

void EnsureMaxFileSize( char *pFileName )
{
  FILE *fp;
  long iFileSize;
  
  fp = fopen(pFileName, "r+b");
  if( NULL != fp )
  {
    fseek(fp, 0, SEEK_END);
    iFileSize = ( long )ftell( fp );
    fclose( fp );
    if(( 5 * 1024 * 1024 ) < iFileSize )
    {
      remove( pFileName );
    }
  }
  
}


void WriteTrace (unsigned char *pszFileName, int iLine, int iIndex, unsigned char *pArguments, ...)
{
  struct tm *newtime;
  time_t long_time;
  unsigned char szFileName[256];
  unsigned char szDateTime[50];
  va_list tagArgList;
  
  sprintf ((char *)szFileName, (char *)"%s%02x.TXT", pszFileName, iIndex);
  
  EnsureMaxFileSize( (char *)szFileName );
  
  FILE *fp = fopen((char *)szFileName, "a+t");
  if (fp)
  {
    time( &long_time );             
    newtime = localtime( &long_time );
    sprintf ((char *)szDateTime, (char *)"%02d:%02d.%02d'%03d [",newtime->tm_hour,newtime->tm_min, newtime->tm_sec,GetMiliSeconds());
    fwrite (szDateTime, 1, strlen((char *)szDateTime), fp);
    va_start(tagArgList, pArguments);
    vfprintf(fp, (char *)pArguments, tagArgList);
    sprintf ((char *)szDateTime, (char *)"] Line = %0d\n",iLine);
    fwrite (szDateTime, 1, strlen((char *)szDateTime), fp);
    fflush (fp);
    va_end(tagArgList);
    fclose (fp);
  }
}

int HexWriteTrace (unsigned char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num)
{
	int i;
	unsigned char byte[3];
	unsigned char hexstr[1000];
	unsigned char *p=hexstr;
	struct tm *newtime;
	time_t long_time;
	unsigned char szFileName[150];
	unsigned char szDateTime[1150];
	va_list tagArgList;
  
        sprintf ((char *)szFileName, (char *)"%s%02x.TXT", pszFileName, iIndex);
	
	memset (hexstr,0,sizeof(hexstr));

	if (num > (1000 / 2)) 
	{
		return -1;
	}

	for (i=0; i<num; i++) 
	{
                if (snprintf((char *)byte, 3, "%02X", bin[i]) != 2)
		{
			return -1;
		}
		memcpy(p, byte, 2);
		p+=2;
	}

        FILE *fp = fopen((char *)szFileName, "a+t");
	if (fp)
	{
		time( &long_time );             
		newtime = localtime( &long_time );
                sprintf ((char *)szDateTime, (char *)"%02d:%02d.%02d'%03d [%s",newtime->tm_hour,newtime->tm_min, newtime->tm_sec,GetMiliSeconds(),hexstr);
                fwrite ((char *)szDateTime, 1, strlen((char *)szDateTime), fp);
                sprintf ((char *)szDateTime, (char *)"] Line = %0d\n",iLine);
                fwrite ((char *)szDateTime, 1, strlen((char *)szDateTime), fp);
		fflush (fp);
		va_end(tagArgList);
		fclose (fp);
	}
	
	return i;
}


///
/// @DSM_GetCurrentDate
///
int DSM_GetCurrentDate( int *nDay, int *nMonth, int *nYear )
{
  struct tm *newtime;
  time_t long_time;

  time( &long_time );
  newtime = localtime( &long_time );
  
  *nDay    = newtime->tm_mday;
  *nMonth  = newtime->tm_mon;
  *nYear   = newtime->tm_year;

  return 0;
}

//
// @hexstr2bin
//
int DSM_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size )
{
   int i, j;
   unsigned char byte[3];

   if ((size % 2) != 0)
   {
      return -1;
   }

   if ((size / 2) > num)
   {
      return -1;
   }

   j=0; byte[2]=0;
   for (i=0; i<size; i+=2)
   {
      byte[0] = hexstr[i];
      byte[1] = hexstr[i+1];

      if (sscanf((char *)byte, "%02X", (unsigned int *)&bin[j++]) != 1)
      {
         return -1;
      }
   }

   return i;
}

//
// @hexstr2bin
//
int  DSM_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num )
{
	int i;
	char byte[3];
	char *p=hexstr;

  if (num > (size / 2)) 
  {
		return -1;
	}

  for (i=0; i<num; i++) 
  {
    if (snprintf(byte, 3, "%02X", bin[i]) != 2) 
    {
			return -1;
		}
		memcpy(p, byte, 2);
		p+=2;
	}

   return i;
}

