/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:42:00 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 17:52:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WrongCat.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Cat", use default stats
WrongCat::WrongCat(void) : WrongAnimal() {
	this->_type = "WrongCat";
	std::cout << "WrongCat default constructor called" << std::endl;
}

// Copy: use assignment operator to clone all fields
WrongCat::WrongCat(WrongCat const & src) : WrongAnimal()  {
	*this = src;
	std::cout << "WrongCat copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
WrongCat	&WrongCat::operator=(WrongCat const & src) {
	std::cout << "WrongCat assignment operator called" << std::endl;
	if (this != &src)
		this->_type = src._type;
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	WrongCat::makeSound(void) const {
	std::cout << this->_type << " said : wwwomemememeM" << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

WrongCat::~WrongCat(void) {
	std::cout << "WrongCat destructor called"<< std::endl;
}
