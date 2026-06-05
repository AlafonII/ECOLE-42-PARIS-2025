/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:07:43 by alex              #+#    #+#             */
/*   Updated: 2025/06/25 23:19:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_H
#define SCAVTRAP_H

# include "ClapTrap.hpp"

#define SCAVTRAP_DEF_HIT_PT		100	// default hit points
#define SCAVTRAP_DEF_ENERGY_PT	50	// default energy points
#define SCAVTRAP_DEF_ATTACK_DMG	20	// default attack damage

class ScavTrap : public ClapTrap {
	private:
		ScavTrap(void);									// private default ctor to prevent unnamed
	public:
		ScavTrap(std::string & name);					// named ctor: set custom name + stats
		ScavTrap(ScavTrap const & src);					// copy ctor: clone another ScavTrap
		~ScavTrap(void);								// destructor

		ScavTrap &	operator=(ScavTrap const & src);	// copy assignment

		void	attack(std::string & target);			// attack override, consumes energy
		void	guardGate(void);						// enter gatekeeper mode
};

#endif
