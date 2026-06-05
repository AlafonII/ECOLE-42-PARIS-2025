/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:11:26 by alex              #+#    #+#             */
/*   Updated: 2025/09/11 10:14:37 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "incl/iter.hpp"

static void printHeader(const std::string& title) {
	std::cout << "\n\033[32m=== " << title << " ===\033[0m\n";
}

struct PrintAny {
	template <typename U>
	void operator()(U const& v) const {
		std::cout << v << " ";
	}
};

struct Increment {
	void operator()(int& v) const {
		v = v + 1;
	}
};

void toUpperChar(char& c) {
	if (c >= 'a' && c <= 'z') {
		c = static_cast<char>(c - 'a' + 'A');
	}
}

template <typename T>
void addBang(std::basic_string<T>& s) {
	s.append(1, static_cast<T>('!'));
}

int main() {
	printHeader("iter with non-const int array (Increment + Print)");
	{
		int a[5] = {1, 2, 3, 4, 5};
		iter(a, 5, Increment());
		iter(a, 5, PrintAny());
		std::cout << "\n";
	}

	printHeader("iter with const int array (Print only)");
	{
		const int a[4] = {10, 20, 30, 40};
		iter(a, 4, PrintAny());
		std::cout << "\n";
	}

	printHeader("iter with char array (toUpper + Print)");
	{
		char s[] = "hello42";
		iter(s, 7, toUpperChar);
		iter(s, 7, PrintAny());
		std::cout << "\n";
	}

	printHeader("iter with std::string array (mutating addBang + Print)");
	{
		std::string arr[3] = {"alpha", "beta", "gamma"};
		iter(arr, 3, addBang<char>);
		iter(arr, 3, PrintAny());
		std::cout << "\n";
	}

	printHeader("iter with const std::string array (PrintOnly)");
	{
		const std::string arr[2] = {"const", "array"};
		iter(arr, 2, PrintAny());
		std::cout << "\n";
	}
	return 0;
}
