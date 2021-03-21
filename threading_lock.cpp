#include <iostream>
#include <thread> 
#include <chrono>
#include <mutex>

using namespace std;

std::mutex mtx;

void printing(){
	//simply prints out a string
	cout << "377 is a class!\n";
}

void truth(bool value){
	//prints out a truth or a lie if the value parameter is true or false, respectively
	int count = 1;
	mtx.lock();
	while (count <= 10){
		if (value){
			cout << "#" << count << ": 377 is cool!\n";
		} else {
			cout << "#" << count << ": 377 is not cool...\n";
		}
		count++;
	}
	mtx.unlock();
	mtx.lock();
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