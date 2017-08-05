#include <QApplication>
#include <QMessageBox>

#include <assert.h>

#ifdef ASM_CRASH_REPORT
#include "asmCrashReport.h"
#endif

class crashTest
{
   public:
      void  crashMe() { function1(); }

   private:
      void  divideByZero( int val )
      {
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


int main( int argc, char** argv )
{
   QApplication  app( argc, argv );

   app.setApplicationName( QStringLiteral( "asmCrashReportExample" ) );
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

   crashTest().crashMe();

   return app.exec();
}
