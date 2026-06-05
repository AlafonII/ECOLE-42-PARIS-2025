/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:37:00 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 19:16:37 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Harl.hpp"
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: Harl.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the Harl class, which can emit log messages at
 *   various levels (DEBUG, INFO, WARNING, ERROR) and dispatches
 *   them via the complain() method using a pointer-to-member.
 * ---------------------------------------------------------------
 */

/*
 * Function: Harl (Constructor)
 * ----------------------------
 * Initializes a new Harl instance and announces its creation.
 *
 * Behavior:
 *   Outputs "Harl is born!" to standard output.
 */
Harl::Harl() {
	std::cout << "Harl is born!" << std::endl;
}

/*
 * Function: ~Harl (Destructor)
 * ----------------------------
 * Called automatically when a Harl object goes out of scope or
 * is deleted. Announces Harl's destruction.
 *
 * Behavior:
 *   Outputs "Harl is dead!" to standard output.
 */
Harl::~Harl() {
	std::cout << "Harl is dead!" << std::endl;
}

void	Harl::debug(void) {
	std::cout << "This is the DEBUG message" << std::endl;
}

void	Harl::info(void) {
	std::cout << "This is the INFO message" << std::endl;
}

void	Harl::warning(void) {
	std::cout << "This is the WARNING message" << std::endl;
}

void	Harl::error(void) {
	std::cout << "This is the ERROR message" << std::endl;
}

/*
 * Function: complain
 * ------------------
 * Dispatches the appropriate log function based on the provided
 * level string. Uses an array of level names and a parallel array
 * of member-function pointers for lookup.
 *
 * Parameters:
 *   level - A std::string specifying one of: "DEBUG", "INFO", "WARNING", or "ERROR".
 *
 * Behavior:
 *   1. Defines an array of valid level strings.
 *   2. Defines an array of pointers to the corresponding member functions.
 *   3. Iterates through levels; when a match is found:
 *        - Invokes the matched member function via (this->*func)().
 *        - Returns immediately.
 *   4. If no match is found, does nothing.
 */
void	Harl::complain(std::string level)
{
	std::string	levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};	// Valid level names
	void		(Harl::*funcs[])() = {								// Corresponding member function pointers
		&Harl::debug,												// DEBUG handler
		&Harl::info,												// INFO handler
		&Harl::warning,												// WARNING handler
		&Harl::error												// ERROR handler
	};

	for (int i = 0; i < 4; i++)										// Loop over each level
	{
		if (level == levels[i])										// If the input matches this level
		{
			(this->*funcs[i])();									// Call the corresponding member function
			return;													// Exit after handling
		}
	}
}
