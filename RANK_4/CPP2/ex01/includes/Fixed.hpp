/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 00:01:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <string>
#include <iostream>
#include <cmath>

class Fixed {
private:
	int _rawBits;							// raw fixed-point storage
	static const int _fracBits = 8;			// number of fractional bits

public:
	Fixed(void);							// default constructor, raw bits = 0
	Fixed(const Fixed &obj);				// copy constructor, clone raw bits
	Fixed(const int n);						// construtor to covert int to fixed-point
	Fixed(const float f);					// construtor to covert float to fixed-point
	~Fixed(void);							// destructor

	Fixed	&operator=(const Fixed &src);	// copy assignment, copy raw bits

	int		getRawBits(void) const;			// return internal raw bits
	void	setRawBits(int const raw);		// set internal raw bits
	float	toFloat(void) const;			// converts to float
	int		toInt(void) const;				// converts to int
};

// Stream insertion: output fixed-point value as float
std::ostream& operator<<( std::ostream& os, const Fixed& number );

#endif