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
  
