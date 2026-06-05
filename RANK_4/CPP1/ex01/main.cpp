/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:46:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 23:42:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Zombie.hpp"
#include <climits>

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Entry point for the Zombie horde program. Demonstrates how to
 *   create a dynamic array of Zombies using zombieHorde, then
 *   properly clean up the allocated memory.
 * ---------------------------------------------------------------
 */

Zombie* zombieHorde(int N, std::string name);

/*
 * Function: main
 * --------------
 * Role:
 *   - Calls zombieHorde to create an array of 10 Zombies named
 *     "Zombie" on the heap.
 *   - Deletes the entire horde to free heap memory.
 *
 * Behavior:
 *   1. zombieHorde(10, "Zombie")
 *      - Allocates 10 Zombies via new[]
 *      - Sets each name to "Zombie"
 *      - Calls announce() on each Zombie
 *   2. delete[] horde
 *      - Invokes destructor for each Zombie in the array
 *      - Frees the allocated memory block
 *
 * Returns:
 *   0 on successful completion.
 */
int main(void)
{
	long long hordeSize = 10;							// Set horde size
	if (hordeSize > INT_MAX)							// Check if more than INT_MAX
        return (1);
    Zombie* horde = zombieHorde(hordeSize, "Zombie");	// Create X Zombies
    delete[] horde;										// Clean up the horde
    return (0);											// Exit program
}
