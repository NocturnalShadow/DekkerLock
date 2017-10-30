#pragma once

#include "fixnum_lockable.h"

#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

inline void test1(FixnumLockable& mutex)
{
	auto task = [&](string message) {
		mutex.attach(this_thread::get_id()); // associate the thread with the mutex	
		mutex.lock(); // enter a critical region
		cout << message << " entered critical region" << endl;
		sleep_for(seconds(1));
		cout << message << " left critical region" << endl;
		mutex.unlock(); // leave the critical region

		sleep_for(milliseconds(250));
		cout << message << " try enter critical region" << endl;
		if (mutex.try_lock()) {	// try enter a critical region
			cout << message << " entered critical region" << endl;
			cout << message << " left critical region" << endl;
			mutex.unlock(); // leave the critical region
		}
		else
		{
			cout << message << " hasn't succsed to enter critical region" << endl;
		}

		mutex.detach(this_thread::get_id()); // this is not obligatory
	};


	thread thread_2{ task, "Thread 2" };
	thread thread_1{ task, "Thread 1" };

	thread_1.join();
	thread_2.join();

	cout << "Main thread." << endl;
}

inline void test2(FixnumLockable& mutex)
{
	auto task = [&](string message) {
		mutex.attach(this_thread::get_id()); // associate the thread with the mutex	
		if (message == "Thread 2") sleep_for(milliseconds(10));
		cout << message << " wants enter critical region " << endl;
		mutex.lock(); // enter a critical region
		cout << message << " entered critical region" << endl;
		sleep_for(seconds(1));
		cout << message << " left critical region" << endl;
		mutex.unlock(); // leave the critical region

		sleep_for(milliseconds(250));
		cout << message << " try enter critical region" << endl;
		if (mutex.try_lock()) {	// try enter a critical region
			cout << message << " entered critical region" << endl;
			cout << message << " left critical region" << endl;
			mutex.unlock(); // leave the critical region
		}
		else
		{
			cout << message << " hasn't succsed to enter critical region" << endl;
		}

		mutex.detach(this_thread::get_id()); // this is not obligatory
	};

	auto task2 = [&](string message) {
		mutex.attach(this_thread::get_id()); // associate the thread with the mutex	
		if (message == "Thread 1") sleep_for(milliseconds(10));
		cout << message << " wants enter critical region " << endl;
		mutex.lock(); // enter a critical region
		cout << message << " entered critical region" << endl;
		sleep_for(seconds(1));
		cout << message << " left critical region" << endl;
		mutex.unlock(); // leave the critical region

		sleep_for(milliseconds(250));
		cout << message << " try enter critical region" << endl;
		if (mutex.try_lock()) {	// try enter a critical region
			cout << message << " entered critical region" << endl;
			cout << message << " left critical region" << endl;
			mutex.unlock(); // leave the critical region
		}
		else
		{
			cout << message << " hasn't succsed to enter critical region" << endl;
		}

		mutex.detach(this_thread::get_id()); // this is not obligatory
	};
	cout << "_______________________________________________" << endl;
	cout << " 1 case: thread 2 wants enter first" << endl;
	cout << "_______________________________________________" << endl;

	thread thread_2{ task2, "Thread 2" };
	thread thread_1{ task2, "Thread 1" };

	thread_1.join();
	thread_2.join();

	cout << "_______________________________________________" << endl;
	cout << " 2 case: thread 1 wants enter first" << endl;
	cout << "_______________________________________________" << endl;

	thread thread_3{ task, "Thread 1" };
	thread thread_4{ task, "Thread 2" };

	thread_4.join();
	thread_3.join();

	cout << "Main thread." << endl;
}

inline void test3(FixnumLockable& mutex)
{
	auto task1 = [&](string message) {
		mutex.attach(this_thread::get_id()); // associate the thread with the mutex	
		mutex.lock(); // enter a critical region
		cout << message << " entered critical region " << endl;
		sleep_for(seconds(1));
		cout << message << " left critical region " << endl;
		mutex.unlock(); // leave the critical region

		sleep_for(milliseconds(250));
		cout << message << " try enter critical region " << endl;
		if (mutex.try_lock()) {	// try enter a critical region
			cout << message << " entered critical region " << endl;
			cout << message << " left critical region " << endl;
			mutex.unlock(); // leave the critical region
		}
		else
		{
			cout << message << " hasn't succsed to enter critical region" << endl;
		}

		mutex.detach(this_thread::get_id()); // this is not obligatory
	};
	auto task2 = [&]() {
		sleep_for(milliseconds(500));
		cout << " Thread 3 started working outside critical region " << endl;
		sleep_for(seconds(1));
		cout << " Thread 3 finished working outside critical region " << endl;
	};

	thread thread_2{ task1, "Thread 2 " };
	thread thread_1{ task1, "Thread 1 " };
	thread thread_3{ task2 };

	thread_1.join();
	thread_2.join();
	thread_3.join();

	cout << "Main thread." << endl;
}

inline void test4(FixnumLockable& mutex)
{
	auto task = [&](string message) {
		mutex.attach(this_thread::get_id()); // associate the thread with the mutex	
		if (message == "Thread 2") sleep_for(milliseconds(10));
		cout << message << " wants enter critical region " << endl;
		mutex.lock(); // enter a critical region
		if (message == "Thread 2") sleep_for(milliseconds(10));
		cout << message << " entered critical region " << endl;
		sleep_for(seconds(1));
		cout << message << " left critical region " << endl;
		mutex.unlock(); // leave the critical region

		if (message == "Thread 1")
		{
			cout << message << " wants enter critical region again " << endl;
			mutex.lock(); // enter a critical region
			cout << message << " entered critical region" << endl;
			sleep_for(seconds(1));
			cout << message << " left critical region" << endl;
			mutex.unlock(); // leave the critical region
		}

		mutex.detach(this_thread::get_id()); // this is not obligatory
	};

	thread thread_2{ task, "Thread 2" };
	thread thread_1{ task, "Thread 1" };

	thread_1.join();
	thread_2.join();

	cout << "Main thread." << endl;
}

