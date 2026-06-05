/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:13:28 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 19:13:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Harl.hpp"

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Instantiates a Harl object and issues complaints at various
 *   severity levels: DEBUG, WARNING, INFO, and ERROR.
 * ---------------------------------------------------------------
 */

int main(void) {
	Harl c;
	c.complain("DEBUG");
	c.complain("INFO");
	c.complain("WARNING");
	c.complain("ERROR");
}
