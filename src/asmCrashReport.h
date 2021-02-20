#pragma once

#include <QString>


namespace asmCrashReport {

   /// Function signature for a callback after the log is written.
   /// @param inLogFileName The full path to the log file which was written.
   /// @param inSuccess Whether the file was successfully written.
   using logWrittenCallback = void (*)(const QString &, bool);

   /// Set a signal handler to capture stack trace to a log file.
   ///
   /// @param inCrashReportDirPath Path to directory to write our crash report to.
   ///   If not set, it will use Desktop/<App Name> Crash Logs/
   /// @param inLogWrittenCallback A function to call after we've written the log file.
   ///   You might use this to display a message to the user about where to find the log for example.
   void  setSignalHandler( const QString &inCrashReportDirPath = QString(), logWrittenCallback inLogWrittenCallback = nullptr );

}
