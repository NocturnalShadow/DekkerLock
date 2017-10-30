#pragma once

#include <thread>

template <class T>
concept bool BasicLockable =
requires(T var) {
	var.lock();
	var.unlock();
};

template <class T>
concept bool Lockable =
requires(T var) {
	requires BasicLockable<T>;
	{ var.try_lock() } -> bool;
};

template <class T>
concept bool FixnumLockable =
requires(T var) {
	requires Lockable<T>;
	{ var.get_id() }->std::thread::id;
	{ var.attach(std::this_thread::get_id()) } -> bool;
	{ var.detach(std::this_thread::get_id()) } -> bool;
};