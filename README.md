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

This section is a review of the history behind the development of page tables. Understanding the problems encountered in the past with memory allocation will help you make more sense of how we've arrived at the current solution of page tables. If you feel confident in your understanding, you can attempt to answer the questions on Gradescope and skip over this section.
<!-- 
### Historical Approaches
In the earliest days of computing, when a program needed to access a memory address, it would simply read directly from memory. In this way, there was only a single address space that corresponding to the computer's physical memory. As it became necessary to be able to switch between the execution of multiple programs came the problem of context switches. 

As it became necessary to run multiple programs on a machine, the solution was to dedicated memory spaces for each program in memory and switching them out to the disk for every context switch. While programs were still small, and run in batch, this was not a significant problem, however as programs grew in size, and it became necessary to be able to switch between the execution of multiple programs, it was no longer an ideal solution. Disk operations are slow, and requiring big reads and writes for every context switch creates a large performance overhead. 

To prevent this, the memory spaces for multiple programs can be loaded into memory simultaneously. While previous approaches had dealt simply with memory, this introduces a distinction between *physical memory* and *virtual memory* (and consequently *physical* and *virtual* addresses). Physical memory represents the bits that are actually stored in a computer's memory. Virtual memory is the abstraction of the computer's memory presented to the program.

When dealing with virtual memory, there are three crucial factors which must be ensured:

1. **Transparency**: Other programs should not have any way of knowing about what other programs are running.
2. **Protection**: Programs should noe be able to interact with the memory of other programs, either deliberately or accidentally.
3. <a name="effiency-property"></a> **Effiency**: Using virtual memory should not significantly impact the speed of memory operations.

Historically there have been three broad ways which virtual memory has been implemented:

1. **Static allocation**: Some predetermined offset is added to a program's virtual address. This however cannot ensure protection and thus was never widely used.
2. **Dynamic allocation**: A dedicated hardware component called the *Memory Management Unit* (MMU) allocates a block of memory to a program at runtime. Allocating large blocks can result in *internal fragmentation*, where a large section of memory is allocated to a program because it may be needed in the future, however until that point, it is allocated memory that isn't being utilized.
3. **Segmentation**: Allocates multiple smaller blocks that are non-contigous and dynamically-sized to be allocated to a single program's address space. This avoids the problem of internal fragmentation as the individual segments can be dynamically allocated more space as needed.

The last two approaches, however, introduce a new problem: *external fragmentation*. As programs are moved in and out of memory, allocating space for them in memory will gradually create small chunks of unallocated memory. These chunks while technically being free, are stuck between blocks of allocated memory, and if they are sufficiently small, then it is impossible for them to be utilized, resulting in wasted space.  -->

<!-- ### Paging

*Paging*, being the contemporary approach to virtual memory (and the subject ot today's lab) is the solution to this problem. By allocating memory to each program in a fixed-sized chunk, the computer can guarantee that it will always be able to fully utilize all of the unallocated memory it has available to it. While this does potentially re-introduce the problem of internal fragmentation, this can be minimized by adjusting the size of the blocks being allocated. In the context of physical memory, these blocks are known as *page frames*, while they are known simply as *pages* when applied to virtual memory. 

A *page table* is used in order to map a program's virtual addresses to a physical address, in a process known as *translation*. In order to use our page table to find the physical address, we take the highest order bits (the exact amount varies depending on the implementation) of the virtual address, known as the address's *virtual page number* or VPN. The VPN is used to index the list to get the *page table entry*, which simply contains the *physical page number*[*](## "also known as a physical frame number or PFN") or PPN, which tells us where the virtual page is located in physical memory. Knowing the PFN, and the size of the pages, it is then possible to calculate the offset to the page frame by multiplying. Then the remaining lower order bits can be used to find the physical address, which is some offset within the page.

Due to the third property of effenciency, we want to make page table lookups as fast as possible.  -->

### Page Table Enhancements
The simplest implementation of a page table is known as a *linear page table*. It can be thought of as a list of PPNs, which is indexed by a VPN in order to return the corresponding PPN. Linear page tables have two main flaws. The first is for every time a program wants to read from memory, it must perform two read operations: one from the page table, and then reading from physical memory. Secondly is that they require a lot of space; because it is a list, it will take up space for every page table entry, even if nothing has been allocated.

The first improvement we can make is by using a cache to store the most important page table entries. This cache is known as the *translation lookaside buffer* or TLB. Because caches are significantly faster than RAM, this eliminated the problem of requiring an extra memory operations.

To fix the latter issue there are two approaches. The first is to use fewer larger pages. This, however, exascerbates the issue of internal fragmentation. The more common approach is through *multi-level paging*. Rather than using a list-like structure, use a dictionary. This way, it is not necessary to maintain a page table entry for an unallocated page. 

The multi-level, comes from the notion that this can then be used to create a tree of dictionaries. Where each dictionary is stored in a page, and then maps to multiple levels of page dictionaries, the final level of which (the root nodes) will contain the page table entries. Traditionally each level of an n-level tree is labelled from L0 at the leaves, to Ln-1 at the root. For example x86-64 uses 4 levels L0 to L3. In the worst case this will wind up using more memory than a linear page table, however this is an extreme scenario. In the average case, multi-level page tables can be expected to use less memory. 

## Part 2: Understanding xv6 (5 Points)

While knowing this is good and all, it is also useful to play around with an example in order to get a better feeling for how it workd. Forunately we have sv6 for that. xv6 is designed for a RISC-V 64 system architecture; as such, it uses 64-bit addresses. It uses a 3-level page table, where each page is addressed by 12 bits.

In this lab we will be exploring and playing around with how paging is implemented by the xv6 operating system.

There are 4 files we will be looking at in order:
* `mmu.h`
* `kalloc.c`
* `exec.c`
* `vm.c`

These files are very dense, and hard to make sense of, this is not helped by how the variables are named. Fortunately for us the makefile comes with an option to compile a PDF that is far more sensible. I encourage you to try and look at the source code and comments to see if you can get a sense of what is happening in each of the files. 

Now that you've given up, let's go ahead and look at a more sensible version. 

Run the command `make xv6.pdf` to generate a more readable PDF version of the source code.

While this is useful for understanding what's happening in the code, please provide all answers on Gradescope referencing the source code. 

### mmu.h

This is a header file, which defines a number of constants in how the memory management unit works. 
### kalloc.c

This file defines how memory is allocated for various processes, relevant for us, is that it defines how pages are allocated for user processes.

### exec.c

This file defines how a process gets executed by the OS.

### vm.c

This file defines how virtual memory is handled. It defines the behavior of the page table, and tells the MMU how to convert a virtual address to a physical address. 

There are a couple of important functions to look at. The first is `setupkvm`, where KVM stands for **K**ernel **V**irtual **M**emory. 