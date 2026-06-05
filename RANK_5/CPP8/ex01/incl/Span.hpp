/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/09/25 09:36:27 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>

class Span {
public:
	explicit Span(unsigned int n);

	void addNumber(int value);

	template <typename InputIt>
	void addRange(InputIt first, InputIt last) {
		std::size_t dist = 0;
		InputIt it = first;
		while (it != last) {
			++dist;
			++it;
		}
		if (_data.size() + dist > _capacity) {
			throw std::overflow_error("Span: adding range would exceed capacity");
		}
		_data.insert(_data.end(), first, last);
	}

	unsigned int shortestSpan() const;
	unsigned int longestSpan() const;

	unsigned int size() const;

private:
	std::vector<int> _data;
	unsigned int _capacity;
};

#endif
