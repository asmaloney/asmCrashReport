#ifndef ASMCRASHREPORT_H
#define ASMCRASHREPORT_H

#include <QString>


namespace asmCrashReport {

   /// Function signature for a callback after the log is written.
   typedef void (*logWrittenCallback)( const QString &inLogFileName );

   ///! Set a signal handler to capture stack trace to a log file.
   ///
   /// @arg inCrashReportDirPath Path to directory to write our crash report to.
   ///      If not set, it will use Desktop/<App Name> Crash Logs/
   /// @arg inLogWrittenCallback A function to call after we've written the log file.
   ///      You might use this to display a message to the user about where to find the log for example.
   void  setSignalHandler( const QString &inCrashReportDirPath = QString(), logWrittenCallback inLogWrittenCallback = nullptr );

}

#endif
