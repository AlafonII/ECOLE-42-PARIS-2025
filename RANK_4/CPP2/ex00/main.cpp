/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 18:24:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Fixed.hpp"
#include <iostream>

/*
 * Filename: main.cpp
 * ------------------
 * Demonstrates Fixed’s constructors, assignment, and raw bit access.
 */

int	main(void)
{
	Fixed	a;		// default constructor → _rawBits = 0
	Fixed	b(a);	// copy constructor   → b._rawBits = a._rawBits
	Fixed	c;		// default constructor → _rawBits = 0

	c = b;			// copy assignment     → c._rawBits = b._rawBits

	// Print raw bit values for each Fixed instance
	std::cout << a.getRawBits() << std::endl; // calls getRawBits()
	std::cout << b.getRawBits() << std::endl; // calls getRawBits()
	std::cout << c.getRawBits() << std::endl; // calls getRawBits()

	return (0);
}
