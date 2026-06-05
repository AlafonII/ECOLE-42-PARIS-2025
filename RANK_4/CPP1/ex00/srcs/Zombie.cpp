/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:42:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:17:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Zombie.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: Zombie.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Implements the Zombie class methods, including the constructor,
 *   destructor, and announce function. Each Zombie has a name and
 *   can announce itself. When a Zombie object is destroyed, it logs
 *   a destruction message.
 * ---------------------------------------------------------------
 */

/*
 * Function: Zombie (Constructor)
 * ------------------------------
 * Initializes a new Zombie instance with the provided name.
 *
 * Parameters:
 *   name - A std::string representing the Zombie's name.
 *
 * Behavior:
 *   Uses an initializer list to directly assign the 'name' member
 *   variable. No additional operations are performed in the body.
 */
Zombie::Zombie(std::string name)
    : name(name)   // Initialize member variable 'name' with constructor argument
{

}

/*
 * Function: ~Zombie (Destructor)
 * ------------------------------
 * Called automatically when a Zombie object goes out of scope or is
 * deleted. Logs a message indicating the Zombie has been destroyed.
 *
 * Behavior:
 *   Outputs "<name> is destroyed" to standard output, signaling cleanup.
 */
Zombie::~Zombie()
{
    std::cout << name << " is destroyed" << std::endl;
}

/*
 * Function: announce
 * ------------------
 * Makes the Zombie announce itself by printing its name followed by
 * the classic "BraiiiiiiinnnzzzZ..." message.
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