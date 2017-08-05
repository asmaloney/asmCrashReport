message( "Building example" )

TARGET = asmCrashReportExample
TEMPLATE = app

mac:CONFIG -= app_bundle
CONFIG += c++14

QT += widgets

if ( !include( ../asmCrashReport.pri ) ) {
    error( Could not find the asmCrashReport.pri file. )
}

SOURCES += \
    main.cpp
