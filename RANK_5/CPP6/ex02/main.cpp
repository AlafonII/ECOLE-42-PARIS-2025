/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:40:09 by alex              #+#    #+#             */
/*   Updated: 2025/09/09 10:06:54 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Identify.hpp"
#include "incl/A.hpp"
#include "incl/B.hpp"
#include "incl/C.hpp"

#include <iostream>

static void	printHeader(const std::string& title)
{
	std::cout << "\n=== " << title << " ===\n";
}

int	main()
{
	printHeader("Identify on manually constructed instances");
	{
		A a; B b; C c;
		Base* pa = &a; Base* pb = &b; Base* pc = &c;

		identify(pa); identify(*pa);
		identify(pb); identify(*pb);
		identify(pc); identify(*pc);
	}

	printHeader("Identify with nullptr");
	{
		Base* pn = 0;
		identify(pn);
		// No reference test with null; would be UB
	}

	printHeader("Random generate() x10 (new/delete)");
	for (int i = 0; i < 10; ++i)
	{
		Base* p = generate();
		identify(p);
		identify(*p);
		delete p;
	}

	return 0;
}
