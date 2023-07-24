# ProjectEverDriven-CompileKit
The purpose of this project is to create a layer between coding in the HSP and the compilation process for ProjectEverDriven. When compiling directly from the HSP-SDK, the created executable is unable to be launched.

So, I delved into the DLL hspcmp.dll of the HSP-SDK and thought it might be worth trying to compile the code directly with the DLL. Fortunately, an executable compiled like this works like a charm.

Furthermore, I included a translated version of the iconins.exe in this project. This tool can be used to change the icon of the created executable. Initially, I tried using other tools like ResourceHacker, but the executable wouldn't launch if one changes the icon in that manner.
