/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 19:42:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Fixed.hpp"

/*
 * Filename: Fixed.cpp
 * -------------------
 * Implements Fixed: a simple fixed-point wrapper with raw bit storage,
 * copy semantics, and accessors.
 */

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default constructor: initialize raw bits to 0
Fixed::Fixed(void) : _rawBits(0) {
	std::cout << "Default constructor called" << std::endl;
}

// Copy constructor: clone raw bits from another Fixed
Fixed::Fixed(const Fixed &obj) : _rawBits(obj.getRawBits()) {
	std::cout << "Copy constructor called" << std::endl;
}

/*
* -----------------------------------------------------------
* 						OPERATOR
* -----------------------------------------------------------
*/

// Copy assignment: guard against self-assignment, then copy raw bits
Fixed &Fixed::operator=(const Fixed &src) {
	if (this != &src)					 // avoid self-copy
	this->_rawBits = src._rawBits;	 // copy raw value
	std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTION
 * -----------------------------------------------------------
 */

// setRawBits: update internal fixed-point representation
void Fixed::setRawBits(int const raw) {
	std::cout << "setRawBits called" << std::endl;
	this->_rawBits = raw;		// assign new raw bits
}

// getRawBits: retrieve internal fixed-point representation
int Fixed::getRawBits(void) const {
	std::cout << "getRawBits called" << std::endl;
	return this->_rawBits;		// return current raw bits
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

// Destructor: announce when object is destroyed
Fixed::~Fixed(void) {
	std::cout << "Default destructor called" << std::endl;
}
