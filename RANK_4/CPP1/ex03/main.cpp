/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:12:52 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:46:13 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/HumanA.hpp"
#include "includes/HumanB.hpp"
#include "includes/Weapon.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Demonstrates how Weapon, HumanA, and HumanB interact when a
 *   weapon’s type changes. Shows that HumanA holds a reference and
 *   reflects updates immediately, as does HumanB holding a pointer.
 * ---------------------------------------------------------------
 */

/*
 * Function: main
 * --------------
 * Creates two separate scopes:
 * 1) A Weapon and a HumanA bound by reference:
 *    - Bob attacks, then the club’s type is changed and Bob attacks again,
 *      showing that the reference sees the updated type.
 * 2) A Weapon and a HumanB acquiring the weapon by pointer:
 *    - Jim is given the club, attacks, then the club’s type is changed
 *      and Jim attacks again, showing that the pointer sees the update.
 *
 * Returns:
 *   0 on successful execution.
 */
int	main(void)
{
	{
		Weapon	club("crude spiked club");			// Create a weapon on the stack
		HumanA	bob("Bob", club);					// HumanA binds to the weapon by reference
		bob.attack();								// Outputs "Bob attacks with their crude spiked club"
		club.setType("some other type of club");
		bob.attack();								// Outputs "Bob attacks with their some other type of club"
	}
	// 'bob' and 'club' go out of scope here; destructors run

	{
		Weapon	club("crude spiked club");			// New weapon instance
		HumanB	jim("Jim");							// HumanB starts without a weapon
		jim.setWeapon(club);						// Assign weapon by pointer
		jim.attack();								// Outputs "Jim attacks with their crude spiked club"
		club.setType("some other type of club");
		jim.attack();								// Outputs "Jim attacks with their some other type of club"
	}
	// 'jim' and 'club' go out of scope here; destructors run

	return (0);
}
