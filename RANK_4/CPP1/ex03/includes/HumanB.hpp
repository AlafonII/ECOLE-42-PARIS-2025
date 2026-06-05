/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:13:38 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:04:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANB_HPP
#define HUMANB_HPP

#include <string>
#include "Weapon.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: HumanB.hpp
 * ---------------------------------------------------------------
 * Description:
 *   Declaration of the HumanB class, representing a human that can
 *   acquire a Weapon at runtime via a pointer. HumanB can set its
 *   weapon after construction, attack with it, and reports when it
 *   is destroyed.
 * ---------------------------------------------------------------
 */

class HumanB {
private:
	std::string name;
	Weapon* weapon;
public:
	HumanB(const std::string& name);
	~HumanB(void);
	void setWeapon(Weapon&);
	void attack();
};

#endif
