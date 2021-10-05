[![GitHub](https://img.shields.io/github/license/asmaloney/asmCrashReport)](LICENSE)

# asmCrashReport

Provides a simple way to get stack trace information from crashes when using the [MinGW](https://en.wikipedia.org/wiki/MinGW) 32-bit or macOS clang compilers for [Qt](https://www.qt.io/)-based applications.

This was made to fit my [purposes](https://asmaloney.com/2017/08/code/qt-crash-reporting-for-mingw-32-windows-and-clang-macos), but I think it is general enough to be useful to others.

## Usage

There is a complete example included in this repo.

In your .pro file, you need to include the asmCrashreport.pri file. e.g.:

```
if ( !include( ../asmCrashReport.pri ) ) {
    error( Could not find the asmCrashReport.pri file. )
}
```

This will define **ASM_CRASH_REPORT** for the preprocessor and modify the C/CXX and linker flags to include the debug symbols properly.

In your main.cpp, include the header:

```cpp
#ifdef ASM_CRASH_REPORT
#include "asmCrashReport.h"
#endif
```

In your _main()_ function, set your signal handler _after_ you have declared your **QApplication** and set the application name and version number:

```cpp
QApplication  app( argc, argv );

app.setApplicationName( QStringLiteral( "asmCrashReportExample" ) );
app.setApplicationVersion( QStringLiteral( "1.0.0" ) );

#ifdef ASM_CRASH_REPORT
  asmCrashReport::setSignalHandler( QString(), [] (const QString &inFileName, bool inSuccess) {
     // do something with results - I show a QMessageBox (see example)
  });
#endif
```

**asmCrashReport::setSignalHandler** has the following prototype:

```cpp
// inCrashReportDirPath is the path to directory to write our crash report to. If this is not set, it will use Desktop/<App Name> Crash Logs/
// inLogWrittenCallback is a callback that will be called after the log file is written
void  setSignalHandler( const QString &inCrashReportDirPath = QString(), logWrittenCallback inLogWrittenCallback = nullptr );
```

The callback can be used to show a message to the user about where to find the log file. It's signature must be this:

```cpp
// inLogFileName is the full path to the log file which was written
// inSuccess returns whether the file was successfully written or not
typedef void (*logWrittenCallback)( const QString &inLogFileName, bool inSuccess );
```

## Windows

Windows needs to be able to find the **addr2line** command line tool.

Currently, asmCrashReporter will look for this in a tools directory next to the executable (see _asmCrashReport.cpp_'s **\_addr2line()** function).

### cygwin

I use **addr2line** from [Cygwin](https://www.cygwin.com/).

When sending your build to a user, you will need to include some DLLs alongside the exe to make it work.

The _tools_ directory (or whatever you change it to) should contain:

```
-rwxrwx---+ 1 Administrators None  934931 Nov 21  2015 addr2line.exe
-rwxrwx---+ 1 Administrators None 1033235 Feb 20  2015 cygiconv-2.dll
-rwxrwx---+ 1 Administrators None   42515 Oct 23  2016 cygintl-8.dll
-rwxrwx---+ 1 Administrators None 3319090 Jul 12 05:00 cygwin1.dll
-rwxrwx---+ 1 Administrators None   85011 Mar  3 16:45 cygz.dll
```

These DLLs may be found in your Cygwin install's _bin_ directory.

### MinGW

The prebuilt MinGW Qt installers include **addr2line** in the _bin_ directory. It may require other DLLs in order to work on the target machine. (As mentioned above, I use cygwin, so I'm not sure what is required here.)

## Example Logs

macOS (clang):

```
asmCrashReportExample v1.0.0
07 Aug 2017 @ 09:42:38

Caught SIGFPE: (integer divide by zero)

2   libsystem_platform.dylib            0x00007fffacc42b3a _sigtramp + 26
3   ???                                 0x0000000000000000 0x0 + 0
4   asmCrashReportExample               0x0000000100008bd4 crashTest::function2(int) (in asmCrashReportExample) (main.cpp:26)
5   asmCrashReportExample               0x0000000100008baa crashTest::function1() (in asmCrashReportExample) (main.cpp:31)
6   asmCrashReportExample               0x00000001000085d5 crashTest::crashMe() (in asmCrashReportExample) (main.cpp:13)
7   asmCrashReportExample               0x00000001000083de main + 206
8   libdyld.dylib                       0x00007fffaca33235 start + 1
```

Windows (MinGW32):

```
asmCrashReportExample v1.0.0
07 Aug 2017 @ 13:48:22

EXCEPTION_INT_DIVIDE_BY_ZERO

[0] 0x00000000004056ea crashTest::divideByZero(int) at C:\dev\asmCrashReport\build-example-Qt_5_9_1_MinGW_32bit/../example/main.cpp:18
[1] 0x0000000000405749 crashTest::function2(int) at C:\dev\asmCrashReport\build-example-Qt_5_9_1_MinGW_32bit/../example/main.cpp:25
[2] 0x0000000000405726 crashTest::function1() at C:\dev\asmCrashReport\build-example-Qt_5_9_1_MinGW_32bit/../example/main.cpp:30
[3] 0x0000000000405707 crashTest::crashMe() at C:\dev\asmCrashReport\build-example-Qt_5_9_1_MinGW_32bit/../example/main.cpp:13
[4] 0x0000000000403388 qMain(int, char**) at C:\dev\asmCrashReport\build-example-Qt_5_9_1_MinGW_32bit/../example/main.cpp:61
[5] 0x0000000000404592 ?? at qtmain_win.cpp:?
```

## Pull Requests

Issues and pull requests welcome!

## Notes

If anyone knows why the macOS version doesn't get the first frame correct in the example I'd love to hear from you!

This code might work on Linux too since the code path for macOS should be POSIX compliant, though I haven't tried it. It could also be extended to handle MSVC compiles (or maybe it already does!), but I don't use that compiler so I can't test it.

## More Information

See the post [Crash Reporting For MinGW 32 (Windows) and Clang (macOS) With Qt](https://asmaloney.com/2017/08/code/crash-reporting-for-mingw-32-windows-and-clang-macos-with-qt/) for details.

07 August 2017
Andy Maloney
https://asmaloney.com
