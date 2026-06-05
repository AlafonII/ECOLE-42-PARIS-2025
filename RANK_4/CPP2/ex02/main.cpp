/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 10:21:15 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Fixed.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Demonstrates Fixed operations: default init, prefix/postfix
 *   increments, and using max. Prints results via stream insertion.
 * ---------------------------------------------------------------
 */

int main(void) {
	Fixed a;												// default construct a (rawBits=0)
	Fixed const b( Fixed(5.05f) * Fixed(2) );				// multiply float 5.05 by int 2, init b
	std::cout << "Normal a	: " << a << std::endl;			// print 0.000000 (toFloat)
	std::cout << "Prefix ++a	: " << ++a << std::endl;	// prefix ++: increment then print
	std::cout << "A now		: " << a << std::endl;			// print updated a
	std::cout << "Prefix a++	: " << a++ << std::endl;	// postfix ++: print then increment
	std::cout << "A now		: " << a << std::endl;   		// print incremented a
	std::cout << "B now		: " << b << std::endl;   		// print b result
	std::cout << "Max		: " << Fixed::max(a, b) << std::endl;	// print larger of a and b

	return (0);
}