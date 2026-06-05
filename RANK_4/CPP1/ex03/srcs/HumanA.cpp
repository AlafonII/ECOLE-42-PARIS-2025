/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:12:14 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:43:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HumanA.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: HumanA.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the HumanA class, which represents a human armed with
 *   a Weapon reference. HumanA can perform an attack using their
 *   weapon and announces when they are destroyed.
 * ---------------------------------------------------------------
 */

 /*
 * Function: HumanA (Constructor)
 * ------------------------------
 * Initializes a HumanA instance with the given name and binds it to
 * the provided Weapon reference.
 *
 * Parameters:
 *   name - const std::string& specifying the human's name.
 *   w    - Weapon& reference to the weapon the human will use.
 *
 * Behavior:
 *   Uses an initializer list to set the private 'name' member and
 *   bind the 'weapon' reference to the passed-in Weapon object.
 */
HumanA::HumanA(const std::string& name, Weapon& w) : name(name), weapon(w) {}

/*
 * Function: ~HumanA (Destructor)
 * ------------------------------
 * Called automatically when a HumanA object goes out of scope or is
 * deleted. Logs a message indicating the human has been destroyed.
 */
HumanA::~HumanA() {
	std::cout << name << " is gone forever" << std::endl;
}

/*
 * Function: attack
 * ----------------
 * Causes the HumanA to perform an attack by printing their name and
 * the type of their weapon.
 *
 * Behavior:
 *   Retrieves the weapon type via weapon.getType() and outputs
 *   "<name> attacks with their <weapon type>" to standard output.
 */
void HumanA::attack() {
	std::cout << name << " attacks with their " << weapon.getType() << std::endl;
}