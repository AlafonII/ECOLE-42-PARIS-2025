/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:47:12 by alex              #+#    #+#             */
/*   Updated: 2025/06/25 22:30:31 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FragTrap.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: name="FragTrap", use default stats
FragTrap::FragTrap(void) : ClapTrap() {
	this->_hitPt = FRAGTRAP_DEF_HIT_PT;
	this->_energyPt = FRAGTRAP_DEF_ENERGY_PT;
	this->_attackDmg = FRAGTRAP_DEF_ATTACK_DMG;
	std::cout << "FragTrap default constructor called!" << std::endl;
}

// Named: custom name, default stats
FragTrap::FragTrap(std::string &name) : ClapTrap(name) {
	this->_hitPt = FRAGTRAP_DEF_HIT_PT;
	this->_energyPt = FRAGTRAP_DEF_ENERGY_PT;
	this->_attackDmg = FRAGTRAP_DEF_ATTACK_DMG;
	std::cout << "FragTrap called *" << name << "* was constructed!" << std::endl;
}

// Copy: use assignment operator to clone all fields
FragTrap::FragTrap(FragTrap const &src) : ClapTrap() {
	*this = src;
	std::cout << "FragTrap copy constructor called!" << std::endl;
}

// Assignment: clone fields if not self
FragTrap &	FragTrap::operator=(FragTrap const & src) {
	if (this != &src) {
		this->_name = src.getName();
		this->_hitPt = src.getHitPt();
		this->_energyPt = src.getEnergyPt();
		this->_attackDmg = src.getAttackDmg();
	}
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	FragTrap::highFivesGuys(void)
{
	if (this->_hitPt == 0)
	{
		std::cout << "🫡 FragTrap *" << this->_name
		          << "* is too busted for high-fives, but sends a heroic salute!"
		          << std::endl;
	}
	else
	{
		std::cout << "🖐️ FragTrap *" << this->_name
		          << "* throws an epic high-five your way—catch it! 🎉"
		          << std::endl;
	}
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

FragTrap::~FragTrap(void) {
	std::cout << "The FragTrap named *" << this->_name << "* was killed in combat... R.I.P"<< std::endl;
}