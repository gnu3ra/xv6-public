# What we changed for xv6 lab 3
--------------------------------------

## exec
Code chaged in file `exec.c` in order to control how exec loads programs into memory. 


in `exec.c` at function `exec()`
---
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

in `vm.c` at function `copyuvm()`
---
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


## Makefile
In the makefile, I changed the compiler options so the initial code is loaded into the 2nd page because the first page is invalid. The first address of this page is 0x1000, so I added 


in `Makefile`
--- 
in section `_%: %.o $(ULIB)`
Replaced:
```
$(LD) $(LDFLAGS) -N -e main -Ttext 0x1000 -o $@ $^
```
with
```
$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
```
in section `  _forktest: forktest.o $(ULIB) `
Replaced:
```
$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o ulib.o usys.o
```
with 
```
$(LD) $(LDFLAGS) -N -e main -Ttext 0x1000 -o _forktest forktest.o ulib.o usys.o
```
