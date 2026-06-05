/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:12:03 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:03:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Weapon.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: Weapon.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the Weapon class, which encapsulates a weapon type
 *   string, allows modifying and retrieving the type, and logs a
 *   message when destroyed.
 * ---------------------------------------------------------------
 */

 /*
 * Function: Weapon (Constructor)
 * ------------------------------
 * Initializes a new Weapon instance with the provided type.
 *
 * Parameters:
 *   type - A std::string specifying the weapon's initial type.
 *
 * Behavior:
 *   Uses an initializer list to set the private 'type' member.
 */
Weapon::Weapon(std::string type) : type(type) {}

/*
 * Function: ~Weapon (Destructor)
 * ------------------------------
 * Called automatically when a Weapon object is destroyed.
 *
 * Behavior:
 *   Outputs "<type> has been destroyed!" to standard output,
 *   indicating that the Weapon is being cleaned up.
 */
Weapon::~Weapon() {
	std::cout << type << " has been detroyed!" << std::endl;
}

/*
 * Function: setType
 * -----------------
 * Updates the Weapon's type to a new value.
 *
 * Parameters:
 *   t - A const std::string& specifying the new type for the weapon.
 *
 * Behavior:
 *   Assigns 't' to the private 'type' member variable.
 */
void Weapon::setType(const std::string &t) {
	type = t;
}


/*
 * Function: getType
 * -----------------
 * Retrieves the current type of the Weapon.
 *
 * Returns:
 *   A const reference to the internal 'type' string.
 */
const std::string& Weapon::getType() const { 
	return type;
}