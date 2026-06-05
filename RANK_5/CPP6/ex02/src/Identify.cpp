/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Identify.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:58:30 by alex              #+#    #+#             */
/*   Updated: 2025/09/09 10:37:10 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/Identify.hpp"
#include "../incl/A.hpp"
#include "../incl/B.hpp"
#include "../incl/C.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

// Randomly create A, B, or C
Base*	generate(void)
{
	static bool seeded = false;
	if (!seeded) {
		std::srand(std::time(NULL));
		seeded = true;
	}

	int r = std::rand() % 3;
	if (r == 0) return new A();
	if (r == 1) return new B();
	return new C();
}

// Identify by pointer (nullptr-safe)
void	identify(Base* p)
{
	if (!p)
	{
		std::cout << "Pointer: null\n";
		return;
	}
	if (dynamic_cast<A*>(p)) { std::cout << "Pointer: A\n"; return; }
	if (dynamic_cast<B*>(p)) { std::cout << "Pointer: B\n"; return; }
	if (dynamic_cast<C*>(p)) { std::cout << "Pointer: C\n"; return; }
	std::cout << "Pointer: Unknown\n";
}

// Identify by reference (use exceptions)
void	identify(Base& p)
{
	// Try A
	try { (void)dynamic_cast<A&>(p); std::cout << "Reference: A\n"; return; } catch (...) {}
	// Try B
	try { (void)dynamic_cast<B&>(p); std::cout << "Reference: B\n"; return; } catch (...) {}
	// Try C
	try { (void)dynamic_cast<C&>(p); std::cout << "Reference: C\n"; return; } catch (...) {}
	std::cout << "Reference: Unknown\n";
}
