#pragma once

#include "fixnum_lockable.h"

#include <thread>

class DekkerLock
{
	typedef std::thread::id thread_id;

private:
	thread_id attached[2];
	thread_id active;

	bool interested[2];
	bool turn = 0;

	const thread_id no_thread;

private:
	bool current_thread() const {
		auto this_thread_id = std::this_thread::get_id();
		if (this_thread_id == attached[0]) {
			return 0;
		}
		else if (this_thread_id == attached[1]) {
			return 1;
		}
		else {
			throw std::runtime_error("The thread is not attached to this instance.");
		}
	}

public:
	void lock() {
		auto current_thread = this->current_thread();
		interested[current_thread] = true;
		while (interested[!current_thread]) {
			if (turn != current_thread) {
				interested[current_thread] = false;
				while (turn != current_thread) { } // busy waiting
				interested[current_thread] = true;
			}
		}

		// entering a critical region
		active = std::this_thread::get_id();
	}

	void unlock() {
		turn = !current_thread();
		interested[current_thread()] = false;
		active = no_thread;
	}

	bool try_lock() {
		auto current_thread = this->current_thread();
		interested[current_thread] = true;
		if (interested[!current_thread] && turn != current_thread) {
			interested[current_thread] = false;
			return false;
		}

		// entering a critical region
		active = std::this_thread::get_id();

		return true;
	}

	bool attach(thread_id thread) {
		if (attached[0] == no_thread) {
			attached[0] = thread;
			return true;
		}
		else if (attached[1] == no_thread) {
			attached[1] = thread;
			return true;
		}
		else {
			return false;
		}
	}

	bool detach(thread_id thread) {
		while (active == thread);

		if (attached[0] == thread) {
			attached[0] = no_thread;
			return true;
		}
		else if (attached[1] == thread) {
			attached[1] = no_thread;
			return true;
		}
		else {
			return false;
		}
	}

	auto get_id() -> std::thread::id {
		return active;
	}
};
