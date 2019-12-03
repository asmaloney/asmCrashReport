#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include <assert.h>

#ifdef ASM_CRASH_REPORT
#include "asmCrashReport.h"
#endif

class crashTest
{
   public:
      void  crashMe() { function1(); }

      void  accessViolation( int val )
      {
         qDebug() << Q_FUNC_INFO;
         int * foo = NULL;
         *foo = val;
      }

      void stackoverflow()
      {
         qDebug() << Q_FUNC_INFO;
         int foo[10000];
         (void)foo;
         stackoverflow();
      }

      void throwError()
      {
         qDebug() << Q_FUNC_INFO;
         throw "error";
      }

      void outOfBounds()
      {
         qDebug() << Q_FUNC_INFO;
         std::vector<int> v;
         v[0] = 5;
      }

      void abort()
      {
         qDebug() << Q_FUNC_INFO;
         ::abort();
      }

   private:
      void  divideByZero( int val )
      {
         qDebug() << Q_FUNC_INFO << val;
         int   foo = val / 0;
      }

      void  function2( int val )
      {
         ++val;

         divideByZero( val );
      }

      void  function1()
      {
         function2( 41 );
      }
};


void myterminate () {
   qCritical() << "terminate handler called";
   abort();  // forces abnormal termination
}


int main( int argc, char** argv )
{
   std::set_terminate (myterminate);

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

   int methode = 0;
   if (argc > 1)
   {
      methode = QString(argv[1]).toInt();
   }
   crashTest ct;
   switch (methode) {
   case 0:
      ct.crashMe();
      break;
   case 1:
      ct.accessViolation(17);
      break;
   case 2:
      ct.stackoverflow();
      break;
   case 3:
      ct.throwError();
      break;
   case 4:
      ct.outOfBounds();
      break;
   case 5:
      ct.abort();
      break;
   default:
      qDebug() << "Invalid methode";
      return 1;
   }

   return app.exec();
}
