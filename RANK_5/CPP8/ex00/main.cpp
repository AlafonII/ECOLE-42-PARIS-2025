/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:11:26 by alex              #+#    #+#             */
/*   Updated: 2025/09/10 14:36:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <list>
#include "incl/easyfind.hpp"

static void printHeader(const std::string& title) {
	std::cout << "\n\033[32m=== " << title << " ===\033[0m\n";
}

int main() {
	printHeader("Find in std::vector<int>");
	{
		std::vector<int> v;
		for (int i = 0; i < 5; ++i) v.push_back(i * 10); // 0 10 20 30 40
		try {
			std::vector<int>::iterator it = easyfind(v, 20);
			std::cout << "Found: " << *it << "\n";
		} catch (std::exception const& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	printHeader("Find in std::list<int> (not found case)");
	{
		std::list<int> lst;
		lst.push_back(7); lst.push_back(8); lst.push_back(9);
		try {
			std::list<int>::iterator it = easyfind(lst, 42);
			std::cout << "Found: " << *it << "\n";
		} catch (std::exception const& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	printHeader("Const container overload");
	{
		const std::vector<int> v(3, 5); // 5 5 5
		try {
			std::vector<int>::const_iterator it = easyfind(v, 5);
			std::cout << "Found const: " << *it << "\n";
		} catch (std::exception const& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}
	return 0;
}
