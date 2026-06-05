/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 20:04:41 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <string>
#include <iostream>
#include <cmath>

class Fixed {
private:
	int _rawBits;													// raw fixed-point storage
	static const int _fracBits = 8;									// number of fractional bits

public:
	Fixed(void);													// default constructor, raw bits = 0
	Fixed(const Fixed &obj);										// copy constructor, clone raw bits
	Fixed(const int n);
	Fixed(const float f);
	~Fixed(void);													// destructor

	Fixed				&operator=(const Fixed &src);				// copy assignment, copy raw bits
	Fixed				&operator++(void);							// prefix ++, increment rawBits by 1
	Fixed				operator++(int);							// postfix ++, increment rawBits, return old
	Fixed				&operator--(void);							// prefix --, decrement rawBits by 1
	Fixed				operator--(int);							// postfix --, decrement rawBits, return old

	bool				operator>(const Fixed &rhs) const;			// greater-than comparison
	bool				operator<(const Fixed &rhs) const;			// less-than comparison
	bool				operator>=(const Fixed &rhs) const;			// greater-equal comparison
	bool				operator<=(const Fixed &rhs) const;			// less-equal comparison
	bool				operator==(const Fixed &rhs) const;			// equality comparison
	bool				operator!=(const Fixed &rhs) const;			// inequality comparison

	Fixed				operator+(const Fixed &rhs) const;			// add fixed-point values
	Fixed				operator-(const Fixed &rhs) const;			// subtract fixed-point values
	Fixed				operator*(const Fixed &rhs) const;			// multiply fixed-point values
	Fixed				operator/(const Fixed &rhs) const;			// divide fixed-point values

	
	int					getRawBits(void) const;						// return internal raw bits
	void				setRawBits(int const raw);					// set internal raw bits
	float 				toFloat(void) const;						// converts to float
	int 				toInt(void) const;							// converts to int

	static Fixed		&min(Fixed &lhs, Fixed &rhs);				// return smaller
	static Fixed		&max(Fixed &lhs, Fixed &rhs);				// return larger
	static const Fixed	&min(const Fixed &lhs, const Fixed &rhs);	// return smaller (const)
	static const Fixed	&max(const Fixed &lhs, const Fixed &rhs);	// return larger (const)
};

// Stream insertion: output fixed-point value as float
std::ostream& operator<<( std::ostream& os, const Fixed& number );

#endif