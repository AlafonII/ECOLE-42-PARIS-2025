/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:42:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:52:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: Zombie.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the Zombie class methods, including a default
 *   constructor, a name-based constructor, destructor, announce,
 *   and a setter for the name. Each Zombie can be named or
 *   renamed, announce itself, and reports when it is destroyed.
 * ---------------------------------------------------------------
 */

/*
 * Function: Zombie (Default Constructor)
 * --------------------------------------
 * Initializes a new Zombie instance with an empty name.
 *
 * Parameters:
 *   None
 *
 * Behavior:
 *   Uses an initializer list to set the private member 'name'
 *   to an empty string.
 */
Zombie::Zombie(void) : name("")       // Initialize 'name' as empty
{

}

/*
 * Function: Zombie (Parameterized Constructor)
 * --------------------------------------------
 * Initializes a new Zombie instance with the provided name.
 *
 * Parameters:
 *   name - A const reference to a std::string specifying
 *          the Zombie's initial name.
 *
 * Behavior:
 *   Uses an initializer list to copy the provided 'name'
 *   into the private member variable.
 */
Zombie::Zombie(const std::string& name)
  : name(name)     // Initialize 'name' with the constructor argument
{
}

/*
 * Function: ~Zombie (Destructor)
 * ------------------------------
 * Automatically called when a Zombie object goes out of scope
 * or is deleted. Logs a message indicating the Zombie has been
 * destroyed.
 *
 * Parameters:
 *   None
 *
 * Behavior:
 *   Outputs "<name> is destroyed" to standard output.
 */
Zombie::~Zombie(void)
{
    std::cout << name << " is destroyed" << std::endl;
}

/*
 * Function: announce
 * ------------------
 * Makes the Zombie announce itself by printing its name
 * followed by "BraiiiiiiinnnzzzZ...".
 *
 * Parameters:
 *   None
 *
 * Behavior:
 *   Outputs "<name>: BraiiiiiiinnnzzzZ..." to standard output.
 */
void Zombie::announce(void)
{
    std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

/*
 * Function: setName
 * -----------------
 * Assigns a new name to this Zombie instance.
 *
 * Parameters:
 *   n - A const reference to a std::string to set as the new name.
 *
 * Behavior:
 *   Copies 'n' into the private member variable 'name'.
 */
void Zombie::setName(const std::string& n)
{
    name = n;
}
