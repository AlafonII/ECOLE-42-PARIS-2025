/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:11:26 by alex              #+#    #+#             */
/*   Updated: 2025/09/10 14:18:29 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "incl/whatever.hpp"

static void printHeader(const std::string& title) {
	std::cout << "\n\033[32m=== " << title << " ===\033[0m\n";
}

int main() {
	printHeader("Subject tests: int and std::string");
	{
		int a = 2;
		int b = 3;
		::swap(a, b);
		std::cout << "a = " << a << ", b = " << b << std::endl;
		std::cout << "min(a, b) = " << ::min(a, b) << std::endl;
		std::cout << "max(a, b) = " << ::max(a, b) << std::endl;

		std::string c = "chaine1";
		std::string d = "chaine2";
		::swap(c, d);
		std::cout << "c = " << c << ", d = " << d << std::endl;
		std::cout << "min(c, d) = " << ::min(c, d) << std::endl;
		std::cout << "max(c, d) = " << ::max(c, d) << std::endl;
	}

	printHeader("Tie behavior returns the second argument");
	{
		int x = 42, y = 42;
		std::cout << "min(x, y) = " << ::min(x, y) << " (should be y=42)\n";
		std::cout << "max(x, y) = " << ::max(x, y) << " (should be y=42)\n";
	}
	return 0;
}
