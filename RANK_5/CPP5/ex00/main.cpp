/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:12:52 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:13:23 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Bureaucrat.hpp"

static void	printHeader(const std::string& title)
{
	std::cout << "\n=== " << title << " ===\n";
}

int main()
{
	printHeader("Construct valid");
	try
	{
		Bureaucrat a("Top", 1);
		Bureaucrat b("Bottom", 150);
		Bureaucrat c("Mid", 75);
		std::cout << a << "\n";
		std::cout << b << "\n";
		std::cout << c << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Construct invalid: 0 (too high)");
	try
	{
		Bureaucrat badHigh("BadHigh", 0);
		std::cout << badHigh << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Construct invalid: 151 (too low)");
	try
	{
		Bureaucrat badLow("BadLow", 151);
		std::cout << badLow << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Increment within bounds");
	try
	{
		Bureaucrat x("NearTop", 2);
		std::cout << x << "\n";
		x.incrementGrade(); // 2 -> 1
		std::cout << x << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Decrement within bounds");
	try
	{
		Bureaucrat y("NearBottom", 149);
		std::cout << y << "\n";
		y.decrementGrade(); // 149 -> 150
		std::cout << y << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Increment at upper limit should throw");
	try
	{
		Bureaucrat t("Top", 1);
		std::cout << t << "\n";
		t.incrementGrade(); // should throw
		std::cout << "NOPE " << t << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Decrement at lower limit should throw");
	try
	{
		Bureaucrat b("Bottom", 150);
		std::cout << b << "\n";
		b.decrementGrade(); // should throw
		std::cout << "NOPE " << b << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Copy constructor + assignment");
	try
	{
		Bureaucrat orig("Alice", 42);
		Bureaucrat copy(orig); // same name, same grade
		Bureaucrat other("Bob", 120);

		std::cout << "orig: " << orig << "\n";
		std::cout << "copy: " << copy << "\n";
		std::cout << "other: " << other << "\n";

		// assignment should copy grade only (name is const)
		other = orig;
		std::cout << "after assign, other: " << other << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Bulk ops crossing bounds (expect throw)");
	try
	{
		Bureaucrat z("Climber", 3);
		std::cout << z << "\n";
		z.incrementGrade(); // 2
		std::cout << z << "\n";
		z.incrementGrade(); // 1
		std::cout << z << "\n";
		z.incrementGrade(); // throw (to 0)
		std::cout << "NOPE " << z << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	try
	{
		Bureaucrat w("Diver", 149);
		std::cout << w << "\n";
		w.decrementGrade(); // 150
		std::cout << w << "\n";
		w.decrementGrade(); // throw (to 151)
		std::cout << "NOPE " << w << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Default constructor sanity");
	try
	{
		Bureaucrat d;
		std::cout << d << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	return 0;
}
