//------------------------------------------------------------------
//
//                            CONFIDENTIAL
//                     COPYRIGHT (c) QRad 2009
//                         ALL RIGHTS RESERVED
//                         AN UNPUBLISHED WORK
//
//                         PROPERTY OF QRad
//                         MANAUS,  AMAZONAS
//
// This software contains proprietary, trade secret information
// and is the property of QRad.  This software and the
// information contained therein may not be disclosed, used or
// copied in whole or in part without the express, prior, written
// consent of QRad.
//
//
// Module Name:   qradrequest.c
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
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "qradsystem.h"
//#include "qradtrace.h"


int QRad_WaitForSingleEvent(sem_t *pEvent, int iTimeout)
{
  struct timespec stdelay;
  int iResult = -1;
  int nTimeCounter = 0;
  int nMagicNumber = 21;
  
  if (QRad_WAIT_INFINITE == iTimeout )
  {
    iResult = sem_wait(pEvent);
  }
  else 
  { 
    while (nTimeCounter < (iTimeout + nMagicNumber)) 
    {
      stdelay.tv_sec = 0;
      stdelay.tv_nsec = 1000000;  // 1 milli sec delay //1000000
    
      iResult = sem_trywait(pEvent);
      if (0  == iResult)  
      {
      // Acquire semaphore success
      // Access the shared resource
        break;
      }
      else 
      {
        nanosleep(&stdelay, NULL);
  //      nTimeCounter++ ;
        nTimeCounter+=10 ;
      }
    }
  }
  
  return iResult;
}

/*
 * Returns the processor's frequency in Khz from the proc system
 */
/*unsigned long CPUFreq()
{
   FILE * fpCpuInfo;
   unsigned char  szMhz[255];
   unsigned char *pcToken = NULL;

   
   if ( ( fpCpuInfo = fopen( "/proc/cpuinfo", "r" ) ) == NULL )
   {
        //printf("error opening /proc/cpuinfo");
   }
   rewind( fpCpuInfo );

   // scans "CPU MHz"
   while ( !feof( fpCpuInfo ) )
   {
      //read line
      fgets( szMhz, 79, fpCpuInfo );

      //scan "CPU MHz"
      if ( strncasecmp( szMhz, "cpu MHz", 7 ) == 0 )
      {
         pcToken = strtok( szMhz, ":" );
         pcToken = strtok( NULL, " \r\n" );
         break;
      }
   } 
   
   fclose( fpCpuInfo );
   return ( atof( pcToken ) * 1000 ); // Result is in KHz
}*/

unsigned int GetTickCount( void )
{
   /*
    static unsigned long lCpuSpeed = 0;
    long long ulTime;
    asm volatile ( "rdtsc":"=A" ( ulTime ) );

    if ( 0 == lCpuSpeed )
    {
       lCpuSpeed = CPUFreq();  // CPU frequency in Khz
    }

    return ( unsigned int) ( ulTime / lCpuSpeed ); // Result is in milisseconds    
    */
   struct timeb currentTime;

   ftime( &currentTime );

   return ( currentTime.time * 1000 ) + currentTime.millitm;   // milliseconds since 1970
}


int QRad_WaitForSingleMutex(pthread_mutex_t *pMutex, int iTimeout)
{
  struct timespec stdelay;
  int iResult = -1;
  int nTimeCounter = 0;
  int nMagicNumber = 21;
  
  if (QRad_WAIT_INFINITE == iTimeout )
  {
    iResult = pthread_mutex_lock(pMutex);
  }
  else 
  {
    while (nTimeCounter < (iTimeout + nMagicNumber))
    {
   
      stdelay.tv_sec = 0;
      stdelay.tv_nsec = 1000000;  // 1 milli sec delay
   
      iResult = pthread_mutex_trylock(pMutex);
      if (0  == iResult)  
      {
     // Acquire semaphore success
     // Access the shared resource
        break;
      }
      else 
      {
        nanosleep(&stdelay, NULL);
        nTimeCounter+=10 ;
      }
    }
  }
  
  return iResult;
}

