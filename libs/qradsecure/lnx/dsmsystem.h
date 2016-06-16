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
#ifndef _DSMSYSTEM_H_
#define _DSMSYSTEM_H_

#include <pthread.h>
#include <semaphore.h>
#include <dlfcn.h>


#define sem_t_dsm sem_t
#define pthread_mutex_t_dsm pthread_mutex_t
#define pthread_t_dsm pthread_t

#define DSM_WAIT_INFINITE 0
#define WAIT_OBJECT_0     0
#define DLL_PROCESS_DETACH      0x00000001
#define DLL_PROCESS_ATTACH      0x00000002
#define WINAPI
#define ULONG unsigned long
#define DSM_IOMANAGER_NAME "libdsmiomanager.so"
#define DSM_CONFIG_FILE    "/etc/dsmio.conf"
#define DSM_DEFAULT_LOWAPI "libdsmiobema.so"

#define DSM_LoadLibrary( szLibrary ) dlopen( szLibrary, RTLD_LAZY )
#define DSM_FreeLibrary( hLibrary )  dlclose( hLibrary )
#define DSM_GetProcAddress( hLibrary, szName ) dlsym( hLibrary, szName )
#define min( a, b ) a > b ? b : a

typedef void *   HINSTANCE;

#ifdef _DLL_MAIN_
int WINAPI DllMain( HINSTANCE hDLL, int dwReason, void * lpReserved );

static void __attribute__ ( ( constructor ) ) LibAttachDllMain( void )
{
  DllMain( 0, DLL_PROCESS_ATTACH, NULL );
}

static void __attribute__ ( ( destructor ) ) LibDetachDllMain( void )
{
  DllMain( 0, DLL_PROCESS_DETACH, NULL );
}

#endif
///// end .........................................................

#define DSM_ReleaseMutex(mutex) pthread_mutex_unlock(mutex)
#define DSM_SetEvent(event) sem_post(event)
#define DSM_CreateEvent(sem, p1, p2) sem_init(sem, p1,  p2)
#define DSM_CloseMutex(mutex) pthread_mutex_destroy(mutex)
#define DSM_CloseEvent(event) sem_destroy(event)
#define DSM_ExitThread( arg ) 

#ifdef __cplusplus
extern "C"
{
#endif

int DSM_CreateMutex(pthread_mutex_t *hMutex);

int DSM_WaitForSingleEvent(sem_t *pEvent, int iTimeout);
int DSM_WaitForSingleMutex(pthread_mutex_t *pMutex, int iTimeout);
void DSM_WaitForSingleThread (pthread_t pThread);

int DSM_CreateThread(pthread_t *hThread, void * (*pFunction)(void *), void *pParameter);

void Sleep (int dwMiliseconds);

//void WriteTrace (unsigned char *pArguments, ...);
void WriteTrace( char *pszFileName, int iLine, int iIndex,  char *pArguments, ...);

int HexWriteTrace ( char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num);

unsigned int GetTickCount( void );

int DSM_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size );
int DSM_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num );
int DSM_GetCurrentDate( int *nDay, int *nMonth, int *nYear );



int GetPrivateProfileString( char * lpAppName, char * lpKeyName, char * lpDefault, 
                             char * lpReturnedString, int nSize, char * lpFileName );

#ifdef __cplusplus
}
#endif

#endif
