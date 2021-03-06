# xv6 fsck project
--------------------------



## What we changed (summary)

**Added two syscalls**
- `getinode()`: gets a read only copy of an on-disk inode
- `icount()`: returns the number of inods from the superblock

**Added dwalk program**
Recursively walks the directory tree and records information from files. Information is stored in a linked lists of generic representations of inodes called unodes. 

**Added iwalk program** 
Gets a list of *allocated* inodes on disk and stores them in the same type of structure as dwalk. 

**Added compare program**
This program uses headers from *iwalk* and *dwalk* to compare the outputs of the two programs and check for any inconsistancies. At the moment, it checks if the same inodes are present in both lists, if the lists are the same length, and if the types of the files are the same. 




##Syscalls
I changed these files to add the syscalls

- sysproc.c
- sysfile.c
- syscall.h
- user.h
- syscall.c
- usys.S
- defs.h

**sysproc.c** and **sysfile.c**: holds the code and implementation of the syscalls

**syscall.h**: defines the trap vector for the call to hardware. New syscalls are at the end. 

**user.h**: adds the function definitions that the userland calls

**syscall.c**: Adds 'linking code' that links the trap vector in *syscall.h* to the syscall defined in *sysproc.c* and *sysfile.c*

**usys.S**: Runs a macro that does background work for defining a syscall at a low level. 

**defs.h**: added definition for global use.
 

## New files added

- dwalk.c
- dwalk.h
- dwalkmain.c
- iwalk.c
- iwalk.h
- iwalkmain.c
- compare.c
- compare.h
- comparemain.c
- fsck.c
- filepuke.c

**dwalk.c**: holds inplementation code for directory walker

**dwalk.h**: hold public functions for directory walker

**dwalkmain.c**: runnable program for directory walker

**iwalk.c**: holds implementation code for inode walker

**iwalk.h**: holds public functions for inode walker

**iwalkmain.c**: runnable program for inode walker

**compare.c**: implementation for comparing walker output

**compare.h**: public functions for comparing

**comparemain.c**: runable program for comparing

**fsck.c**: filesystem checker using all above programs to recover and verify filesystem

**filepuke.c**: erases parts of directories
