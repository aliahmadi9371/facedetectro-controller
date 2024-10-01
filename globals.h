#ifndef GLOBALS_H
#define GLOBALS_H

#include "windef.h"

typedef int  (*ConnectNet)(int, char*, int, int, int, int, int);
typedef int  (*DisConnect)(int);
typedef int  (*EmptyGeneralLogData)(int);
typedef int  (*SetDoorStatus)(int, int);
typedef int  (*EnableDevice)(int, BYTE);
typedef int  (*LoadData)(int, int);
typedef int  (*GetData)(int, int*, int*, int*, int*, int*, int*, int*, int*, int*);
typedef int  (*GeEnroll)(int, int, char*, int*);


#endif // GLOBALS_H
