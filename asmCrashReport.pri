
CONFIG (release, release|debug) {
    message( 'Enabling asmCrashReport and including debug symbols' )

    DEFINES += ASM_CRASH_REPORT

    VPATH += $$PWD/src
    DEPENDPATH += $$PWD/src
    INCLUDEPATH += $$PWD/src

    HEADERS += \
        $$PWD/src/asmCrashReport.h

    SOURCES += \
        $$PWD/src/asmCrashReport.cpp

    win32-g++* {

        CONFIG += file_copies
        ADDR2LINE = $$system(where addr2line)
        # https://stackoverflow.com/a/54162789
        addr2line.files = $$ADDR2LINE
        addr2line.path = $$OUT_PWD/release/tools
        COPIES += addr2line

        QMAKE_CFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE -= -O2

        QMAKE_CFLAGS_RELEASE += -g -O0
        QMAKE_CXXFLAGS_RELEASE += -g -O0
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
    message( 'NOTE: asmCrashReport is only valid for release builds' )
}
