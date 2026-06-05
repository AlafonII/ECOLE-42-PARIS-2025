/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 19:42:01 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Fixed.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: Fixed.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the Fixed class for fixed-point arithmetic. Provides
 *   constructors (default, copy, int, float), copy assignment,
 *   raw bit accessors, conversions to int/float, stream output,
 *   and destructor logging.
 * ---------------------------------------------------------------
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

// Int constructor: convert integer to fixed-point format
Fixed::Fixed(const int n) : _rawBits(n << _fracBits) {
	std::cout << "Int constructor called" << std::endl;
}

// Float constructor: convert float to fixed-point format
Fixed::Fixed(const float f) : _rawBits(roundf(f * (1 << _fracBits))) {
	std::cout << "Float constructor called" << std::endl;
}


/*
* -----------------------------------------------------------
* 						OPERATOR
* -----------------------------------------------------------
*/

// Copy assignment: guard against self-assignment then copy raw bits
Fixed &Fixed::operator=(const Fixed &src) {
	if (this != &src)					// avoid self-copy
	this->_rawBits = src._rawBits;	// copy raw value
    std::cout << "Copy assignment operator called" << std::endl;
    return *this;						// return self-reference
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTION
 * -----------------------------------------------------------
 */

// setRawBits: update internal fixed-point representation
void	Fixed::setRawBits(int const raw) {
	std::cout << "setRawBits called" << std::endl;
	this->_rawBits = raw;		// assign new raw bits
}

// getRawBits: retrieve internal fixed-point representation
int	Fixed::getRawBits(void) const {
	std::cout << "getRawBits called" << std::endl;
	return this->_rawBits;		// return current raw bits
}

// toInt: convert fixed-point to integer
int	Fixed::toInt(void) const {
	return (this-> _rawBits >> Fixed::_fracBits); // shift down by fractional bits
}

// toFloat: convert fixed-point to floating-point
float	Fixed::toFloat(void) const {
	return ((float)this->_rawBits / (1 << _fracBits)); // divide by scale
}

// Stream insertion: output fixed-point value as float
std::ostream	&operator<<(std::ostream &os, Fixed const &number) {
	return (os << number.toFloat()); // insert float representation
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