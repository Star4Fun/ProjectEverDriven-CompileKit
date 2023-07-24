#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ShlObj.h>

#include "DLLTools.h"
//#include "hsp3/hsp3struct.h"

const char *usage = "Usage of the program: \n \
    -i or --icon=iconpath           specify the icon the executeable will use \n \
    -o or --output=exepath          specify the path for the executeable \n \
    -n or --name=name               specify the name of the executeable \n \
    -s or --source=sourcepath       specify the path of start.hsp \n \
    -v or --versionfile=versionpath specify the path of the versionfile \n \
    -h or --help                    print this help \n \
    -e or --execute                 execute program after compiling \n \
    -d or --debug                   enabling verbose debug output \n \
    -c or --config                  use a config file instead of options (For HSP Script Editor) \n \
    \nYou can use the path relativ to this executeable or give the full path.\
    \nRequired are only the output path, the output name and the sourcefile.\n";

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

DLL* dllFile;

DLL* hspdaDll;

DLLFUNC sortval;
DLLFUNC sortget;
DLLFUNC vsave_start;
DLLFUNC vsave_put;
DLLFUNC vsave_end; // Added for 3.0
DLLFUNC vload_start; // Addition for 3.0
DLLFUNC vload_get;
DLLFUNC vload_end;

char *iconpath = NULL, *versionpath = NULL, *exename = NULL, *exepath = NULL, *sourcepath = NULL, *objname = NULL, *configpath = NULL;
//some booleans
int executeAfter = 0, changeIcon = 0, changeVersionInfo = 0;

int error = 0;
int debug = 0;

int init( char *libname )
{

    dll_load(libname, &dllFile);
    dll_loadFunction(dllFile, "hsc_ini", &hsc_ini);
    dll_loadFunction(dllFile, "hsc_refname", &hsc_refname);
    dll_loadFunction(dllFile, "hsc_objname", &hsc_objname);
    dll_loadFunction(dllFile, "hsc_comp", &hsc_comp);
    dll_loadFunction(dllFile, "hsc_getmes", &hsc_getmes);
    dll_loadFunction(dllFile, "hsc_clrmes", &hsc_clrmes);
    dll_loadFunction(dllFile, "hsc_ver", &hsc_ver);
    dll_loadFunction(dllFile, "hsc_bye", &hsc_bye);
    dll_loadFunction(dllFile, "pack_ini", &pack_ini);
    dll_loadFunction(dllFile, "pack_make", &pack_make);
    dll_loadFunction(dllFile, "pack_exe", &pack_exe);
    dll_loadFunction(dllFile, "pack_opt", &pack_opt);
    dll_loadFunction(dllFile, "pack_rt", &pack_rt);
    dll_loadFunction(dllFile, "hsc3_getsym", &hsc3_getsym);
    dll_loadFunction(dllFile, "hsc3_make", &hsc3_make);
    dll_loadFunction(dllFile, "hsc3_getruntime", &hsc3_getruntime);
    dll_loadFunction(dllFile, "hsc3_run", &hsc3_run);
    dll_loadFunction(dllFile, "pack_view", &pack_view);
    dll_loadFunction(dllFile, "pack_get", &pack_get);

    return 0;
}

int compareCommand(char *name0, char *name1, char *argToCompare) {
    int len0 = strlen(name0);
    int len1 = strlen(name1);

    char *name0Variant = (char *) malloc(sizeof(char) * len0);
    char *name1Variant = (char *) malloc(sizeof(char) * (len1-1));

    strncpy(name0Variant, name0+1, len0-1);
    strncpy(name1Variant, name1+2, len1-2);
    name0Variant[len0-1] = '\0';
    name1Variant[len1-2] = '\0';

    if(!strncmp(name0, argToCompare, len0) || !strncmp(name1, argToCompare, len1) || !strncmp(name0Variant, argToCompare, len0-1) || !strncmp(name1Variant, argToCompare, len1-2)) {
        free(name0Variant);
        free(name1Variant);
        return 1;
    }
    free(name0Variant);
    free(name1Variant);
    return 0;
}

/*
 * Splits the command at an equals char. Returns true at a success and put result into the split parameter.
 */
int splitCommand(char *command, char **split) {
    *split = strtok(command, "=");
    *split = strtok(NULL, "=");
    if(*split != NULL) {
        return 1;
    }
    return 0;
}

