/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:11:26 by alex              #+#    #+#             */
/*   Updated: 2025/09/25 09:38:37 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "incl/Span.hpp"

static void printHeader(const std::string& title) {
	std::cout << "\n\033[32m=== " << title << " ===\033[0m\n";
}

static void printTable(const std::vector<int>& v, std::size_t maxPreview) {
	std::cout << "values [" << v.size() << "]: ";
	if (v.empty()) {
		std::cout << "[]\n";
		return;
	}
	std::cout << "[ ";
	std::size_t shown = v.size();
	if (shown > maxPreview) {
		shown = maxPreview;
	}
	for (std::size_t i = 0; i < shown; ++i) {
		std::cout << v[i];
		if (i + 1 < shown) {
			std::cout << ", ";
		}
	}
	if (v.size() > maxPreview) {
		std::cout << ", ...";
	}
	std::cout << " ]\n";
}

static void printStats(const Span& sp) {
	std::cout << "size=" << sp.size()
	          << " | shortest=" << sp.shortestSpan()
	          << " | longest=" << sp.longestSpan() << "\n";
}

int main() {
	printHeader("Subject-like basic test (explicit numbers)");
	{
		Span sp(5);
		std::vector<int> src;
		src.push_back(6);
		src.push_back(3);
		src.push_back(17);
		src.push_back(9);
		src.push_back(11);

		printTable(src, 20);
		for (std::size_t i = 0; i < src.size(); ++i) {
			sp.addNumber(src[i]);
		}

		printStats(sp); // expect shortest=2, longest=14
	}

	printHeader("Range insert with iterators (0,3,6,...,27)");
	{
		std::vector<int> src;
		for (int i = 0; i < 10; ++i) {
			src.push_back(i * 3);
		}
		printTable(src, 20);

		Span sp(static_cast<unsigned int>(src.size()));
		sp.addRange(src.begin(), src.end());

		printStats(sp);
	}

	printHeader("10,000 random numbers (preview table, then stats)");
	{
		const unsigned int N = 10000;
		Span sp(N);

		std::srand(static_cast<unsigned int>(std::time(0)));
		std::vector<int> big;
		big.reserve(N);
		for (unsigned int i = 0; i < N; ++i) {
			int r = std::rand();
			big.push_back(r);
		}
		printTable(big, 30);

		sp.addRange(big.begin(), big.end());
		printStats(sp);
	}

	printHeader("Exceptions: over-capacity and too-few elements");
	{
		std::cout << "[Over-capacity]\n";
		try {
			Span sp(1);
			sp.addNumber(42);
			sp.addNumber(21); // should throw
			std::cout << "unexpected (no exception)\n";
		} catch (std::exception const& e) {
			std::cout << "caught: " << e.what() << "\n";
		}

		std::cout << "[Too few elements for span]\n";
		try {
			Span sp(2);
			sp.addNumber(1);
			std::cout << sp.shortestSpan() << "\n"; // should throw
			std::cout << "unexpected (no exception)\n";
		} catch (std::exception const& e) {
			std::cout << "caught: " << e.what() << "\n";
		}
	}

	return 0;
}
