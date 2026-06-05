/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 17:43:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Animal.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Weird thing", use default stats
Animal::Animal(void) : _type("Weird thing") {
		std::cout << "Animal default constructor called" << std::endl;
}

// type: custom type, default stats
Animal::Animal(std::string const &type) : _type(type) {
	std::cout << "Animal type constructor has been called" << std::endl;
}

// Copy: use assignment operator to clone all fields
Animal::Animal(Animal const &src) {
	*this = src;
	std::cout << "Animal copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
Animal &	Animal::operator=(Animal const &src) {
	std::cout << "Animal assignment operator called" << std::endl;
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

std::string	const	&Animal::getType(void) const {
	return (this->_type);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	Animal::makeSound(void) const {
	std::cout << this->_type << " said : I̸̡̢̹̥̖̓̈́̊̉͊̈̂͌͑̅̇̂͝ͅ ̷̞̬͕͇̠͔̰̗͖̣̝̙͗̄̔͆͑̔̂̈́̈́̄̈́̅̃͝ͅẀ̴̲Į̴̦̘̼̭̏̅͛̒̑̆͂͝L̷̨̛̛͈͖͕̠̥͛͌̄͋̈͂̂͋͛L̷̢̡̜̥̬̗̘̦̑̾͂́́̃̕ ̶̬̳̯͕̈́̈͆͌͑̑͐͐̎̇͛̚͘̕K̷̡̛̮̦̣̫͇̙͓͈̲̍̀͒̾͑̾̐̔͑̃͛̽̚͝I̴̙͔͈̠͇̖̫̳̤̹͉͐̓̅̈́͒̒̅͜͠L̸̯̗͍͒̄̈́̉̅̆̔̀͒̚̚͝L̷̩͓̘̗̰̰͍͈̪̺͛̿̓͠ ̸̦̼̼͆͂̊ͅY̷̦̎̂̂̌́̍͂̓̀̉̂̕͠Ọ̴̧̧̥̖̤̖̬̦̼̭̤͛͆͑͆́͗̕Ȗ̸͇͓͓̟̭͇̞̤̙̙̖̲̪́́̓̅̊̃͝ͅ ̵̧̹̯̘̤͕̺̤͖͎͙̎͂̃̌̑̊͋͐͒̓͛̓̎̄I̶̟͙̦̦͔̗̱͚̘̗̺̼͊̾̉͠ͅǸ̷̘̝͍̞̫̟͚̠̤͇̜̠̜̈̀̀̊́̃͆̑̀̽̚͝͠ͅ ̵̧̡̡͔̗̟̠̳͎̺̃̊͆̌͌́̌̐͌̕̚Ỵ̵̣̿̓̊͑͌̄̊͝͝Ǒ̵̡̭͉̝̲̖͈̙̿̊͋͌̒́̓̓Û̷̡͕̲̥͕̹̩̦̦͔̹̻̀̊̎Ř̴̨̢̺̳̠̟̼̞̜̬̇ ̷̩̞̘̺̰̬̈́̈̃̂̽͆̑̃͑̾̐͛S̷͖͇͖̱̟̬̞̥̙̖͉̫̩̎͝L̷̨̢̡̦̯͙͇͉̬̏͆̈́̐͗͊̕͜ͅE̸̛̻̩̅̌͂̐̎͝͠Ȇ̶̛̤̒̊̐͊̔͋̽̑̑P̴̨̝̤̜̜͚̭̱̲̘̺͉͖̌" << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

Animal::~Animal(void) {
	std::cout << "Animal destructor called"<< std::endl;
}
