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
#ifndef _QRadSYSTEM_H_
#define _QRadSYSTEM_H_

#include <pthread.h>
#include <semaphore.h>
#include <dlfcn.h>


#define sem_t_qrad sem_t
#define pthread_mutex_t_qrad pthread_mutex_t
#define pthread_t_qrad pthread_t

#define QRad_WAIT_INFINITE 0
#define WAIT_OBJECT_0     0
#define DLL_PROCESS_DETACH      0x00000001
#define DLL_PROCESS_ATTACH      0x00000002
#define WINAPI
#define ULONG unsigned long
#define QRad_IOMANAGER_NAME "libqradiomanager.so"
#define QRad_CONFIG_FILE    "/etc/qradio.conf"
#define QRad_DEFAULT_LOWAPI "libqradiobema.so"

#define QRad_LoadLibrary( szLibrary ) dlopen( szLibrary, RTLD_LAZY )
#define QRad_FreeLibrary( hLibrary )  dlclose( hLibrary )
#define QRad_GetProcAddress( hLibrary, szName ) dlsym( hLibrary, szName )
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

#define QRad_ReleaseMutex(mutex) pthread_mutex_unlock(mutex)
#define QRad_SetEvent(event) sem_post(event)
#define QRad_CreateEvent(sem, p1, p2) sem_init(sem, p1,  p2)
#define QRad_CloseMutex(mutex) pthread_mutex_destroy(mutex)
#define QRad_CloseEvent(event) sem_destroy(event)
#define QRad_ExitThread( arg ) 

#ifdef __cplusplus
extern "C"
{
#endif

int QRad_CreateMutex(pthread_mutex_t *hMutex);

int QRad_WaitForSingleEvent(sem_t *pEvent, int iTimeout);
int QRad_WaitForSingleMutex(pthread_mutex_t *pMutex, int iTimeout);
void QRad_WaitForSingleThread (pthread_t pThread);

int QRad_CreateThread(pthread_t *hThread, void * (*pFunction)(void *), void *pParameter);

void Sleep (int dwMiliseconds);

//void WriteTrace (unsigned char *pArguments, ...);
void WriteTrace( char *pszFileName, int iLine, int iIndex,  char *pArguments, ...);

int HexWriteTrace ( char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num);

unsigned int GetTickCount( void );

int QRad_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size );
int QRad_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num );
int QRad_GetCurrentDate( int *nDay, int *nMonth, int *nYear );



int GetPrivateProfileString( char * lpAppName, char * lpKeyName, char * lpDefault, 
                             char * lpReturnedString, int nSize, char * lpFileName );

#ifdef __cplusplus
}
#endif

#endif
