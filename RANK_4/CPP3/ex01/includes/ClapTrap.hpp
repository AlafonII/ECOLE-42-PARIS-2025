/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 20:34:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <string>
#include <iostream>

# define CLAPTRAP_DEF_HIT_PT 		10	// default hit points
# define CLAPTRAP_DEF_ENERGY_PT 	10	// default energy points
# define CLAPTRAP_DEF_ATTACK_DMG	0	// default attack damage

class ClapTrap {
protected:
	ClapTrap(void);					// default ctor: name="ClapTrap", default stats

	std::string		_name;			// ClapTrap's identifier
	unsigned int	_hitPt;			// current hit points
	unsigned int	_energyPt;		// current energy points
	unsigned int	_attackDmg;		// damage dealt per attack
public:
	ClapTrap(std::string name);							// named ctor: set custom name, default stats
	ClapTrap(ClapTrap const &src);						// copy ctor: duplicate another ClapTrap
	~ClapTrap(void);									// destructor

	ClapTrap		&operator=(ClapTrap const &src);	// copy assignment

	void			attack(std::string &target);		// perform attack on target
	void			takeDamage(unsigned int amount);	// reduce hit points by amount
	void			beRepaired(unsigned int amount);	// restore hit points by amount

	std::string		getName(void) const;				// retrieve name
	unsigned int	getHitPt(void) const;				// retrieve hit points
	unsigned int	getEnergyPt(void) const;			// retrieve energy points
	unsigned int	getAttackDmg(void) const;			// retrieve attack damage
};

#endif
