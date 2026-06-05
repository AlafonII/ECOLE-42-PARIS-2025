/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:38:35 by alex              #+#    #+#             */
/*   Updated: 2025/09/25 09:37:59 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Span.hpp"
#include <algorithm>
#include <limits>

Span::Span(unsigned int n) : _data(), _capacity(n) {}

void Span::addNumber(int value) {
	if (_data.size() >= _capacity) {
		throw std::overflow_error("Span: capacity reached");
	}
	_data.push_back(value);
}

unsigned int Span::size() const {
	return static_cast<unsigned int>(_data.size());
}

unsigned int Span::shortestSpan() const {
	if (_data.size() < 2) {
		throw std::logic_error("Span: need at least two numbers");
	}
	std::vector<int> sorted = _data;
	std::sort(sorted.begin(), sorted.end());
	unsigned int best = static_cast<unsigned int>(-1);
	for (std::size_t i = 1; i < sorted.size(); ++i) {
		long diff = static_cast<long>(sorted[i]) - static_cast<long>(sorted[i - 1]);
		if (diff < 0)
			diff = -diff;
		unsigned int udiff = static_cast<unsigned int>(diff);
		if (udiff < best)
			best = udiff;
	}
	return best;
}

unsigned int Span::longestSpan() const {
	if (_data.size() < 2) {
		throw std::logic_error("Span: need at least two numbers");
	}
	std::vector<int>::const_iterator minIt = std::min_element(_data.begin(), _data.end());
	std::vector<int>::const_iterator maxIt = std::max_element(_data.begin(), _data.end());
	long diff = static_cast<long>(*maxIt) - static_cast<long>(*minIt);
	if (diff < 0)
		diff = -diff;
	return static_cast<unsigned int>(diff);
}
