# COMPSCI 377 LAB #5: Threading

## Purpose

This lab is designed to cover threads and how to use gdb to debug threads. Please make sure that all of your answers to questions in these labs come from work done on the Edlab environment – otherwise, they may be inconsistent results and will not receive points.

Please submit your answers to this lab on Gradescope in the assignment marked “Lab #5’. All answers are due by the time specified on Gradescope. The TA present in your lab will do a brief explanation of the various parts of this lab, but you are expected to answer all questions by yourself. Please raise your hand if you have any questions during the lab section – TAs will be notified you are asking a question. Questions and Parts have a number of points marked next to them to signify their weight in this lab’s final grade. Labs are weighted equally, regardless of their total points.

Once you have logged in to Edlab, you can clone this repo using

```bash
git clone https://github.com/umass-cs-377/377-lab-threading.git
```

Then you can use `cd` to open the directory you just cloned:

```bash
cd 377-lab-threading
```

This repo includes a Makefile that allows you to locally compile and run all the sample code listed in this tutorial. You can compile them by running `make`. Feel free to modify the source files yourself, after making changes you can run `make` again to build new binaries from your modified files. You can also use `make clean` to remove all the built files, this command is usually used when something went wrong during the compilation so that you can start fresh.

## Part 1: Threads (5 Points)

With many programs, it can be advantageous to have multiple processes running at once (for example, a simple game server could be running several games at a time, one per process). To this end, we use threading, which allows us to run multiple processes from the same original process. We can make multiple threads conduct different operations, run simultaneously, or wait for each other to finish. Please look at the code below, and read the commented sections:

threading.cpp
```c++
#include <iostream>
#include <thread> 
#include <chrono>

using namespace std;

void printing(){
	//simply prints out a string
	cout << "377 is a class!\n";
}

void truth(bool value){
	//prints out a truth or a lie if the value parameter is true or false, respectively
	int count = 1;
	while (count <= 10){
		if (value){
			cout << "#" << count << ": 377 is cool!\n";
		} else {
			cout << "#" << count << ": 377 is not cool...\n";
		}
		count++;
	}
}

int main() {
	std::thread one(truth, false); //Creates a thread that will run the truth() method with the parameter of 'false'
	std::thread two(printing); //Creates a thread that will run the printing() method
	std::thread three(truth, true); //Creates a thread that will run the truth() method with the parameter of 'true'

	//Each of the below lines starts a thread, and pauses the execution of the main function until each of them is finished.
	one.join(); //Runs thread one
	cout << "Thread #1 finished.\n";

	two.join(); //Runs thread two
	cout << "Thread #2 finished.\n";

	three.join(); //Runs thread three
	cout << "Thread #3 finished.\n";

	cout << "All threads finished.\n";

	return 0;
}
```

As can be seen, using join() pauses the main method, but does not necessarily pause the other threads from running since they were created before join() is called. However, the main method will wait until thread one is finished to go past one.join(), until thread two is finished to go past two.join(), and until thread three is finished to go past three.join().

## Part 2: GDB (10 Points)

We have gone over basic GDB before in lab, but today will be focused around utilizing GDB with threading to debug programs that use multiple threads. To run gdb on a process that uses multiple threads, simply open it with gdb like any other executable (gdb \_\_\_\_, where \_\_\_\_ is the name of the executable). When you do so, you should be able to see output as such:
 
 \[New Thread 0x7f85e5bce700 (LWP 364)]

As can be seen in the screenshot above, the memory address is shown for the created thread in the form 0x\_\_\_\_, and the LWP address for the thread is shown after LWP. The LWP address of a thread isn’t something we will work with in this class, but it useful to know that it is the identifier assigned to the process by the operating system. In the above image, the address of the thread is 0x7f85e5bce700, and the LWP of the thread is 364.

We can also find out more information about threads through our use of gdb. The ‘info threads’ command will list the info in the screenshot for all currently running threads from the process, as well as the id of its progenitor thread. The thread we are currently in will have an asterisk to its left. Threads that request time to wait will also show the amount of time they requested and need – the threads in threading.cpp will do this when run via gdb. Each of the threads also has a local ID listed on the right – using gdb, we can switch between these threads with the command ‘thread \_\_\_\_’, where \_\_\_\_ is the id of the thread we are trying to switch to.

## Part 3: Mutex (5 Points)

Threads can be extremely useful, but they can also encounter errors when they attempt to modify the same memory as one another. For example, if two threads each try to increment a value by 1, it may be that both operations occur at the same time, and the value is only incremented once instead of the two times it should be incremented instead. For this reason, we use various techniques to ensure that critical data components are not modified outside of their desired scope. One of these structures is called a lock, which is shared between threads and acts as per its name to prevent other threads from accessing sensitive data while it is locked. Please look at the modified threading.cpp below, called threading_lock.cpp:

threading_lock.cpp
```c++
#include <iostream>
#include <thread> 
#include <chrono>
#include <mutex>

using namespace std;

std::mutex mtx; //Creates the shared mutex

void printing(){
	//simply prints out a string
	cout << "377 is a class!\n";
}

void truth(bool value){
	//prints out a truth or a lie if the value parameter is true or false, respectively
	int count = 1;
	mtx.lock(); //Locks the mutex
	while (count <= 10){
		if (value){
			cout << "#" << count << ": 377 is cool!\n";
		} else {
			cout << "#" << count << ": 377 is not cool...\n";
		}
		count++;
	}
	mtx.unlock(); //Unlocks the mutex
}

int main() {
	std::thread one(truth, false); //Creates a thread that will run the truth() method with the parameter of 'false'
	std::thread two(printing); //Creates a thread that will run the printing() method
	std::thread three(truth, true); //Creates a thread that will run the truth() method with the parameter of 'true'

	//Each of the below lines starts a thread, and pauses the execution of the main function until each of them is finished.
	one.join(); //Runs thread one
	cout << "Thread #1 finished.\n";

	two.join(); //Runs thread two
	cout << "Thread #2 finished.\n";

	three.join(); //Runs thread three
	cout << "Thread #3 finished.\n";

	cout << "All threads finished.\n";

	return 0;
}
```

As you can see when running this code, thread #1 will always finish printing out its 10 statements before thread #3 prints out any of its 10 statements. This is because when we lock mtx at the start of truth(), it prevents further calls of truth() to progress past that point until we call mtx.unlock() from the same thread that locked it. In essence, this allows us to stop any threads that rely on mtx, ensuring that certain pieces of code do not run at the same time as one another even if we want multiple processes to be running simultaneously.
