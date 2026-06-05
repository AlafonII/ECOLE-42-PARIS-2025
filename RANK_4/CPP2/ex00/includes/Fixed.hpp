/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/24 19:31:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <string>
#include <iostream>

class Fixed {
private:
	int _rawBits;							// raw fixed-point storage
	static const int _fracBits = 8;			// number of fractional bits

public:
	Fixed(void);							// default constructor, raw bits = 0
	Fixed(const Fixed &obj);				// copy constructor, clone raw bits
	~Fixed(void);							// destructor

	Fixed	&operator=(const Fixed &src);	// copy assignment, copy raw bits

	int		getRawBits(void) const;			// return internal raw bits
	void	setRawBits(int const raw);		// set internal raw bits
};

#endif