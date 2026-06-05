/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAnimal.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 19:46:20 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AAnimal.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: type="Weird thing", use default stats
AAnimal::AAnimal(void) : _type("WWeird tthing") {
		std::cout << "AAnimal default constructor called" << std::endl;
}	

// Copy: use assignment operator to clone all fields
AAnimal::AAnimal(AAnimal const &src) {
	*this = src;
	std::cout << "AAnimal copy constructor called" << std::endl;
}

// Assignment: clone fields if not self
AAnimal &	AAnimal::operator=(AAnimal const &src) {
	std::cout << "AAnimal assignment operator called" << std::endl;
	if (this != &src)
		this->_type = src._type;
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						GETTERS
 * -----------------------------------------------------------
 */

std::string	const	&AAnimal::getType(void) const {
	return (this->_type);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

// void	AAnimal::makeSound(void) const {
// 	std::cout << this->_type << " said : I̸̡̢̹̥̖̓̈́̊̉͊̈̂͌͑̅̇̂͝ͅ ̷̞̬͕͇̠͔̰̗͖̣̝̙͗̄̔͆͑̔̂̈́̈́̄̈́̅̃͝ͅẀ̴̲Į̴̦̘̼̭̏̅͛̒̑̆͂͝L̷̨̛̛͈͖͕̠̥͛͌̄͋̈͂̂͋͛L̷̢̡̜̥̬̗̘̦̑̾͂́́̃̕ ̶̬̳̯͕̈́̈͆͌͑̑͐͐̎̇͛̚͘̕K̷̡̛̮̦̣̫͇̙͓͈̲̍̀͒̾͑̾̐̔͑̃͛̽̚͝I̴̙͔͈̠͇̖̫̳̤̹͉͐̓̅̈́͒̒̅͜͠L̸̯̗͍͒̄̈́̉̅̆̔̀͒̚̚͝L̷̩͓̘̗̰̰͍͈̪̺͛̿̓͠ ̸̦̼̼͆͂̊ͅY̷̦̎̂̂̌́̍͂̓̀̉̂̕͠Ọ̴̧̧̥̖̤̖̬̦̼̭̤͛͆͑͆́͗̕Ȗ̸͇͓͓̟̭͇̞̤̙̙̖̲̪́́̓̅̊̃͝ͅ ̵̧̹̯̘̤͕̺̤͖͎͙̎͂̃̌̑̊͋͐͒̓͛̓̎̄I̶̟͙̦̦͔̗̱͚̘̗̺̼͊̾̉͠ͅǸ̷̘̝͍̞̫̟͚̠̤͇̜̠̜̈̀̀̊́̃͆̑̀̽̚͝͠ͅ ̵̧̡̡͔̗̟̠̳͎̺̃̊͆̌͌́̌̐͌̕̚Ỵ̵̣̿̓̊͑͌̄̊͝͝Ǒ̵̡̭͉̝̲̖͈̙̿̊͋͌̒́̓̓Û̷̡͕̲̥͕̹̩̦̦͔̹̻̀̊̎Ř̴̨̢̺̳̠̟̼̞̜̬̇ ̷̩̞̘̺̰̬̈́̈̃̂̽͆̑̃͑̾̐͛S̷͖͇͖̱̟̬̞̥̙̖͉̫̩̎͝L̷̨̢̡̦̯͙͇͉̬̏͆̈́̐͗͊̕͜ͅE̸̛̻̩̅̌͂̐̎͝͠Ȇ̶̛̤̒̊̐͊̔͋̽̑̑P̴̨̝̤̜̜͚̭̱̲̘̺͉͖̌" << std::endl;
// }
 
/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

AAnimal::~AAnimal(void) {
	std::cout << "AAnimal destructor called"<< std::endl;
}