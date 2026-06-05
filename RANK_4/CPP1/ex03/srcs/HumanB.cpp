/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:12:37 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:48:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HumanB.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: HumanB.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the HumanB class, which represents a human that can
 *   pick up, drop, and switch weapons at runtime via a pointer. The
 *   behavior adapts based on whether the weapon's type is empty or
 *   already held. Also defines attack logic that varies if unarmed.
 * ---------------------------------------------------------------
 */

 /*
 * Function: HumanB (Constructor)
 * ------------------------------
 * Initializes a HumanB instance with the given name and no weapon.
 *
 * Parameters:
 *   name - const std::string& specifying the human's name.
 *
 * Behavior:
 *   Uses an initializer list to set 'name' and initialize the
 *   'weapon' pointer to NULL, indicating no weapon equipped yet.
 */
HumanB::HumanB(const std::string& name) : name(name) , weapon(NULL) {}

/*
 * Function: ~HumanB (Destructor)
 * ------------------------------
 * Called automatically when a HumanB object goes out of scope or
 * is deleted. Logs a message indicating the human has been destroyed.
 */
HumanB::~HumanB() {
	std::cout << name << " is gone forever" << std::endl;
}

/*
 * Function: setWeapon
 * -------------------
 * Assigns or removes a Weapon for this HumanB, with feedback:
 *   - If the provided weapon's type is empty, treats it as a drop/fail.
 *   - Otherwise, equips the new weapon, printing whether it's a pickup
 *     or a switch from an existing weapon.
 *
 * Parameters:
 *   w - Weapon& reference to the weapon to equip or drop.
 *
 * Behavior:
 *   1. If w.getType() is empty:
 *        a. If no current weapon or already empty, print failure message.
 *        b. Else, print drop message and clear weapon pointer.
 *   2. Else (non-empty type):
 *        a. If no current weapon, print pickup message.
 *        b. Else, print switch message.
 *        c. Store address of w in 'weapon'.
 */
void HumanB::setWeapon(Weapon &w)
{
    if (w.getType().empty())
    {
        if (weapon == NULL || weapon->getType().empty())
            std::cout << name
                      << "picked up a weapon and failed like a loser!" << std::endl;
        else
            std::cout << name << " dropped their weapon... basically dead!" << std::endl;
        weapon = NULL;
    }
    else
    {
        if (weapon == NULL || weapon->getType().empty())
            std::cout << name << " picked up " << w.getType() << std::endl;
        else
            std::cout << name << " is now using " << w.getType() << " that they picked up out of nowhere!" << std::endl;
        weapon = &w;
    }
}

/*
 * Function: attack
 * ----------------
 * Performs an attack action:
 *   - If unarmed (pointer NULL or empty type), prints a loser message.
 *   - Otherwise, prints a gentle hit message with the weapon type.
 */
void HumanB::attack()
{
    if (weapon == NULL || weapon->getType().empty())
        std::cout << name << " has no weapons... LOSER!" << std::endl;
    else
        std::cout << name << " gently hit the opponent with a " << weapon->getType() << std::endl;
}
