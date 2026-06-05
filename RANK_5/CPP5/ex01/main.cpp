/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:12:52 by alex              #+#    #+#             */
/*   Updated: 2025/09/02 09:56:02 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Bureaucrat.hpp"
#include "incl/Form.hpp"

static void	printHeader(const std::string& title)
{
	std::cout << "\n=== " << title << " ===\n";
}

int main()
{
	printHeader("Construct valid Forms");
	try
	{
		Form a("A", 1, 1);
		Form b("B", 150, 150);
		Form c("C", 42, 75);
		std::cout << a << "\n" << b << "\n" << c << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Construct invalid Forms (sign grade out of bounds)");
	try
	{
		Form f("BadHighSign", 0, 10); std::cout << f << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}
	try
	{
		Form f("BadLowSign", 151, 10); std::cout << f << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("Construct invalid Forms (exec grade out of bounds)");
	try
	{
		Form f("BadHighExec", 10, 0); std::cout << f << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}
	try
	{
		Form f("BadLowExec", 10, 151); std::cout << f << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << "\n";
	}

	printHeader("beSigned success and failure");
	try
	{
		Bureaucrat top("Top", 1);
		Bureaucrat mid("Mid", 50);
		Bureaucrat low("Low", 150);

		Form hard("Hard", 10, 10);
		Form easy("Easy", 150, 150);

		std::cout << hard << "\n" << easy << "\n";

		// Success
		hard.beSigned(top);
		std::cout << "After Top signs Hard: " << hard << "\n";

		// Easy form by Low (ok)
		easy.beSigned(low);
		std::cout << "After Low signs Easy: " << easy << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Bureaucrat::signForm messaging");
	{
		Bureaucrat alice("Alice", 20);
		Bureaucrat bob("Bob", 120);

		Form doc1("Doc1", 50, 80);
		Form doc2("Doc2", 120, 120);
		Form doc3("Doc3", 20, 20);

		// Too low
		bob.signForm(doc1);  // should print couldn't sign (too low)

		// Success
		bob.signForm(doc2);  // ok
		bob.signForm(doc2);  // already signed
		bob.signForm(doc2);  // already signed

		// Boundary exact match
		alice.signForm(doc3); // exactly grade required
	}

	printHeader("Copy + assignment behavior");
	try
	{
		Form x("X", 10, 20);
		Form y("Y", 50, 60);

		Bureaucrat t("Top", 1);
		t.signForm(x); // sign X

		std::cout << "x: " << x << "\n";
		std::cout << "y: " << y << "\n";

		Form copy(x);
		std::cout << "copy(x): " << copy << "\n";

		y = x; // copies signed state only
		std::cout << "after y = x: " << y << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	printHeader("Default Form and default Bureaucrat");
	try
	{
		Form defF;
		Bureaucrat defB;
		std::cout << defF << "\n";
		std::cout << defB << "\n";
		defB.signForm(defF); // grade 150 signs sign-grade 150
		std::cout << defF << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected: " << e.what() << "\n";
	}

	return 0;
}