void QRad_WaitForSingleThread (pthread_t pThread)
{
	pthread_join(pThread, NULL);
}


///
/// CreateMutex
///

int QRad_CreateMutex(pthread_mutex_t *hMutex )
{
  pthread_mutexattr_t  attr;

  pthread_mutexattr_init (&attr);
  if ( pthread_mutex_init(hMutex, &attr) )
  {
    return -1;
  }  
  return 0;
}


///
/// Equivalent Name in Windows 
///

int  QRad_CreateThread(pthread_t * hThread, void * (*pFunction)(void *), void *pParameter)
{
  
  pthread_attr_t threadAttr;
  int iReturn;

  // initialize the thread attribute
  pthread_attr_init(&threadAttr);  
  
  // Set the stack size of the thread
  pthread_attr_setstacksize(&threadAttr, 120*1024);

  //Set thread to detached state. No need for pthread_join
  //pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);  
  
  // Create the threads
  iReturn = pthread_create(hThread, &threadAttr, pFunction, pParameter);            

  // Destroy the thread attributes
  pthread_attr_destroy(&threadAttr);    
  
  return iReturn;
}


int GetMiliSeconds()
{
 struct timeval tv;
 gettimeofday( &tv, NULL );
 return (int) tv.tv_usec? tv.tv_usec/1000:0;
}

void Sleep ( int dwMiliseconds )
{
  usleep( dwMiliseconds * 1000 );
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


void WriteTrace ( char *pszFileName, int iLine, int iIndex,  char *pArguments, ...)
{
  struct tm *newtime;
  time_t long_time;
  char szFileName[150];
  char szDateTime[50];
  va_list tagArgList;
  
  sprintf (szFileName, "%s%02x.TXT", pszFileName, iIndex);

  EnsureMaxFileSize( szFileName );
  
  FILE *fp = fopen(szFileName, "a+t");
  if (fp)
  {
    time( &long_time );             
    newtime = localtime( &long_time );
    sprintf (szDateTime, "%02d:%02d.%02d'%03d [",newtime->tm_hour,newtime->tm_min, newtime->tm_sec,GetMiliSeconds());
    fwrite (szDateTime, 1, strlen(szDateTime), fp);
    va_start(tagArgList, pArguments);
    vfprintf(fp, pArguments, tagArgList);
    sprintf (szDateTime, "] Line = %0d\n",iLine);
    fwrite (szDateTime, 1, strlen(szDateTime), fp);
    fflush (fp);
    va_end(tagArgList);
    fclose (fp);
  }
}

int HexWriteTrace ( char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num)
{
	int i;
	char byte[3];
	unsigned char hexstr[1000];
	unsigned char *p=hexstr;
	struct tm *newtime;
	time_t long_time;
	char szFileName[150];
	char szDateTime[1150];
	va_list tagArgList;
  
	sprintf (szFileName, "%s%02x.TXT", pszFileName, iIndex);
	
	memset (hexstr,0,sizeof(hexstr));

	if (num > (1000 / 2)) 
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

	FILE *fp = fopen(szFileName, "a+t");
	if (fp)
	{
		time( &long_time );             
		newtime = localtime( &long_time );
		sprintf (szDateTime, "%02d:%02d.%02d'%03d [%s",newtime->tm_hour,newtime->tm_min, newtime->tm_sec,GetMiliSeconds(),hexstr);
		fwrite (szDateTime, 1, strlen(szDateTime), fp);
		sprintf (szDateTime, "] Line = %0d\n",iLine);
		fwrite (szDateTime, 1, strlen(szDateTime), fp);
		fflush (fp);
		va_end(tagArgList);
		fclose (fp);
	}
	
	return i;
}


///
/// @QRad_GetCurrentDate
///
int QRad_GetCurrentDate( int *nDay, int *nMonth, int *nYear )
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
int QRad_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size )
{
   int i, j;
   char byte[3];

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

      if (sscanf(byte, "%02X", (unsigned int *)&bin[j++]) != 1)
      {
         return -1;
      }
   }

   return i;
}

