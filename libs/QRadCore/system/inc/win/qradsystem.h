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

#include <windows.h>
#include <pthread.h>

#define pthread_t_qrad HANDLE
#define sem_t_qrad HANDLE
#define pthread_mutex_t_qrad HANDLE


/*typedef HANDLE   pthread_t;
typedef HANDLE   sem_t;
typedef HANDLE   pthread_mutex_t ;
*/

#define QRad_WAIT_INFINITE 0
#define QRad_IOMANAGER_NAME "qradiomanager.dll"
#define QRad_CONFIG_FILE    "qradio.ini"
#define QRad_DEFAULT_LOWAPI "qradiobema.dll"


#define QRad_LoadLibrary( szLibrary ) LoadLibraryA( szLibrary )
#define QRad_FreeLibrary( hLibrary )  FreeLibrary( hLibrary )
#define QRad_GetProcAddress( hLibrary, szName ) GetProcAddress( hLibrary, szName )

///// end .........................................................

#define QRad_ReleaseMutex( mutex ) ReleaseMutex( *mutex )
#define QRad_SetEvent( event ) SetEvent( *event )
//#define QRad_CreateEvent(sem, p1, p2) sem_init(sem, p1,  p2)
#define QRad_CloseMutex( mutex ) CloseHandle( *mutex )
#define QRad_CloseEvent(event) CloseHandle( *event )
#define QRad_ExitThread( value ) ExitThread( value )

#ifdef __cplusplus
extern "C"
{
#endif

int QRad_CreateMutex( pthread_mutex_t_qrad *hMutex );
int QRad_CreateEvent( sem_t_qrad *pSem, int p1, int p2 ) ;
int QRad_WaitForSingleEvent( sem_t_qrad *pEvent, int iTimeout );
int QRad_WaitForSingleMutex( pthread_mutex_t_qrad *pMutex, int iTimeout );
void QRad_WaitForSingleThread( pthread_t_qrad pThread );
int QRad_CreateThread(pthread_t_qrad *hThread, void * (*pFunction)(void *), void *pParameter);

//void WriteTrace (unsigned char *pArguments, ...);
void WriteTrace (unsigned char *pszFileName, int iLine, int iIndex, unsigned char *pArguments, ...);

int HexWriteTrace (unsigned char *pszFileName, int iLine, int iIndex,   unsigned char *bin, int num);
int QRad_GetCurrentDate( int *nDay, int *nMonth, int *nYear );


int QRad_hexstr2bin(   unsigned char *bin, int num, const unsigned char *hexstr, int size );
int QRad_bin2hexstr(char *hexstr, int size,  unsigned char *bin, int num );

#ifdef __cplusplus
}
#endif


#endif
