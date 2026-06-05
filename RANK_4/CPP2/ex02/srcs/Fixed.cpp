/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 23:13:52 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Fixed.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: Fixed.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements Fixed class: fixed-point storage and arithmetic.
 *   Provides constructors, operators, conversions, min/max, and logging.
 * ---------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default constructor: initialize raw bits to 0
Fixed::Fixed(void) : _rawBits(0) {
	// std::cout << "Default constructor called" << std::endl;
}

// Copy constructor: clone raw bits from another Fixed
Fixed::Fixed(const Fixed &obj) : _rawBits(obj.getRawBits()) {
	// std::cout << "Copy constructor called" << std::endl;
}

// Int constructor: convert integer to fixed-point format
Fixed::Fixed(const int n) : _rawBits(n << _fracBits) {
	// std::cout << "Int constructor called" << std::endl;
}

// Float constructor: convert float to fixed-point format
Fixed::Fixed(const float f) : _rawBits(roundf(f * (1 << _fracBits))) {
	// std::cout << "Float constructor called" << std::endl;
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

// Greater-than: true if this raw value exceeds rhs
bool Fixed::operator>(Fixed const &rhs) const
{
	return this->_rawBits > rhs.getRawBits();
}

// Less-than: true if this raw value is less than rhs
bool Fixed::operator<(Fixed const &rhs) const
{
	return this->_rawBits < rhs.getRawBits();
}

// Greater-equal: true if this raw value >= rhs
bool Fixed::operator>=(Fixed const &rhs) const
{
	return this->_rawBits >= rhs.getRawBits();
}

// Less-equal: true if this raw value <= rhs
bool Fixed::operator<=(Fixed const &rhs) const
{
	return this->_rawBits <= rhs.getRawBits();
}

// Equality: true if raw values match
bool Fixed::operator==(Fixed const &rhs) const
{
	return this->_rawBits == rhs.getRawBits();
}

// Inequality: true if raw values differ
bool Fixed::operator!=(Fixed const &rhs) const
{
	return this->_rawBits != rhs.getRawBits();
}

// Addition: convert to float, add, then back to fixed
Fixed Fixed::operator+(Fixed const &rhs) const
{
	return Fixed(this->toFloat() + rhs.toFloat());
}

// Subtraction: convert to float, subtract, then back to fixed
Fixed Fixed::operator-(Fixed const &rhs) const
{
	return Fixed(this->toFloat() - rhs.toFloat());
}

// Multiplication: convert to float, multiply, then back to fixed
Fixed Fixed::operator*(Fixed const &rhs) const
{
	return Fixed(this->toFloat() * rhs.toFloat());
}

// Division: convert to float, divide, then back to fixed
Fixed Fixed::operator/(Fixed const &rhs) const
{
	return Fixed(this->toFloat() / rhs.toFloat());
}

// Prefix increment: increase raw bits by 1 (smallest unit)
Fixed &Fixed::operator++(void)
{
	this->_rawBits++;	// step raw value up
	return *this;		// return updated object
}

// Postfix increment: return old value, then increment
Fixed Fixed::operator++(int)
{
	Fixed	tmp(*this);	// copy current state
	++*this;			// increment this
	return tmp;			// return previous state
}

// Prefix decrement: decrement raw bits by 1 (smallest unit)
Fixed &Fixed::operator--(void)
{
	this->_rawBits--;	// step raw value down
	return *this;		// return updated object
}

// Postfix decrement: return old value, then decrement
Fixed Fixed::operator--(int)
{
	Fixed	tmp(*this);	// copy current state
	--*this;			// decrement this
	return tmp;			// return previous state
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTION
 * -----------------------------------------------------------
 */

// setRawBits: update internal fixed-point representation
void	Fixed::setRawBits(int const raw) {
	// std::cout << "setRawBits called" << std::endl;
	this->_rawBits = raw;		// assign new raw bits
}

// getRawBits: retrieve internal fixed-point representation
int	Fixed::getRawBits(void) const {
	// std::cout << "getRawBits called" << std::endl;
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

// Return the smaller of two mutable Fixed instances
Fixed &Fixed::min(Fixed &lhs, Fixed &rhs)
{
	if (lhs <= rhs)		// if lhs is less than or equal to rhs
		return lhs;		// lhs is the minimum
	return rhs;			// otherwise rhs is the minimum
}

// Return the smaller of two const Fixed instances
const Fixed &Fixed::min(const Fixed &lhs, const Fixed &rhs)
{
	if (lhs <= rhs)		// compare lhs and rhs by raw bits
		return lhs;		// lhs is the minimum
	return rhs;			// otherwise rhs is the minimum
}

// Return the larger of two mutable Fixed instances
Fixed &Fixed::max(Fixed &lhs, Fixed &rhs)
{
	if (lhs >= rhs)		// if lhs is greater than or equal to rhs
		return lhs;		// lhs is the maximum
	return rhs;			// otherwise rhs is the maximum
}

// Return the larger of two const Fixed instances
const Fixed &Fixed::max(const Fixed &lhs, const Fixed &rhs)
{
	if (lhs >= rhs)		// compare lhs and rhs by raw bits
		return lhs;		// lhs is the maximum
	return rhs;			// otherwise rhs is the maximum
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
	// std::cout << "Default destructor called" << std::endl;
}
