/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:46:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 17:59:11 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
using namespace std;

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Demonstrates the difference between accessing a std::string
 *   directly, via a pointer, and via a reference. Prints out
 *   each address and value to the console.
 * ---------------------------------------------------------------
 */

/*
 * Function: main
 * --------------
 * Role:
 *   - Creates a std::string variable 'x' initialized to
 *     "HI THIS IS BRAIN".
 *   - Declares a pointer and a reference to 'x'.
 *   - Prints the addresses of 'x', the pointer, and the reference.
 *   - Prints the value of 'x' directly, via the pointer, and via the reference.
 *
 * Returns:
 *   0 on successful execution.
 */
int	main(void)
{
	std::string	x = "HI THIS IS BRAIN";		// Original string variable

	std::string*	stringPTR = &x;			// Pointer holding the address of x
	std::string&	stringREF = x;			// Reference alias to x

	// Print the address of x
	std::cout << "Address of x: " << &x << std::endl;

	// Print the address stored in stringPTR (should match &x)
	std::cout << "Address held by stringPTR: " << stringPTR << std::endl;

	// Print the address of stringREF (also matches &x)
	std::cout << "Address held by stringREF: " << &stringREF << std::endl;

	// Print the value of x directly
	std::cout << "Value of x: " << x << std::endl;

	// Print the value via the pointer (dereferenced)
	std::cout << "Value via pointer: " << *stringPTR << std::endl;

	// Print the value via the reference
	std::cout << "Value via reference: " << stringREF << std::endl;

	return (0);
}
