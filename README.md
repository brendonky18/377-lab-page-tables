# COMPSCI 377 LAB: Page Tables

## Purpose

This lab is designed to explore page tables, and how they are used in Xv6. Please make sure that all of your answers to questions in these labs come from work done on the Edlab environment – otherwise, they may be inconsistent results and will not receive points.

Please submit your answers to this lab on Gradescope in the assignment marked “Lab #5’. All answers are due by the time specified on Gradescope. The TA present in your lab will do a brief explanation of the various parts of this lab, but you are expected to answer all questions by yourself. Please raise your hand if you have any questions during the lab section – TAs will be notified you are asking a question. Questions and Parts have a number of points marked next to them to signify their weight in this lab’s final grade. Labs are weighted equally, regardless of their total points.

<!-- Once you have logged in to Edlab, you can clone this repo using

```bash
git clone https://github.com/umass-cs-377/377-lab-threading.git
```

Then you can use `cd` to open the directory you just cloned:

```bash
cd 377-lab-threading
``` -->

<!-- This repo includes a Makefile that allows you to locally compile and run all the sample code listed in this tutorial. You can compile them by running `make`. Feel free to modify the source files yourself, after making changes you can run `make` again to build new binaries from your modified files. You can also use `make clean` to remove all the built files, this command is usually used when something went wrong during the compilation so that you can start fresh. -->

## Part 1: Reviewing Page Tables

This section is a review of the history behind the development of page tables. Understanding the problems encountered in the past with memory allocation will help you make more sense of how we've arrived at the current solution of page tables. If you feel confident in your understanding, you can attempt to answer the questions on Gradescope and skip over this section. Otherwise I would encourage you to review [this](Paging-Explanation.md).
## Part 2: Understanding xv6 (5 Points)

While knowing this is good and all, it is also useful to play around with an example in order to get a better feeling for how it workd. Forunately we have sv6 for that. xv6 is designed for a RISC-V 64 system architecture; as such, it uses 64-bit addresses. It uses a 3-level page table, where each page is addressed by 12 bits.

In this lab we will be exploring and playing around with how paging is implemented by the xv6 operating system. You should have already cloned xv6, so `cd` into the directory where that is located, it should be named `xv6-public`. You're in the right place if you run `ls` and get an output that looks like this:

```
BUGS              console.o       ide.o         ls.c         runoff.list   trap.c
LICENSE           cuth            init.asm      ls.d         runoff.spec   trap.d
Makefile          date.h          init.c        ls.o         runoff1       trap.o
Notes             defs.h          init.d        ls.sym       sh.asm        trapasm.S        
README            dot-bochsrc     init.o        main.c       sh.c          trapasm.o        
TRICKS            echo.asm        init.sym      main.d       sh.d          traps.h
...
```

There should be many more files than what has been shown. I've only included the first few lines so you can check if you're in the right place.

Now that you're in the right directory, there are 4 files we will be looking at in order:
* `mmu.h`
* `kalloc.c`
* `exec.c`
* `vm.c`

While you can use `vim` to view the files, I would reccomend downloading them from the [xv6 repository](https://github.com/mit-pdos/xv6-public) on to your own computer, and open them with your editor of choice...unless that is vim for whatever reason you wierdo.

### mmu.h

This is a header file, which defines a number of constants in how the memory management unit works. 
### kalloc.c

This file defines how memory is allocated for various processes, relevant for us, is that it defines how pages are allocated for user processes.

### exec.c

This file defines how a process gets executed by the OS.

### vm.c

This file defines how virtual memory is handled. It defines the behavior of the page table, and tells the MMU how to convert a virtual address to a physical address. 

There are a couple of important functions to look at. The first is `setupkvm`, where KVM stands for **K**ernel **V**irtual **M**emory. 