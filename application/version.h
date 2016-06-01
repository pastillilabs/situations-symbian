#ifndef VERSION_H
#define VERSION_H

/**
  * VERSION_STRING
  */
#define VERSIONSTRINGBASE "v1.06(16)"

#ifdef Q_WS_SYMBIAN_UI
    #define VERSIONSTRINGPLATFORM " - Nokia Belle"
#endif

#define VERSION_STRING VERSIONSTRINGBASE VERSIONSTRINGPLATFORM


/**
  * SERVER_EXECUTABLE
  */
#ifdef Q_OS_SYMBIAN
    #define SERVER_EXECUTABLE "situationsserver.exe"
#endif
#ifdef Q_WS_SIMULATOR
    #undef SERVER_EXECUTABLE
    #define SERVER_EXECUTABLE "situationsserver.exe"
#endif

#endif // VERSION_H
