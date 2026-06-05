/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newZombie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:42:52 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:48:53 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: newZombie.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Defines the newZombie function, which dynamically allocates a
 *   Zombie instance on the heap with a given name and returns a
 *   pointer to it.
 * ---------------------------------------------------------------
 */

/*
 * Function: newZombie
 * -------------------
 * Allocates a new Zombie object on the heap, initializes it with
 * the provided name, and returns a pointer to it. The caller is
 * responsible for deleting the returned Zombie to avoid memory leaks.
 *
 * Parameters:
 *   name - A std::string specifying the name to assign to the new Zombie.
 *
 * Returns:
 *   A pointer to the newly created Zombie object (Zombie*).
 *
 * Behavior:
 *   1. Calls Zombie's constructor Zombie(std::string) via 'new'.
 *   2. Returns the pointer so the caller can use and eventually delete it.
 */
Zombie* newZombie(std::string name)
{
    Zombie* z = new Zombie(name);  // Heap-allocate a Zombie named 'z'
    return z;                      // Return the pointer; caller must delete
}