
## Historical Approaches
### Managing Memory 
In the earliest days of computing, when a program needed to access a memory address, it would simply read directly from memory. In this way, there was only a single address space that corresponding to the computer's physical memory. As it became necessary to be able to switch between the execution of multiple programs came the problem of context switches. 

As it became necessary to run multiple programs on a machine, the solution was to dedicated memory spaces for each program in memory and switching them out to the disk for every context switch. While programs were still small, and run in batch, this was not a significant problem, however as programs grew in size, and it became necessary to be able to switch between the execution of multiple programs, it was no longer an ideal solution. Disk operations are *s l o o o w*, and requiring big reads and writes for every context switch creates a large performance overhead. To prevent this, the memory spaces for multiple programs can be loaded into memory simultaneously. 

### Virtual Memory

While previous approaches had dealt simply with a unified representation of memory, this development introduces a distinction between *physical memory* and *virtual memory* (and consequently *physical* and *virtual* addresses). Physical memory represents the bits that are actually stored in a computer's memory. Virtual memory is the abstraction of the computer's memory presented to the program.

When dealing with virtual memory, there are three crucial factors which must be ensured:

1. **Transparency**: Other programs should not have any way of knowing about what other programs are running.
2. **Protection**: Programs should not be able to interact with the memory of other programs, either deliberately or accidentally.
3. <a name="efficiency-property"></a> **Efficiency**: Using virtual memory should not significantly impact the speed of memory operations.

Historically there have been three broad ways which virtual memory has been implemented:

1. **Static allocation**: Some predetermined offset is added to a program's virtual address. This however cannot ensure protection and thus was never widely used.
2. **Dynamic allocation**: A dedicated hardware component called the *Memory Management Unit* (MMU) allocates a block of memory to a program at runtime. Allocating large blocks can result in *internal fragmentation*, where a large section of memory is allocated to a program because it may be needed in the future, however until that point, it is allocated memory (which means it can't be used by other programs that may actually need it) that isn't being utilized.
3. **Segmentation**: Allocates multiple smaller blocks that are non-contiguous and dynamically-sized to be allocated to a single program's address space. This avoids the problem of internal fragmentation as the individual segments can be dynamically allocated more space as needed.

The last two approaches, however, introduce a new problem: *external fragmentation*. As programs are moved in and out of memory, allocating space for them in memory will gradually create small chunks of unallocated memory. These chunks while technically being free, are stuck between blocks of allocated memory, and if they are sufficiently small, then it is impossible for them to be utilized, resulting in wasted space.

## Paging

*Paging*, being the contemporary approach to virtual memory (and the subject ot today's lab) is the solution to this problem. By allocating memory to each program in a fixed-sized chunk, the computer can guarantee that it will always be able to fully utilize all of the unallocated memory it has available to it. While this does potentially re-introduce the problem of internal fragmentation, this can be minimized by adjusting the size of the blocks being allocated. In the context of physical memory, these blocks are known as *page frames*, while they are known simply as *pages* when applied to virtual memory. 

A *page table* is used in order to map a program's virtual addresses to a physical address, in a process known as *translation*. In order to use our page table to find the physical address, we take the highest order bits (the exact amount varies depending on the implementation) of the virtual address, known as the address's *virtual page number* or VPN. The VPN is used to index the list to get the *page table entry*, which simply contains the *physical page number*[*](## "also known as a physical frame number or PFN") or PPN, which tells us where the virtual page is located in physical memory. Knowing the PFN, and the size of the pages, it is then possible to calculate the offset to the page frame by multiplying. Then the remaining lower order bits can be used to find the physical address, which is some offset within the page.

Due to the third property of effenciency, we want to make page table lookups as fast as possible. 

## Page Table Enhancements
The simplest implementation of a page table is known as a *linear page table*. It can be thought of as a list/array of PPNs, which is indexed by a VPN in order to return the corresponding PPN. Linear page tables have two main flaws. The first is for every time a program wants to read from memory, it must perform two read operations: one from the page table, and then reading from physical memory. Secondly is that they require a lot of space; because it is a list, it will take up space for every page table entry, even if nothing has been allocated.

The first improvement we can make is by using a cache to store the most important page table entries. This cache is known as the *translation lookaside buffer* or TLB. Because caches are significantly faster than RAM, this eliminated the problem of requiring an extra memory operations.

To fix the latter issue there are two approaches. The first is to use fewer larger pages. This, however, exascerbates the issue of internal fragmentation. The more common approach is through *multi-level paging*. Rather than using a list-like structure, use a dictionary. This way, it is not necessary to maintain a page table entry for an unallocated page. 

The multi-level, comes from the notion that this can then be used to create a tree of dictionaries. Where each dictionary is stored in a page, and then maps to multiple levels of page dictionaries, the final level of which (the root nodes) will contain the page table entries. Traditionally each level of an n-level tree is labelled from L0 at the leaves, to Ln-1 at the root. For example x86-64 uses 4 levels L0 to L3. In the worst case this will wind up using more memory than a linear page table, however this is an extreme scenario. In the average case, multi-level page tables can be expected to use less memory. 