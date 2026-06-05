/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:25:06 by alex              #+#    #+#             */
/*   Updated: 2025/09/10 14:31:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include "incl/Array.hpp"

static void printHeader(const std::string& title) {
	std::cout << "\n\033[32m=== " << title << " ===\033[0m\n";
}

template <typename T>
static void dumpArray(Array<T> const& a) {
	std::cout << "[size=" << a.size() << "] ";
	for (unsigned int i = 0; i < a.size(); ++i) {
		std::cout << a[i] << " ";
	}
	std::cout << "\n";
}

int main() {
	printHeader("Default-constructed empty array");
	{
		Array<int> a;
		std::cout << "size: " << a.size() << "\n";
	}

	printHeader("Array of 5 ints, default-initialized");
	{
		Array<int> a(5);
		dumpArray(a);
		for (unsigned int i = 0; i < a.size(); ++i) {
			a[i] = static_cast<int>(i * 10);
		}
		dumpArray(a);
	}

	printHeader("Copy constructor & deep copy check");
	{
		Array<std::string> a(3);
		a[0] = "alpha"; a[1] = "beta"; a[2] = "gamma";
		Array<std::string> b(a); // copy
		b[1] = "BETA!";          // mutate copy
		std::cout << "a: "; dumpArray(a);
		std::cout << "b: "; dumpArray(b);
	}

	printHeader("Assignment operator & deep copy check");
	{
		Array<int> a(3);
		for (unsigned int i = 0; i < a.size(); ++i) a[i] = i + 1;
		Array<int> b;
		b = a;                    // assign
		b[0] = 999;               // mutate b only
		std::cout << "a: "; dumpArray(a);
		std::cout << "b: "; dumpArray(b);
	}

	printHeader("Const access & bounds checking");
	{
		Array<int> a(2);
		a[0] = 10; a[1] = 20;
		Array<int> const ca = a;
		std::cout << "ca[0]=" << ca[0] << ", ca[1]=" << ca[1] << "\n";
		try {
			std::cout << "Accessing ca[2]...\n";
			std::cout << ca[2] << "\n"; // should throw
		} catch (std::exception const& e) {
			std::cout << "Caught exception: " << e.what() << "\n";
		}
	}

	return 0;
}
