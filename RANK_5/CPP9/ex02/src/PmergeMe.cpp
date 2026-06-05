/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:20:53 by alafon            #+#    #+#             */
/*   Updated: 2025/10/17 11:25:24 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/PmergeMe.hpp"
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>

// ---------------------------- Constructor / Destructor ----------------------------
PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &) {}
PmergeMe &PmergeMe::operator=(const PmergeMe &) { return *this; }
PmergeMe::~PmergeMe() {}

// ---------------------------- Parsing ----------------------------
void PmergeMe::parseInput(int argc, char **argv, std::vector<int> &v, std::list<int> &l) {
	for (int i = 1; i < argc; ++i) {
		char *end;
		long num = std::strtol(argv[i], &end, 10);
		if (*end || num < 0 || num > 2147483647)
			throw std::runtime_error("Error");
		v.push_back(static_cast<int>(num));
		l.push_back(static_cast<int>(num));
	}
}

// ---------------------------- Display ----------------------------
void PmergeMe::display(const std::string &label, const std::vector<int> &v) {
	std::cout << label << ": ";
	for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

// ---------------------------- Jacobsthal Sequence ----------------------------
std::vector<size_t> PmergeMe::jacobsthalSequence(size_t size) {
	std::vector<size_t> seq;

	if (size == 0)
		return seq;

	// Step 1: Generate Jacobsthal numbers up to the size of the pending array
	std::vector<size_t> jac;
	jac.push_back(1); // J(1)
	jac.push_back(3); // J(2)
	while (jac.back() < size)
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);
	// Example for size = 10 → jac = [1, 3, 5, 11]

	// Step 2: Build the insertion order sequence from the Jacobsthal numbers
	std::vector<bool> added(size, false);

	// process Jacobsthal numbers in reverse order
	// For each J(k) insert indices going backward from J(k) down to the
	// previous Jacobsthal number, only those that haven’t been added yet
	for (int j = (int)jac.size() - 1; j >= 0; --j) {
		for (size_t i = jac[j]; i > 0; --i) {
			if (i - 1 < size && !added[i - 1]) {
				seq.push_back(i - 1);
				added[i - 1] = true;
			}
		}
	}

	// Step 3: Add any remaining indices that weren’t covered by Jacobsthal jumps
	for (size_t i = 0; i < size; ++i)
		if (!added[i])
			seq.push_back(i);

	return seq;
}

// ---------------------------- Insert Helpers ----------------------------
void PmergeMe::insertSorted(std::vector<int> &v, int value) {
	std::vector<int>::iterator it = std::lower_bound(v.begin(), v.end(), value);
	v.insert(it, value);
}

void PmergeMe::insertSorted(std::list<int> &l, int value) {
	std::list<int>::iterator it = l.begin();
	while (it != l.end() && *it < value)
		++it;
	l.insert(it, value);
}

// ---------------------------- Ford–Johnson (Vector) ----------------------------
void PmergeMe::fordJohnson(std::vector<int> &v) {
	if (v.size() <= 2) {
		if (v.size() == 2 && v[0] > v[1])
			std::swap(v[0], v[1]);
		return;
	}

	// Step 1: Pair elements
	std::vector<std::pair<int, int> > pairs;
	int pending = -1;
	for (size_t i = 0; i + 1 < v.size(); i += 2) {
		int a = v[i];
		int b = v[i + 1];
		if (a > b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	if (v.size() % 2)
		pending = v.back();

	// Step 2: Build main chain: all a(i) + b(1)
	std::vector<int> main;
	for (size_t i = 0; i < pairs.size(); ++i)
		main.push_back(pairs[i].first);
	if (!pairs.empty())
		main.insert(main.begin(), pairs[0].second); // add first b at front

	// Step 3: Recursively sort the main chain
	if (main.size() > 2)
		fordJohnson(main);

	// Step 4: Insert remaining b(2)...b(n) using Jacobsthal order
	std::vector<int> pend;
	for (size_t i = 1; i < pairs.size(); ++i)
		pend.push_back(pairs[i].second);
	if (pending != -1)
		pend.push_back(pending);

	std::vector<size_t> order = jacobsthalSequence(pend.size());
	for (size_t i = 0; i < order.size(); ++i)
		insertSorted(main, pend[order[i]]);

	// Step 5: Replace input with sorted result
	v.swap(main);
}

// ---------------------------- Ford–Johnson (List) ----------------------------
void PmergeMe::fordJohnson(std::list<int> &l) {
	if (l.size() <= 2) {
		if (l.size() == 2) {
			std::list<int>::iterator it = l.begin();
			std::list<int>::iterator jt = it; ++jt;
			if (*jt < *it)
				std::iter_swap(it, jt);
		}
		return;
	}

	// Step 1: Pair elements
	std::list<std::pair<int, int> > pairs;
	std::list<int> pending;
	std::list<int>::iterator it = l.begin();

	while (it != l.end()) {
		int a = *it++;
		if (it == l.end()) {
			pending.push_back(a);
			break;
		}
		int b = *it++;
		if (a > b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}

	// Step 2: Build main chain: all a(i) + b(1)
	std::list<int> main;
	for (std::list<std::pair<int, int> >::iterator p = pairs.begin(); p != pairs.end(); ++p)
		main.push_back(p->first);
	if (!pairs.empty())
		main.push_front(pairs.begin()->second);

	// Step 3: Recursively sort the main chain
	if (main.size() > 2)
		fordJohnson(main);

	// Step 4: Collect pend (b(2)..b(n) + odd)
	std::vector<int> pend;
	if (!pairs.empty()) {
		std::list<std::pair<int, int> >::iterator p = pairs.begin();
		++p; // skip first
		for (; p != pairs.end(); ++p)
			pend.push_back(p->second);
	}
	if (!pending.empty())
		pend.push_back(pending.front());

	// Insert in Jacobsthal order
	std::vector<size_t> order = jacobsthalSequence(pend.size());
	for (size_t i = 0; i < order.size(); ++i)
		insertSorted(main, pend[order[i]]);

	// Step 5: Replace list
	l.swap(main);
}

// ---------------------------- Timed Runs ----------------------------
void PmergeMe::runVector(std::vector<int> v) {
	clock_t start = clock();
	fordJohnson(v);
	double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC * 1e6;
	display("After", v);
	std::cout << "Time to process a range of " << v.size()
			  << " elements with std::vector : "
			  << std::fixed << std::setprecision(5)
			  << elapsed << " us" << std::endl;
}

void PmergeMe::runList(std::list<int> l) {
	clock_t start = clock();
	fordJohnson(l);
	double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC * 1e6;
	std::cout << "Time to process a range of " << l.size()
			  << " elements with std::list : "
			  << std::fixed << std::setprecision(5)
			  << elapsed << " us" << std::endl;
}

// ---------------------------- Unified Run ----------------------------
void PmergeMe::run(int argc, char **argv) {
	std::vector<int> v;
	std::list<int> l;

	parseInput(argc, argv, v, l);

	display("Before", v);

	runVector(v);
	runList(l);
}