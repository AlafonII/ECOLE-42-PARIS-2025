/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:26:12 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 17:52:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dog.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Dog", use default stats
Dog::Dog(void) : Animal() {
	this->_type = "Dog";
	std::cout << "Dog default constructor called" << std::endl;
}

// Copy: use assignment operator to clone all fields
Dog::Dog(Dog const & src) : Animal()  {
	*this = src;
	std::cout << "Dog copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
Dog	&Dog::operator=(Dog const & src) {
	std::cout << "Dog assignment operator called" << std::endl;
	if (this != &src)
		this->_type = src._type;
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	Dog::makeSound(void) const {
	std::cout << this->_type << " said : BARK BARK BARK" << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

Dog::~Dog(void) {
	std::cout << "Dog destructor called"<< std::endl;
}
