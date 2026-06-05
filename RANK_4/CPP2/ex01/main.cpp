/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 19:14:02 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Fixed.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Demonstrates Fixed class construction, assignment, and conversion.
 *   Shows default, int, float, and copy construction, as well as
 *   streaming and integer conversion.
 * ---------------------------------------------------------------
 */

// Entry point: exercises Fixed constructors, assignment, and output
int main(void) {
	Fixed a;						// default constructor → rawBits = 0
	Fixed const b(10);				// int constructor → rawBits = 10 << fracBits
	Fixed const c(42.42f);			// float constructor → roundf(42.42 * 256)
	Fixed const d(b);				// copy constructor → clone rawBits from b
	a = Fixed(1234.4321f);			// assign a new Fixed via float constructor then copy

	std::cout << "a is " << a << std::endl;	// uses operator<< to float
	std::cout << "b is " << b << std::endl;
	std::cout << "c is " << c << std::endl;
	std::cout << "d is " << d << std::endl;

	std::cout << "a is " << a.toInt() << " as integer" << std::endl;	// toInt shifts rawBits
	std::cout << "b is " << b.toInt() << " as integer" << std::endl;
	std::cout << "c is " << c.toInt() << " as integer" << std::endl;
	std::cout << "d is " << d.toInt() << " as integer" << std::endl;

	return (0);
}
