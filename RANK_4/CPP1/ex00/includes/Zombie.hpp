/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:43:22 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:58:22 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include <string>
#include <iostream>

/*
 * ---------------------------------------------------------------
 * Filename: Zombie.hpp
 * ---------------------------------------------------------------
 * Description:
 *   Declaration of the Zombie class, which represents a simple
 *   entity with a name that can announce itself and reports when
 *   it is destroyed.
 * ---------------------------------------------------------------
 */

class Zombie {
private:
    std::string name;

public:
	Zombie(const std::string name);
	~Zombie();
    void announce(void);
};

#endif
