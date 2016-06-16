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

#include <windows.h>
#include <pthread.h>

#define pthread_t_dsm HANDLE
#define sem_t_dsm HANDLE
#define pthread_mutex_t_dsm HANDLE


/*typedef HANDLE   pthread_t;
typedef HANDLE   sem_t;
typedef HANDLE   pthread_mutex_t ;
*/

#define DSM_WAIT_INFINITE 0
#define DSM_IOMANAGER_NAME "dsmiomanager.dll"
#define DSM_CONFIG_FILE    "dsmio.ini"
#define DSM_DEFAULT_LOWAPI "dsmiobema.dll"


#define DSM_LoadLibrary( szLibrary ) LoadLibraryA( szLibrary )
#define DSM_FreeLibrary( hLibrary )  FreeLibrary( hLibrary )
#define DSM_GetProcAddress( hLibrary, szName ) GetProcAddress( hLibrary, szName )

///// end .........................................................

#define DSM_ReleaseMutex( mutex ) ReleaseMutex( *mutex )
#define DSM_SetEvent( event ) SetEvent( *event )
//#define DSM_CreateEvent(sem, p1, p2) sem_init(sem, p1,  p2)
#define DSM_CloseMutex( mutex ) CloseHandle( *mutex )
#define DSM_CloseEvent(event) CloseHandle( *event )
#define DSM_ExitThread( value ) ExitThread( value )

#ifdef __cplusplus
extern "C"
{
#endif

int DSM_CreateMutex( pthread_mutex_t_dsm *hMutex );
int DSM_CreateEvent( sem_t_dsm *pSem, int p1, int p2 ) ;
int DSM_WaitForSingleEvent( sem_t_dsm *pEvent, int iTimeout );
int DSM_WaitForSingleMutex( pthread_mutex_t_dsm *pMutex, int iTimeout );
void DSM_WaitForSingleThread( pthread_t_dsm pThread );
int DSM_CreateThread(pthread_t_dsm *hThread, void * (*pFunction)(void *), void *pParameter);

//void WriteTrace (unsigned char *pArguments, ...);
void WriteTrace (unsigned char *pszFileName, int iLine, int iIndex, unsigned char *pArguments, ...);

int HexWriteTrace (unsigned char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num);
int DSM_GetCurrentDate( int *nDay, int *nMonth, int *nYear );


int DSM_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size );
int DSM_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num );

#ifdef __cplusplus
}
#endif


#endif
