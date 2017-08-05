
CONFIG (release, release|debug) {
    message( 'Enabling asmCrashReport & forcing debug symbols' )
    message( ' Use ASM_CRASH_REPORT to wrap crash report code' )

    DEFINES += ASM_CRASH_REPORT

    VPATH += $$PWD/src
    DEPENDPATH += $$PWD/src
    INCLUDEPATH += $$PWD/src

    HEADERS += \
        $$PWD/src/asmCrashReport.h

    SOURCES += \
        $$PWD/src/asmCrashReport.cpp

    win32 {
        QMAKE_CFLAGS_RELEASE += -g
        QMAKE_CXXFLAGS_RELEASE += -g
        QMAKE_LFLAGS_RELEASE =

        LIBS += "-L$$PWD/Win/WinDebug" -lDbghelp
    }

    mac {
        QMAKE_CFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE -= -O2

        QMAKE_CFLAGS_RELEASE += -g -fno-pie -fno-omit-frame-pointer -O0
        QMAKE_CXXFLAGS_RELEASE += -g -fno-pie -fno-omit-frame-pointer -O0
        QMAKE_LFLAGS_RELEASE += -Wl,-no_pie
    }
}

CONFIG (debug, release|debug) {
    message( 'NOTE: asmCrashReport only valid for release builds' )
}
