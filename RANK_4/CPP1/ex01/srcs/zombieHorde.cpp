/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombieHorde.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:40:45 by alex              #+#    #+#             */
/*   Updated: 2025/06/23 17:54:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: zombieHorde.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Creates a horde of N Zombies on the heap, assigns each the
 *   provided name, makes them announce themselves, and returns
 *   a pointer to the array.
 * ---------------------------------------------------------------
 */

/*
 * Function: zombieHorde
 * ---------------------
 * Dynamically allocates an array of N Zombie objects on the heap,
 * sets each Zombie's name to 'name', invokes announce() on each,
 * and returns the pointer to the first element of the array.
 *
 * Parameters:
 *   N    - Number of Zombies to create (must be > 0).
 *   name - Name to assign to each Zombie in the horde.
 *
 * Returns:
 *   Pointer to the first Zombie in the allocated array, or NULL
 *   if N <= 0. Caller must call 'delete[]' on the returned pointer
 *   to avoid memory leaks.
 *
 * Behavior:
 *   1. If N <= 0, returns NULL to signal invalid horde size.
 *   2. Allocates N Zombies on the heap via 'new Zombie[N]' using
 *      the default constructor (name will be set afterward).
 *   3. Loops from 0 to N-1:
 *        a. Calls setName(name) on horde[i] to assign its name.
 *        b. Calls announce() to print "<name>: BraiiiiiiinnnzzzZ...".
 *   4. Returns the pointer to the first element of the horde.
 */
Zombie* zombieHorde(int N, std::string name)
{
    if (N <= 0)
        return NULL;                // Invalid count → signal failure

    Zombie* horde = new Zombie[N];  // Allocate N Zombies on the heap

    for (int i = 0; i < N; i++)
    {
        horde[i].setName(name);     // Assign the specified name
        horde[i].announce();        // Make the Zombie announce itself
    }
    return horde;                   // Return pointer to the horde array
}
