/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:07:53 by alex              #+#    #+#             */
/*   Updated: 2025/06/25 20:45:22 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ScavTrap.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: name="ScavTrap", use default stats
ScavTrap::ScavTrap(void) : ClapTrap() {
	this->_hitPt = SCAVTRAP_DEF_HIT_PT;
	this->_energyPt = SCAVTRAP_DEF_ENERGY_PT;
	this->_attackDmg = SCAVTRAP_DEF_ATTACK_DMG;
	std::cout << "ScavTrap default constructor called!" << std::endl;
}

// Named: custom name, default stats
ScavTrap::ScavTrap(std::string &name) : ClapTrap(name) {
	this->_hitPt = SCAVTRAP_DEF_HIT_PT;
	this->_energyPt = SCAVTRAP_DEF_ENERGY_PT;
	this->_attackDmg = SCAVTRAP_DEF_ATTACK_DMG;
	std::cout << "ScavTrap called *" << name << "* was constructed!" << std::endl;
}

// Copy: use assignment operator to clone all fields
ScavTrap::ScavTrap(ScavTrap const &src) : ClapTrap() {
	*this = src;
	std::cout << "ScavTrap copy constructor called!" << std::endl;
}

// Assignment: clone fields if not self
ScavTrap &ScavTrap::operator=(ScavTrap const & src) {
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

void	ScavTrap::attack(std::string &target)
{
	if (this->_hitPt == 0)
	{
		std::cout << "💀 ScavTrap *" << this->_name
				  << "* can't attack: it's destroyed!"
				  << std::endl;
		return ;
	}
	if (this->_energyPt == 0)
	{
		std::cout << "🔋⚠️ ScavTrap *" << this->_name
				  << "* can't attack: no energy left 😞"
				  << std::endl;
		return ;
	}
	if (target == this->_name)
		target = "itself";
	--this->_energyPt;
	std::cout << "🦾 ScavTrap *" << this->_name
			  << "* leaps on *" << target
			  << "* and crushes it for " << this->_attackDmg
			  << " damage! 💥"
			  << std::endl;
}

void	ScavTrap::guardGate(void)
{
	if (this->_hitPt == 0)
	{
		std::cout << "💔 ScavTrap *" << this->_name
				  << "* can't guard the gate: it's broken!"
				  << std::endl;
	}
	else
	{
		std::cout << "🚪 ScavTrap *" << this->_name
				  << "* switches to Gate guarding mode! 🛡️"
				  << std::endl;
	}
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

ScavTrap::~ScavTrap(void) {
	std::cout << "The ScavTrap named *" << this->_name << "* was killed in combat... R.I.P"<< std::endl;
}