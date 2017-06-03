#ifndef __qradDEBUG__H__
#define __qradDEBUG__H__

#include <QCoreApplication>
#include <QTime>
#include <QStringList>
#include <stdio.h>

#ifdef Q_OS_WIN32
#define debug_message(fmt, args...) \
    if (QCoreApplication::instance()->arguments().contains("--debug")) \
    {   \
      FILE *fp;\
      fp = fopen( "debug.txt", "a+t");\
      if( fp )\
      {\
         fprintf(fp, "%s:%s:L:%d ", __FILE__,__PRETTY_FUNCTION__, __LINE__);  \
         fprintf(fp, fmt, ##args); \
         fflush(fp);\
         fclose(fp);\
      }\
    }

#else
#define debug_message(fmt, args...) \
    if (QCoreApplication::instance()->arguments().contains("--debug")) \
{ fprintf(stdout, "%s:%s:L:%d ", __FILE__,__PRETTY_FUNCTION__, __LINE__);  fprintf(stdout, fmt, ##args); fflush(stdout); }
#endif

#define debug_messagetime(fmt, args...) \
    if (QCoreApplication::instance()->arguments().contains("--debugtime")) \
    { fprintf(stdout, fmt, ##args); fflush(stdout); }

#define _tracein_ \
    QTime g_timer_inout; \
    g_timer_inout.start(); \
    debug_messagetime("--> [%s] %s - L%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define _tracerestart_\
    debug_messagetime("[%s] L%d - t:%d\n",  __FILE__, __LINE__, g_timer_inout.elapsed()) \
    g_timer_inout.restart();


#define _traceout_\
    debug_messagetime("<-- [%s] %s:L%d - t:%d\n",  __FILE__, __PRETTY_FUNCTION__, __LINE__, g_timer_inout.elapsed())


#define _tracein2_ \
    QTime g_timer_inout; \
    g_timer_inout.start();


#define _traceout2_\
    {\
    int ielapsed = g_timer_inout.elapsed(); \
    if( ielapsed )  debug_messagetime("<-- [%s] %s:L%d - t:%d\n",  __FILE__, __PRETTY_FUNCTION__, __LINE__, g_timer_inout.elapsed()) \
    }

#endif