//
// @hexstr2bin
//
int  QRad_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num )
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


/*
 *  Remove all initial blank characters from string,
 *  the blank characters are defined by the isblank function from GNU glib
 */
char *RemoveSpaces( char *szBuffer )
{
    char *szTmpBuffer = szBuffer;

    while ( isblank( *szTmpBuffer ) )
    {
       szTmpBuffer++;
    }

    return ( szTmpBuffer );
}

/*
 * The GetPrivateProfileString function retrieves a string from the specified
 * section in an initialization file.
 */
 
 
int GetPrivateProfileString( char * lpAppName, char * lpKeyName, char * lpDefault, 
                             char * lpReturnedString, int nSize, char * lpFileName )
{
   
   typedef enum
   {
      isSearchingSec, 
      isSearchingKey  
   }QRad_ST_MACHINE;

   int dwCount;     
   int  bEndProc;    
   FILE  *iniFile;    
   QRad_ST_MACHINE estMachine; 
   char  *szDefault = "";     
   char  *szLineText = NULL; 
   char  *szLineTmp;          
   int   iLineSize;           
   int   iBufferSize;        

   bEndProc = 0;

   if ( NULL != lpDefault )
      szDefault = lpDefault;

   
   dwCount = strlen( szDefault );
   if ( dwCount > ( nSize - 1 ) )
      dwCount = nSize - 1;

   strncpy( lpReturnedString, szDefault, dwCount );
   lpReturnedString[ dwCount ] = '\0';
   dwCount = strlen( lpReturnedString );

  
   iniFile = fopen( lpFileName, "r" );
   if ( NULL == iniFile )
        return ( dwCount );

   
   estMachine = isSearchingSec;

   
   szLineText = NULL;
   iBufferSize = 0;
   iLineSize = getline( &szLineText, (size_t *)&iBufferSize, iniFile );
   while ( ( -1 != iLineSize ) && ( 0 == bEndProc ) )
   {
      
      szLineTmp = RemoveSpaces( szLineText );

      switch ( estMachine )
      {
         case isSearchingSec:
           
            if ( '[' == *szLineTmp )
            {
               
               if ( 0 == strncasecmp( ++szLineTmp, lpAppName, strlen( lpAppName ) ) )
               { 
                  estMachine = isSearchingKey; 
               }
            }
            break;

         case isSearchingKey:
            
            if ( '[' == *szLineTmp )
            { 
                bEndProc = 1;
            }
            else if ( 0 == strncasecmp( szLineTmp, lpKeyName,  strlen( lpKeyName ) ) )
            {
                
                szLineTmp = strchr( szLineTmp, '=' );
                if ( NULL != szLineTmp )
                {
                   ++szLineTmp;                                    
                   szLineTmp = RemoveSpaces( szLineTmp ); 
                   dwCount = strlen( szLineTmp );                  

                   
                   while ( ( '\n' == szLineTmp[ dwCount - 1 ] )  ||
                           ( '\r' == szLineTmp[ dwCount - 1 ] ) )
                   {
                        dwCount--;
                   }
                  
                   
                   if ( dwCount > ( nSize - 1 ) )
                      dwCount = nSize - 1;
                   strncpy( lpReturnedString, szLineTmp, dwCount );
                   lpReturnedString[ dwCount ] = '\0'; 
                   
                   bEndProc = 1;
                }
            }
            break;
        }

        if ( 0 == bEndProc )
        {
            
            iLineSize = getline( &szLineText, (size_t *)&iBufferSize, iniFile );
        }
    }

    if( NULL != szLineText )
    {
       free( szLineText );
    }

    fclose( iniFile );

    return ( strlen( lpReturnedString ) );
}

