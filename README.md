WWIV BBS
========

WWIV is compiled with the following compilers:
  
- MS Visual C++ 2019 Community Edition.
- GCC 8.3 (or later) on Linux 
  (tested on Debian10, Ubuntu 20.04, Centos8 with SCL/GCC-8)
- CMake 3.13 or newer

***

# Building WWIV BBS
***

We prefer contributors to FORK ```wwivbbs``` repositories to their account and work from there.

## Building on Windows

### Installing Git

You will need [Git](https://git-scm.com) installed.  You can use the GitHub Desktop GUI, but it's also easy
to use the commandline tool directly.  You'll need to download [Git](https://git-scm.com/download/win) and
install it.  Make sure the ```git``` command is in your PATH.

If you are using GitHub Desktop, this is likely in the folder: "Documents\GitHub\WWIV".  Otherwise just
create a directory and clone your fork.  You can follow instructions that are written by GitHub 
[Here](https://help.github.com/en/github/getting-started-with-github/fork-a-repo). Just make sure that
when you clone the repo, you have "Recurse Submodules" specified in the tool, or using 
```--recurse-submodules``` on the commandline.

### Download and Install Visual Studio
WWIV is compiled with the VS2019 compiler for windows. 
You can download [Microsoft Visual Studio 2019 Community](https://www.visualstudio.com/downloads/)

Choose to install the ```Desktop development with C++``` workload.
You also may want to optionally install the following "Individual Components":
```
   Git For Windows (Only if you do not have this already)
   GitHub extension for Visaul Studio
```


### Build WWIV (Windows)
* If you cloned a git repository for your fork of WWIV, then select File then Open and choose Folder.

* If you are using the GitHub for Windows extension, then from the Visual Studio IDE, select File and then ```Open from Source Control```
On the bottom, you should see your local GIT repositories already.
Above that you will see Login to GitHub, do that.
* Now in your Local repositories (Documents\GitHub\WWIV), open the
  folder WWIV in Visual Studio. It should recognize the CMake build
  and be able to build WWIV.
* When VS says "READY" on the bottom, go to Build on the menu and select Build Solution(F7). If you have any build errors, run Build one more time and see if that resolves itself as there can be timing issues on some machines.
* You select whether or not you are building DEBUG or RELEASE on the toolbar. Those binaries and other built files will be places in a \debug and \release folder along side your github source files. ex: ```Documents\GitHub\WWIV\debug``` or ```Documents\GitHub\WWIV\release```.


## Building on Linux
This only builds the binaries, it does NOT include the supporting files.
Please follow the
[Linux Installation](http://docs.wwivbbs.org/en/latest/linux_installation/) instructions under
*Steps to install the software* for first installing the base system.

** NOTE:** Do these steps as a non-root user; your BBS user would be the easiest from a file permissions perspective later on.  root should never be used to compile binaries.

### Install pre-requisite software

Package | Comments
------- | ----------
git | to grab the source code for compiling  
ncurses | ncurses-devel, libncurses5-dev, etc depending on your distro
cmake | 3.13 or later
make | for cryptlib
ninja-build | 1.8 or later, earlier versions probably work too
g++ | 8.3.0 or later (easiest to install via build-essential on debian/ubuntu)

If you are on debian or ubuntu, you can use the ```/builds/jenkins/linux/install-prereqs.sh``` 
escript to ensure that the right software is installed.  This command should be executed as root (using sudo)

### WWIV Binaries
Here's the list of binaries that will be built in the build directory:  

* bbs/bbs  
* wwivconfig/wwivconfig  
* wwivd/wwivd  
* wwivutil/wwivutil  
* network/network  
* networkb/networkb
* networkc/networkc
* networkf/networkf
* network1/network1
* network2/network2
* network3/network3

#### Getting the source from GitHub
If you plan to have an active repo, we prefer contributors to FORK WWIVBBS repositories to their account and work from there.  
* [Fork](https://help.github.com/articles/fork-a-repo/), then clone your fork
    
    ```bash
    # Create a directory for your fork's clone.
    mkdir git
    chdir git
    # Clone your fork into the current directory (git).
    # Use your GitHub username instead of <em>YOUR-USERNAME</em>
    git clone --recurse-submodules -j8 https://github.com/<em>YOUR-USERNAME</em>/wwiv.git
    ```
* Navigate to wwiv

#### Compiling WWIV


Now change directory to the ```wwiv``` directory where you cloned the repository
and run the following:
  ```
  mkdir _build
  cd _build 
  ../cmake-config.sh 
  cmake --build .
  ```

Copy all of the files newly built, or symlinks to them from your WWIV base install
i.e. in /home/wwiv/ (assuming the source is in $HOME/git/wwiv now.)
```
 # This should be done as the wwiv user who has the source code.
 cd /home/wwiv
 ln -s $HOME/git/wwiv/builds/tools/linux/use-built-bin.sh
 
 export BUILT_BIN=$HOME/out/wwiv
 ./use-built-bin.sh ${BUILT_BIN} bbs wwivd wwivconfig network network?

```

#### Out of Source Build warning

If you get an error about needing to do an out of source build, please make sure that
you are executing the cmake command from a directory that is not the same as your source
code, such as a subdirectory called ```_b``` or ```_build``` or even a different directory
all together. I use ```/home/rushfan/out/wwiv``` myself.

Once you get a warning about this, you will need to clean the build so you need to
remove the following file and directory:
  * CMakeFiles
  * CMakeCache.txt 

*** 

Installation and SysOp Instructions
====================

All the installation and SysOp administration information you 
need is in the [WWIV Documentation](https://docs.wwivbbs.org/)

***

Get Involved
====================

If you want to help out with WWIV BBS:

* Read the [Contributors Guidelines](contributing.md)
* Check out the [WWIVBBS Homepage](https://www.wwivbbs.org) and find us on IRC.
* Jump into the [Issues List](https://github.com/wwivbbs/wwiv/issues).