/*
 * Allocates memory for destination string and copys content of src into it. Remember to free destination later!
 */
void cpyStr(char *src, char **dest) {
    *dest = (char *) malloc(sizeof(char) * strlen(src));
    strcpy(*dest, src);
}

int fileExists(char* path) {
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

void checkForError(char *command, int val) {
    static char errbuf [32000]; // Error message storage area
    if(val) {
        hsc_getmes ((int) errbuf, 0,0,0); // get the error
        printf("Error while proceeding: %s\n%s\n", command, errbuf);
    }
}


void deleteChars(char *theString, char **result, char *charsToDelete) {
    int deleteCount = 0;
    int charCnt = strlen(charsToDelete);
    for(int i = 0; i < strlen(theString); i++) {
        for(int j = 0; j < charCnt; j++) {
            if(theString[i] == charsToDelete[j]) {
                deleteCount++;
                break;
            }
        }
    }
    *result = (char *) malloc(sizeof(char)*strlen(theString)-deleteCount+1);

    char *ptr = *result;

    int toDelete = 1;

    for(int i = 0; i < strlen(theString); i++) {
        toDelete = 1;
        for(int j = 0; j < charCnt; j++) {
            if(theString[i] == charsToDelete[j]) {
                toDelete = 0;
            }
        }
        if(toDelete) {
            *ptr = theString[i];
            ptr++;
        }
    }
    *ptr = '\0';
}

void unsolvableError() {
    fflush(stdout);
    if(debug) {
        //Wait a moment to analyse the output in HSP Script Editor
        Sleep(15000);
    }
    exit(1);
}

int checkArgument(char *arg) {
        if(compareCommand("-h", "--help", arg)) {
            printf("%s", usage);
            return 0;
        }
        else if(compareCommand("-i", "--icon", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 0;
            }
            cpyStr(c, &iconpath);
            if(debug) {
                printf("Iconpath: %s\n", iconpath);
            }
            changeIcon = 1;
        }
        else if(compareCommand("-o", "--output", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 0;
            }
            cpyStr(c, &exepath);
            if(debug) {
                printf("Exepath: %s\n", exepath);
            }
        }
        else if(compareCommand("-n", "--name", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 0;
            }
            cpyStr(c, &exename);
            if(debug) {
                printf("Exename: %s\n", exename);
            }
            char *tmp;
            cpyStr(exename, &tmp);
            tmp = strtok(tmp, "\\.");
            objname = (char *) malloc(sizeof(char) * (strlen(tmp)+4));
            sprintf(objname, "%s.ax", tmp);
            free(tmp);
        }
        else if(compareCommand("-s", "--sourcepath", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 0;
            }
            cpyStr(c, &sourcepath);
            if(debug) {
                printf("Sourcefilepath: %s\n", sourcepath);
            }
            if(!fileExists(sourcepath)) {
                printf("Couldn't find source file looked at: %s\n", sourcepath);
                return 1;
            }
        }
        else if(compareCommand("-v", "--versionfile", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 0;
            }
            cpyStr(c, &versionpath);
            if(debug) {
                printf("Versionfilepath: %s\n", versionpath);
            }
            changeVersionInfo = 1;
        }
        else if(compareCommand("-e", "--execute", arg)) {
            executeAfter = 1;
            if(debug) {
                printf("Will execute file after compiling!\n");
            }
        }
        else if(compareCommand("-c", "--config", arg)) {
            char *c;
            error = !splitCommand(arg, &c);
            if(error) {
                return 1;
            }
            cpyStr(c, &configpath);
            if(debug) {
                printf("Configfilepath: %s\n", configpath);
            }
            FILE *config = fopen(configpath, "r");
            char buf[512];
            char *result;

            while(!feof(config)) {
                fgets(buf, 512, config);
                char toDelete[] = { '\"', '\n', '\0' };
                deleteChars(buf, &result, toDelete);
                if(checkArgument(result)) {
                    error = 1;
                    break;
                }
                free(result);
            }
            fclose(config);
        }
        else if(!compareCommand("-d", "--debug", arg)) {
            printf("Unknown parameter: %s\n", arg);
        }
    return 0;
}

int main(int argcnt, char **args) {
    for(int i = 0; i < argcnt; i++) {
        if(compareCommand("-d", "--debug", args[i])) {
            debug = 1;
            printf("Enabling debug output!\n");
        }
    }
    fflush(stdout);

    for(int i = 0; i < argcnt; i++) {
        if(argcnt == 1) {
            error = 1;
        }
        if(i > 0) {
            if(checkArgument(args[i])) {
                unsolvableError();
            }
        }
        if(error) {
            break;
        }
    }
    if(!sourcepath|| !exename) {
        error = 1;
    }
    else if(!exepath) {
        exepath = (char *) malloc(sizeof(char) * strlen(sourcepath));
        strcpy(exepath, sourcepath);
        char *rightmostSlash = strrchr(exepath, '\\');
        if(*rightmostSlash != '\n') {
            *rightmostSlash = '\0';
        }
        if(debug) {
            printf("Output the generated executable to: %s\n", exepath);
        }
    }

    if(error) {
        printf("%s", usage);
        unsolvableError();
    }

    if(!fileExists("hspcmp.dll")) {
        printf("Couldn't find hspcmp.dll. Make sure that it is located in the same folder like this program.\n");
        unsolvableError();
    }

    //Load the hspcmp dll (For details look at hspcmp txt file in sdk folder!)
    init("hspcmp.dll");

    //Create hsp file
    checkForError("HSP hsc_ini", hsc_ini( 0,(int)sourcepath, 0,0 ));
    checkForError("HSP hsc_objname", hsc_objname( 0,(int)objname, 0,0 ));
    checkForError("HSP hsc_comp", hsc_comp( 1,0,0,0 ));

    //This has to be done because the main file is forced to be called start.ax. If we call it anything else the executable won't be able to launch.
    CopyFile(objname, "start.ax", FALSE);

    //Generate the packfile
    char output[128];
    sprintf(output, ";\n;	source generated packfile\n;\n+%s", "start.ax");
    FILE *pack = fopen("packfile", "w");
    fputs(output, pack);
    fclose(pack);

    //Create dpm file
    checkForError("DPM pack_ini", pack_ini(0, (int)objname, 0, 0));
    //checkForError("DPM pack_get", pack_get(0, (int)"packfile", 0, 0));
    checkForError("DPM pack_make", pack_make(0, 0, 0, 0));

    //Create exe
    checkForError("EXE pack_exe", pack_exe(0, 0, 0, 0));

    char fullPathExe[MAX_PATH];
    sprintf(fullPathExe, "%s\\%s", exepath, exename);

    if(fileExists("iconins.exe")) {
        char cmd[512];

        if(changeIcon && changeVersionInfo) {
            sprintf(cmd, "iconins.exe -e\"%s\" -i\"%s\" -l\"1033\" -v\"%s\"", fullPathExe, iconpath, versionpath);
        }
        else if(changeIcon) {
            sprintf(cmd, "iconins.exe -e\"%s\" -i\"%s\" -l\"1033\"", fullPathExe, iconpath);
        }
        else if(changeVersionInfo) {
            sprintf(cmd, "iconins.exe -e\"%s\" -l\"1033\" -v\"%s\"", fullPathExe, versionpath);
        }

        if(debug) {
            printf("Run iconins like following: %s\n", cmd);
        }

        if(debug) {
            system(cmd);
        }
        else {
            sprintf(cmd, "%s > NUL 2>&1", cmd);
            system(cmd);
        }
    }
    else if(debug) {
        printf("Couldn't find iconins.exe! (Place it in same directory as this executable!)\n");
    }

    dll_bye(dllFile);

    if(remove("EchidnaWarsDX.ax")) {
        printf("Failed to remove EchidnaWarsDX.ax!\n");
    }
    if(remove("start.ax")) {
        printf("Failed to remove start.ax!\n");
    }
    if(remove("EchidnaWarsDX.dpm")) {
        printf("Failed to remove EchidnaWarsDX.dpm!\n");
    }
    if(remove("packfile")) {
        printf("Failed to remove packfile!\n");
    }

    if(CopyFile(exename, fullPathExe, FALSE)) {
        remove(exename);
        SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH, exepath, NULL);
    }

    //Execute HSP3 process
    if(executeAfter) {
        char *exeInQuotes = (char *) malloc(sizeof(char)*strlen(fullPathExe)+2);
        sprintf(exeInQuotes, "\"%s\"", fullPathExe);
        system(exeInQuotes);
        free(exeInQuotes);
    }
    free(iconpath);
    free(versionpath);
    free(exename);
    free(exepath);
    free(sourcepath);
    free(objname);
    free(configpath);

    return 0;
}
