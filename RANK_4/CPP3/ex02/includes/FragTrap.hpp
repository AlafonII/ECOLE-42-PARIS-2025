/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:46:57 by alex              #+#    #+#             */
/*   Updated: 2025/06/25 23:22:22 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAGTRAP_CLASS_H
# define FRAGTRAP_CLASS_H

#include "ClapTrap.hpp"

# define FRAGTRAP_DEF_HIT_PT		100	// default hit points
# define FRAGTRAP_DEF_ENERGY_PT		100	// default energy points
# define FRAGTRAP_DEF_ATTACK_DMG	30	// default attack damage

class FragTrap : public ClapTrap {
private:
	FragTrap(void);									// private default ctor to prevent unnamed
public:
	FragTrap(std::string & name);					// named ctor: initialize with name + stats
	FragTrap(FragTrap const & src);					// copy ctor: clone another FragTrap
	~FragTrap(void);								// destructor

	FragTrap &	operator=(FragTrap const & src);	// copy assignment


	void	highFivesGuys(void);					 // special action: request high fives
};

#endif