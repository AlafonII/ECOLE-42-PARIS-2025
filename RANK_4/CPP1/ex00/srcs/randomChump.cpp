/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomChump.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:42:55 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:46:40 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: randomChump.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Defines the randomChump function, which creates a temporary
 *   Zombie instance on the stack with a given name and immediately
 *   invokes its announce() method.
 * ---------------------------------------------------------------
 */

/*
 * Function: randomChump
 * ---------------------
 * Creates a Zombie object using stack allocation, gives it the
 * provided name, calls its announce() method to print its message,
 * and lets it go out of scope (destroyed automatically).
 *
 * Parameters:
 *   name - A std::string specifying the name to assign to the new Zombie.
 *
 * Behavior:
 *   1. Calls Zombie's constructor Zombie(std::string) to build 'z'.
 *   2. Invokes z.announce(), which typically outputs something like
 *      "<name>: BraiiiiiiinnnzzzZ...".
 *   3. Upon exiting this function, 'z' falls out of scope and its
 *      destructor is called, cleaning up any resources if needed.
 */
void randomChump(std::string name)
{
    Zombie z(name);    // Stack-allocated Zombie object named 'z'
    z.announce();      // Make the Zombie announce itself
    // 'z' is destroyed here when the function returns
}