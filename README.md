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

## Part 1: Reviewing Page Tables (13 Points)

This section is a review of the history behind the development of page tables. Understanding the problems encountered in the past with memory allocation will help you make more sense of how we've arrived at the current solution of page tables. If you feel confident in your understanding, you can attempt to answer the questions on Gradescope and skip over this section. Otherwise I would encourage you to review [this](Paging-Explanation.md).

## Part 2: Now You're Thinking with Pages (20 Points)

Use the following to answer the questions 2.2 and 2.3 on gradescope. I

### Single-level page table
Imagine an 8-bit address space, and a single-level page table. Each VPN is 4 bits, and the offset is the remaining 4 bits.

#### Page Table:
A section of the page table:
| VPN   | PPN   |
|-------|-------|
| `0x0` | `0x3` |
| `0x1` | `0x0` |
| `0x2` | `0x2` |
| `0x3` | `0x1` |
| `0x4` | `0x7` |
| `0x5` | `0x4` |
| `0x6` | `0x5` |
| `0x7` | `0x6` |
| ...   | ...   |
| `0xf` | `0xf` |
#### Memory
A section of the memory:
```
0x00: 0xdc      0x20: 0x42      0x40: 0x30      0x60: 0xde
      0x7e            0x0c            0x80            0xad
      0x9d            0x6a            0x0d            0xbe
      0xc0            0xb4            0xa0            0xef
      0xe6            0xd7            0x12            0xad
      0x08            0x5b            0xbe            0x6d
      0xcd            0x92            0xe4            0x18
      0x71            0xc1            0x68            0xa1
      0xd6            0xf4            0x06            0x3f
      0x61            0xa6            0xe6            0xa0
      0x12            0x0f            0x1a            0xcd
      0xb1            0x0b            0xec            0x97
      0x88            0xf4            0x1d            0xa4
      0xbc            0x5c            0x76            0xb5
      0x66            0x01            0x6a            0x1b
      0x5a            0xf0            0xc2            0xd9
0x10: 0x39      0x30: 0x42      0x50: 0x36      0x70: 0xa4
      0xea            0x0c            0x13            0x9a 
      0x27            0x6a            0xf0            0x6f 
      0x47            0xb4            0xd2            0x80 
      0x38            0xd7            0xdd            0x9f 
      0xbe            0x5b            0xf9            0xb1 
      0x23            0x92            0x59            0xbd 
      0xda            0xc1            0x2f            0xc0 
      0x75            0xf4            0x24            0xcf 
      0xec            0xa6            0xb2            0x3f 
      0x96            0x0f            0x81            0xfe 
      0x9b            0x0b            0x02            0x78 
      0xbb            0xf4            0xbb            0x54 
      0x44            0x5c            0x1d            0xaf 
      0xb4            0x01            0x19            0x2d 
      0xd8            0xf0            0x4d            0xcc
```

### Multi-level page table
Imagine a 16-bit address space, and multi-level page table with 2 levels, L0 and L1. Each VPN is 4 bits, and the offset is the remaining 8 bits.
#### L1 Page Table
A section of the L1 page table:
| VPN   | PPN    |
|-------|--------|
| `0x0` | `0xfc` |
| `0x1` | `0xfe` |
| `0x2` | `0xfd` |
| `0x3` | `0xff` |
| `0x4` | `0xfb` |
| `0x5` | `0xf0` |
| `0x6` | `0xfa` |
| `0x7` | `0xf1` |
| ...   | ...    |
#### L0 Page Tables
A section of the last 4 L0 page tables:

Page Tables at PPN `0xfb` and `0xfc`:
| VPN(`0xfb`)| PPN         | | VPN(`0xfc`)| PPN         |      
|------------|-------------|-|------------|-------------|      
| ...        | ...         | | ...        | ...         |    
| `0x5`      | `0x00`      | | `0x9`      | `0x06`      |      
| `0x6`      | `0x02`      | | `0xa`      | `0x05`      |      
| ...        | ...         | | ...        | ...         |        



Page Tables at PPN `0xfd` and `0xfe` rexpectively:
| VPN(`0xfd`)| PPN         | | VPN(`0xfe`)| PPN         |     
|------------|-------------|-|------------|-------------|    
| ...        | ...         | | ...        | ...         |      
| `0x7`      | `0x03`      | | `0xc`      | `0x04`      |      
| `0x8`      | `0x01`      | | `0xd`      | `0x06`      |      
| ...        | ...         | | ...        | ...         |    
  

