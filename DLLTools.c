#include "DLLTools.h"

DLLFUNC hsc_ini;
DLLFUNC hsc_refname;
DLLFUNC hsc_objname;
DLLFUNC hsc_comp;
DLLFUNC hsc_getmes;
DLLFUNC hsc_clrmes;
DLLFUNC hsc_ver;
DLLFUNC hsc_bye;
DLLFUNC pack_ini;
DLLFUNC pack_make;
DLLFUNC pack_exe;
DLLFUNC pack_opt;
DLLFUNC pack_rt;
DLLFUNC hsc3_getsym;
DLLFUNC hsc3_make;
DLLFUNC hsc3_getruntime; // Added for 3.0
DLLFUNC hsc3_run; // Addition for 3.0
DLLFUNC pack_view;
DLLFUNC pack_get;

//static	int dllflg=0;			// DLL uses flag

char *SetDllFunc( char *name, DLL *dll )
{
    // Assign DLL function
    //
    char *ent;
    char fncname[128];
    fncname[0]='_';
    strcpy( fncname+1,name );
    strcat( fncname,"@16" );
    ent = (char *)GetProcAddress( dll->hDLL, fncname );
    if (ent==NULL) {
        dll->dllflag=-1;				// error flag
    }
    return ent;
}

int dll_load(char *libname, DLL **dll) {
    *dll = (DLL*)malloc(sizeof(DLL)*1);
    //		Initalize DLL entry
    //			(result:1=ok)
    //
    (*dll)->hDLL = LoadLibrary( libname );
    if ( (*dll)->hDLL==NULL ) {
        printf("Failed to find dll!");
        return 0;
    }
    return 1;
}

int dll_loadFunction(DLL *dll, char *funcName, DLLFUNC *dllFunc)
{
    dll->dllflag=1;
    *dllFunc = (DLLFUNC)SetDllFunc(funcName, dll);
    if(dll->dllflag==-1) {
        printf("Error while loading: %s\n", funcName);
        fflush(stdout);
    }
    return dll->dllflag;
}

void dll_bye( DLL *dll )
{
    //		Release DLL entry
    //
    if (dll->dllflag==0) return;
    if (dll->dllflag==1) {
//        hsc_bye (0,0,0,0); // Clean up routine
    }
    FreeLibrary( dll->hDLL );
    dll->dllflag=0;
}
