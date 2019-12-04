#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include <cassert>

#ifdef ASM_CRASH_REPORT
#include "asmCrashReport.h"
#endif

class crashTest
{
   public:
      void  divideByZero() { _function1(); }

      void  accessViolation( int val ) { _accessViolation( val ); }

      void stackoverflow() { _stackoverflow(); }

      void throwError() { _throwError(); }

      void outOfBounds() { _outOfBounds(); }

      void abort() { _abort(); }

   private:
      // The purpose of all the private methods is just to provide a slightly longer call stack

      void  _divideByZero( int val )
      {
         qDebug() << Q_FUNC_INFO << val;
         int   foo = val / 0;
      }

      void  _function2( int val )
      {
         ++val;

         _divideByZero( val );
      }

      void  _function1()
      {
         _function2( 41 );
      }

      void  _accessViolation( int val )
      {
         qDebug() << Q_FUNC_INFO;
         int * foo = nullptr;
         *foo = val;
      }

      void _stackoverflow()
      {
         qDebug() << Q_FUNC_INFO;
         int foo[10000];
         (void)foo;
         stackoverflow();
      }

      void _throwError()
      {
         qDebug() << Q_FUNC_INFO;
         throw "error";
      }

      void _outOfBounds()
      {
         qDebug() << Q_FUNC_INFO;
         std::vector<int> v;
         v[0] = 5;
      }

      void _abort()
      {
         qDebug() << Q_FUNC_INFO;
         ::abort();
      }
};


static void sMyTerminate()
{
   qCritical() << "Terminate handler called";
   abort();  // forces abnormal termination
}


int main( int argc, char** argv )
{
   std::set_terminate( sMyTerminate );

   QApplication  app( argc, argv );

   app.setApplicationName( QStringLiteral( "asmCrashReportTest" ) );
   app.setApplicationVersion( QStringLiteral( "1.0.0" ) );

#ifdef ASM_CRASH_REPORT
   asmCrashReport::setSignalHandler( QString(), [] (const QString &inFileName, bool inSuccess) {
      QString  message;

      if ( inSuccess )
      {
         message = QStringLiteral( "Sorry, %1 has crashed. A log file was written to:\n\n%2\n\nPlease email this to support@example.com." ).arg( QCoreApplication::applicationName(), inFileName );
      }
      else
      {
         message = QStringLiteral( "Sorry, %1 has crashed and we could not write a log file to:\n\n%2\n\nPlease contact support@example.com." ).arg( QCoreApplication::applicationName(), inFileName );
      }

      QMessageBox::critical( nullptr, QObject::tr( "%1 Crashed" ).arg( QCoreApplication::applicationName() ), message );
   });
#endif

   int crashType = 0;

   if ( argc > 1 )
   {
      crashType = QString( argv[1] ).toInt();
   }

   crashTest crashTest;

   switch ( crashType )
   {
      case 0:
         crashTest.divideByZero();
         break;

      case 1:
         crashTest.accessViolation( 17 );
         break;

      case 2:
         crashTest.stackoverflow();
         break;

      case 3:
         crashTest.throwError();
         break;

      case 4:
         crashTest.outOfBounds();
         break;

      case 5:
         crashTest.abort();
         break;

      default:
         qDebug() << "Invalid crash type. Expecting 0-5.";
         return 1;
   }

   return app.exec();
}