Sections of memory:
```
0x0000: 0x20be    0x0020: 0x77b7    0x0040: 0x18ed     0x0060: 0x6ddf     
        0xdcbb            0x77b3            0x32cd             0xa79d     
        0x1e31            0xecf4            0x31a0             0x4717     
        0x9bbd            0x951f            0x2585             0xb4e2     
        0xdfbe            0x4043            0xf804             0xf0b4     
        0x16ff            0x830c            0xd1ee             0xcce5     
        0x408a            0xc3e0            0xe9ef             0x16c9     
        0xcb19            0x97fe            0x79ac             0xc13d     
        0xa262            0x857b            0xa695             0xfdca     
        0xb3f2            0x779e            0x448f             0xd45e     
        0x9d7e            0xbccb            0x629f             0x59b8     
        0xac12            0xdf8c            0x9265             0x6e75     
        0x1815            0x264d            0x63a4             0xb38e     
        0x0bcd            0xbb3b            0x991f             0x2c8a     
        0xbe73            0x687f            0xd5ad             0xc52b     
        0x88ac            0x73b3            0xf2ee             0x357c     
0x0010: 0xbd7f    0x0030: 0x82b6    0x0050: 0xac88     0x0070: 0x27c8     
        0x9488            0x264f            0xea00             0x3df3     
        0x15f0            0x7531            0x3f69             0xd775     
        0xb6bf            0xe706            0x4d80             0x5d63     
        0x264a            0x75a2            0x6b6c             0x640b     
        0xb73f            0xf620            0xef4c             0xd0cc     
        0x0bf9            0x04db            0xdc79             0x5d4f     
        0x0876            0xc221            0x647c             0x57da     
        0xa307            0x6508            0xa226             0x3f21     
        0x0549            0xf133            0x911b             0x22ff     
        0xbdbe            0x348f            0x6561             0x3385     
        0xb5fb            0xa2ad            0x29ff             0x4369     
        0x84e9            0xb3a4            0x1c59             0xf7ce     
        0x4074            0x8ada            0x9073             0x2395     
        0x7bfc            0x7649            0x3688             0x4532     
        0x2adb            0x8895            0x7aad             0x045e
```
and:
```
0x0100: 0xe0cf    0x0120: 0xb271     0x0140: 0x50d5     0x0160: 0x8582
        0x6be3            0xf4a4             0x898f             0x7bed
        0xb144            0x4ba9             0x9e47             0xe351
        0xc89b            0xa58b             0x9cfc             0x7aa1
        0x57c6            0x5e4e             0x6cce             0x8053
        0xbdf8            0xf6de             0xcbef             0x37c7
        0xb601            0xd131             0xaae8             0x9b62
        0x4c40            0x0249             0x56cb             0x8db3
        0x0a8b            0x24b2             0x4be2             0xae95
        0x2d82            0x6465             0x0435             0x5f38
        0xbe46            0x08d2             0x42bd             0x5284
        0xfa63            0x9351             0x400b             0xfccb
        0x27be            0xdac0             0xfadb             0x8b46
        0x91d1            0xb168             0x587b             0xf053
        0xb099            0x7645             0xbf4f             0x341c
        0x8822            0x5627             0xcb29             0x7807
0x0110: 0xdfab    0x0130: 0x5a38     0x0150: 0x5563     0x0170: 0x5106
        0x3d7d            0x0b8e             0x8494             0x42e0
        0xfbfd            0x3069             0xa95b             0xa854
        0x022c            0x0b62             0x917f             0x0a6d
        0xf95a            0x3904             0xad9e             0x40ae
        0x974b            0xf2eb             0x3e83             0xe8f4
        0x813d            0x6dfe             0x2219             0xafde
        0xfe9b            0xf7c6             0xd3c5             0x1ea5
        0x8132            0xb717             0x9129             0xbf8c
        0x14d4            0x2838             0x424d             0x5511
        0x7229            0xc866             0x7671             0x3967
        0x227c            0xac24             0xfb7c             0x0282
        0xd1c1            0x15ee             0x7c07             0x0a41
        0x8e2e            0x259c             0x5fde             0x0a07
        0xbff4            0x142b             0x7285             0x168f
        0xbb27            0x32f6             0x9ff5             0xe51b

```
## Part 3: Understanding xv6 (2 Points)

While knowing this is good and all, it is also useful to play around with an example in order to get a better feeling for how it worked. Forunately we have sv6 for that. xv6 is designed for a RISC-V 64 system architecture; as such, it uses 64-bit addresses. It uses a 3-level page table, where each page is addressed by 12 bits. This means that the offset for each page is 12 bits.

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
<!-- 
There are a couple of important functions to look at. The first is `setupkvm`, where KVM stands for **K**ernel **V**irtual **M**emory.  -->

<!-- 
TODO:
Have them allocate a variable, and then print out the page table, and manually navigate the 
 -->