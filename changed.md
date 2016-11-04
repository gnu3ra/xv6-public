# What we changed for xv6 lab 3
--------------------------------------

## exec
Code chaged in file `exec.c` in order to control how exec loads programs into memory. 


in `exec.c`
Replaced:
```
sz = 0;
```
with
```
sz = PGSIZE - 1; 
```
This causes exec to load the memory of a program into the first address of the second page, instead of loading it into the first address of the first page. This means that all addresses below the code-loading address are candidates for being considered invalid

## fork
When fork is run, a copy of the address space of the parent is sent over to the child. This process must also contain a copy of the page table. Since the page table needs to be smaller (not including the unused first page), we decreased the length of the loop used to copy the page table itself. 

in `vm.c`
Replaced:
```
for(i = 0; i < sz; i += PGSIZE) {
...
```
with
```
for(i = PGSIZE; i < sz; i += PGSIZE){
...
```



