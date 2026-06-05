/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:13:13 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:05:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>

/*
 * ---------------------------------------------------------------
 * Filename: Weapon.hpp
 * ---------------------------------------------------------------
 * Description:
 *   Declaration of the Weapon class, which encapsulates a weapon
 *   type string. Allows setting and retrieving the type, and logs
 *   when destroyed.
 * ---------------------------------------------------------------
 */

class Weapon {
private:
	std::string type;
public:
	Weapon(std::string type);
	~Weapon();
	void setType(const std::string&);
	const std::string& getType() const;
};

#endif