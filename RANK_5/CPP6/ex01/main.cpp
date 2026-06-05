/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:40:09 by alex              #+#    #+#             */
/*   Updated: 2025/09/09 10:34:47 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Serializer.hpp"
#include <iostream>
#include <iomanip>

static void	printHeader(const std::string& title)
{
	std::cout << "\n=== " << title << " ===\n";
}

static void	printData(const char* label, Data* p)
{
	std::cout << label << " ptr=" << p;
	if (p)
		std::cout << " {name=\"" << p->name << "\", value=" << p->value << ", ratio=" << std::fixed << std::setprecision(2) << p->ratio << "}";
	std::cout << "\n";
}

int main()
{
	printHeader("Null pointer round trip");
	{
		Data* p = 0;
		uintptr_t raw = Serializer::serialize(p);
		Data* q = Serializer::deserialize(raw);
		printData("p", p);
		std::cout << "raw=" << raw << "\n";
		printData("q", q);
		std::cout << "same? " << (p == q ? "yes" : "no") << "\n";
	}

	printHeader("Stack object round trip (identity & integrity)");
	{
		Data a;
		a.name = "alpha";
		a.value = 42;
		a.ratio = 3.14;

		printData("a (before)", &a);
		uintptr_t raw = Serializer::serialize(&a);
		std::cout << "raw=" << raw << "\n";
		Data* r = Serializer::deserialize(raw);
		printData("r (after)", r);
		std::cout << "same address? " << (&a == r ? "yes" : "no") << "\n";
	}

	printHeader("Heap object round trip");
	{
		Data* p = new Data;
		p->name = "heap";
		p->value = -2147483648;
		p->ratio = 1.0 / 0.0; // inf

		uintptr_t raw = Serializer::serialize(p);
		Data* q = Serializer::deserialize(raw);
		printData("p", p);
		std::cout << "raw=" << raw << "\n";
		printData("q", q);
		std::cout << "same? " << (p == q ? "yes" : "no") << "\n";

		delete p; // q is dangling now
	}

	printHeader("Multiple distinct objects (no aliasing)");
	{
		Data x; x.name = "x"; x.value = 1; x.ratio = 0.25;
		Data y; y.name = "y"; y.value = 2; y.ratio = 0.50;

		uintptr_t rx = Serializer::serialize(&x);
		uintptr_t ry = Serializer::serialize(&y);

		Data* px = Serializer::deserialize(rx);
		Data* py = Serializer::deserialize(ry);

		printData("px", px);
		printData("py", py);
		std::cout << "addresses distinct? " << (px != py ? "yes" : "no") << "\n";
	}

	return 0;
}
