/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:13:25 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:04:20 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANA_HPP
#define HUMANA_HPP

#include <string>
#include "Weapon.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: HumanA.hpp
 * ---------------------------------------------------------------
 * Description:
 *   Declaration of the HumanA class, representing a human armed with
 *   a Weapon reference. HumanA can attack using its bound weapon and
 *   announces when it is destroyed.
 * ---------------------------------------------------------------
 */

class HumanA {
private:
	std::string name;
	Weapon& weapon;
public:
	HumanA(const std::string& name, Weapon& w);
	~HumanA(void);
	void attack();
};

#endif
