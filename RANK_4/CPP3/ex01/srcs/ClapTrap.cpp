/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 20:41:08 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClapTrap.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: name="ClapTrap", use default stats
ClapTrap::ClapTrap(void)
	: _name("ClapTrap"),
	_hitPt(CLAPTRAP_DEF_HIT_PT),
	_energyPt(CLAPTRAP_DEF_ENERGY_PT),
	_attackDmg(CLAPTRAP_DEF_ATTACK_DMG) {
		std::cout << "ClapTrap default constructor called, he is called ClapTrap!" << std::endl;
}

// Named: custom name, default stats
ClapTrap::ClapTrap(std::string name)
	: _name(name),
	_hitPt(CLAPTRAP_DEF_HIT_PT),
	_energyPt(CLAPTRAP_DEF_ENERGY_PT),
	_attackDmg(CLAPTRAP_DEF_ATTACK_DMG) {
	std::cout << "ClapTrap called *" << name << "* was constructed!" << std::endl;
}

// Copy: use assignment operator to clone all fields
ClapTrap::ClapTrap(ClapTrap const & src) {
	*this = src;
	std::cout << "ClapTrap copy constructor called!" << std::endl;
}

// Assignment: clone fields if not self
ClapTrap &ClapTrap::operator=(ClapTrap const & src) {
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
 * 						GETTERS
 * -----------------------------------------------------------
 */

std::string	ClapTrap::getName(void) const {
	return (this->_name);
}

unsigned int	ClapTrap::getHitPt(void) const {
	return (this->_hitPt);
}

unsigned int	ClapTrap::getEnergyPt(void) const {
	return (this->_energyPt);
}

unsigned int	ClapTrap::getAttackDmg(void) const {
	return (this->_attackDmg);
}

/*
 * -----------------------------------------------------------
 * 						MEMBER_FUNCTIONS
 * -----------------------------------------------------------
 */

void	ClapTrap::attack(std::string &target)
{
	if (this->_hitPt == 0)
	{
		std::cout << "💀 ClapTrap *" << this->_name
				  << "* can't attack: it's destroyed!"
				  << std::endl;
		return ;
	}
	if (this->_energyPt == 0)
	{
		std::cout << "🔋⚠️ ClapTrap *" << this->_name
				  << "* can't attack: no more energy 😞"
				  << std::endl;
		return ;
	}
	if (target == this->_name)
		target = "itself";
	--this->_energyPt;
	std::cout << "🗡️ ClapTrap *" << this->_name
			  << "* attacks *" << target
			  << "* and deals " << this->_attackDmg
			  << " damage! 💥"
			  << std::endl;
}

void	ClapTrap::takeDamage(unsigned int amount)
{
	if (this->_hitPt == 0)
	{
		std::cout << "💀 ClapTrap *" << this->_name
				  << "* can't take damage: it's already dead! 💀"
				  << std::endl;
		return ;
	}
	if (amount >= this->_hitPt)
	{
		this->_hitPt = 0;
		std::cout << "💥 ClapTrap *" << this->_name
				  << "* takes " << amount
				  << " damage and is obliterated! 💥"
				  << std::endl;
	}
	else
	{
		this->_hitPt -= amount;
		std::cout << "🔥 ClapTrap *" << this->_name
				  << "* takes " << amount
				  << " damage! 🔥"
				  << std::endl;
	}
}

void	ClapTrap::beRepaired(unsigned int amount)
{
	if (this->_hitPt == 0)
	{
		std::cout << "💀 ClapTrap *" << this->_name
				  << "* can't repair itself: it's destroyed!"
				  << std::endl;
		return ;
	}
	if (this->_energyPt == 0)
	{
		std::cout << "🔋⚠️ ClapTrap *" << this->_name
				  << "* can't repair itself: no energy left 😞"
				  << std::endl;
		return ;
	}
	if (this->_hitPt + amount > CLAPTRAP_DEF_HIT_PT)
		amount = CLAPTRAP_DEF_HIT_PT - this->_hitPt;
	if (amount == 0)
	{
		--this->_energyPt;
		std::cout << "♻️ ClapTrap *" << this->_name
				  << "* tries to repair at full health and wastes energy! 🤦"
				  << std::endl;
		return ;
	}
	--this->_energyPt;
	this->_hitPt += amount;
	std::cout << "🛠️ ClapTrap *" << this->_name
			  << "* repairs itself for " << amount
			  << " hit points! ✨"
			  << std::endl;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

ClapTrap::~ClapTrap(void) {
	std::cout << "The ClapTrap named *" << this->_name << "* was killed in combat... R.I.P"<< std::endl;
}
