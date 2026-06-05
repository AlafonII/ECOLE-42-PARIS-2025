/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:26:42 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 19:29:47 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cat.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Cat", use default stats
Cat::Cat(void) : AAnimal() , _brain(new Brain()) {
	this->_type = "Cat";
	std::cout << "Cat default constructor called" << std::endl;
}

// Copy: use assignment operator to clone all fields
Cat::Cat(Cat const & src) : AAnimal(), _brain(NULL) {
	*this = src;
	std::cout << "Cat copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
Cat	&Cat::operator=(Cat const & src) {
	std::cout << "Cat assignment operator called" << std::endl;
	if (this != &src)
	{
		this->_type = src._type;
		if (this->_brain != NULL)
			delete this->_brain;
		this->_brain = new Brain(*src._brain);
	}
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						GETTERS
 * -----------------------------------------------------------
 */

Brain	*Cat::getBrain(void) const {
	return this->_brain;
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	Cat::makeSound(void) const {
	std::cout << this->_type << " said : Mememememowww" << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

Cat::~Cat(void) {
	std::cout << "Cat destructor called"<< std::endl;
	delete this->_brain;
}
