#ifndef DLLTOOLS_H
#define DLLTOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef BOOL (CALLBACK *DLLFUNC)(int,int,int,int);

typedef struct DLL {
    int dllflag;
    HINSTANCE hDLL;
} DLL;

char *SetDllFunc( char *name, DLL *dll );
int dll_load(char *libname, DLL **dll);
int dll_loadFunction(DLL *dll, char *funcName, DLLFUNC *dllFunc);
void dll_bye( DLL *dll );

#endif // DLLTOOLS_H
