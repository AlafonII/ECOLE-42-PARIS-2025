/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:41:52 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 17:43:59 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WrongAnimal.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Weird thing", use default stats
WrongAnimal::WrongAnimal(void) : _type("*NOT* Weird thing") {
		std::cout << "WrongAnimal default constructor called" << std::endl;
}

// type: custom type, default stats
WrongAnimal::WrongAnimal(std::string const &type) : _type(type) {
	std::cout << "WrongAnimal type constructor has been called" << std::endl;
}

// Copy: use assignment operator to clone all fields
WrongAnimal::WrongAnimal(WrongAnimal const &src) {
	*this = src;
	std::cout << "WrongAnimal copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
WrongAnimal &	WrongAnimal::operator=(WrongAnimal const &src) {
	std::cout << "WrongAnimal assignment operator called" << std::endl;
	if (this != &src) {
		this->_type = src._type;
	}
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						GETTERS
 * -----------------------------------------------------------
 */

std::string	const	&WrongAnimal::getType(void) const {
	return (this->_type);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	WrongAnimal::makeSound(void) const {
	std::cout << this->_type << " said : I̶̤̜̤͑͊ ̴͈̂̇͠Ľ̴̛͉̥̳̂Ŏ̴̱̳̊̕V̴͔̐̐͜É̸̺̏ ̸̳̕Y̸̡̳̮̿Ȯ̷̖̖̦Ų̶̯̪̑́" << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

WrongAnimal::~WrongAnimal(void) {
	std::cout << "WrongAnimal destructor called"<< std::endl;
}
