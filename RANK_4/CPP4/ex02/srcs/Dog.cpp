/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:26:12 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 19:39:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dog.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Dog", use default stats
Dog::Dog(void) : AAnimal(), _brain(new Brain()) {
	this->_type = "Dog";
	std::cout << "Dog default constructor called" << std::endl;
}

// Copy: use assignment operator to clone all fields
Dog::Dog(Dog const & src) : AAnimal(), _brain(NULL)  {
	*this = src;
	std::cout << "Dog copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
Dog	&Dog::operator=(Dog const & src) {
	std::cout << "Dog assignment operator called" << std::endl;
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

Brain	*Dog::getBrain(void) const {
	return this->_brain;
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
	delete this->_brain;
}
