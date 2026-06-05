/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:46:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:53:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Entry point of the Zombie program. Demonstrates creating a
 *   Zombie on the stack with randomChump, and a Zombie on the heap
 *   with newZombie, then announcing and cleaning up.
 * ---------------------------------------------------------------
 */

Zombie*	newZombie(std::string name);
void	randomChump(std::string name);

/*
 * Function: main
 * --------------
 * Role:
 *   - Calls randomChump to create a temporary stack-allocated Zombie
 *     named "Foo" and make it announce itself.
 *   - Uses newZombie to heap-allocate a Zombie named "Zombie", calls
 *     announce(), then deletes it to free memory.
 *
 * Behavior:
 *   1. randomChump("Foo") constructs a Zombie on the stack, invokes
 *      announce(), and automatically destroys it when the function
 *      returns.
 *   2. newZombie("Zombie") allocates a Zombie on the heap, returns a
 *      pointer which is used to call announce().
 *   3. delete heapZ deallocates the heap-allocated Zombie, invoking
 *      its destructor.
 *
 * Returns:
 *   0 on successful execution.
 */
int main(void)
{
    randomChump("Foo");						// Stack-allocate "Foo", announce, then destroy
    Zombie* heapZ = newZombie("Zombie");	// Heap-allocate "Zombie"
    heapZ->announce();						// Make the heap Zombie announce itself
    delete heapZ;							// Free heap memory and call destructor
    return 0;
}
