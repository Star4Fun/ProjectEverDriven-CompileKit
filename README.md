# ProjectEverDriven-CompileKit
The purpose of this project is to create a layer between coding in the HSP and the compilation process for ProjectEverDriven. When compiling directly from the HSP-SDK, the created executable is unable to be launched.

So, I delved into the DLL hspcmp.dll of the HSP-SDK and thought it might be worth trying to compile the code directly with the DLL. Fortunately, an executable compiled like this works like a charm.

Furthermore, I included a translated version of the iconins.exe in this project. This tool can be used to change the icon of the created executable. Initially, I tried using other tools like ResourceHacker, but the executable wouldn't launch if one changes the icon in that manner.
<h2>Installation</h2>
To install just copy the executeable into hsp-sdk folder.

<h3>How to compile</h3>
After you cloned the projects repository you just need to create a build directory. Then you open a command prompt in the build directory and run:
<br><br>

```
cmake ..
```
After this you can just open the project in visual studio and run ALL_BUILD. Now it's getting compiled.

<h3>Add as external tool to HSP Script Editor</h3>
Open the settings inside of the HSP Script Editor
<br><br>

![0](https://github.com/Star4Fun/ProjectEverDriven-CompileKit/assets/37082237/79a10efb-6cfd-4886-8af2-131062bca5fa)
<br><br>
Select ExtTool from the Tool section and click Add...

![2](https://github.com/Star4Fun/ProjectEverDriven-CompileKit/assets/37082237/2840068a-b497-4d7e-9cb1-cd0972a0162e)
<br><br>
So in the Tool TextField can you select a name. File Path is just the path to the PED-CompileKit.exe and in Command Line one should add a config file and the source file with %F (That is the macro for the current file) If not changing the output directory it will be written to the folder where the source file is located.




<h2>Usage</h2>
You can launch the program with different parameters. I've added the ability to add a config file because the commandline for external tools in HSP Script Editor isn't very long. You use the same switches like the command line except the sign. For example:

```
  icon="C:\Users\Star4Fun\Desktop\start.ico"
  versionfile="C:\Users\Star4Fun\Desktop\modversion"
  source="C:\Users\Star4Fun\Desktop\ProjectEverDriven\start.hsp"
  output=C:\Users\Star4Fun\Desktop
  name=EchidnaWarsDX.exe
  execute
```
```
-i="C:\Users\Star4Fun\Desktop\start.ico"  -v="C:\Users\Star4Fun\Desktop\modversion" -s="C:\Users\Star4Fun\Desktop\ProjectEverDriven\start.hsp" -o=C:\Users\Star4Fun\Desktop -n=EchidnaWarsDX.exe -e -d
```
