/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:12:52 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:42:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Bureaucrat.hpp"
#include "incl/AForm.hpp"
#include "incl/Intern.hpp"

#include <iostream>
#include <fstream>

static void	printHeader(const std::string& title)
{
	std::cout << "\n=== " << title << " ===\n";
}

static bool fileExists(const std::string& path)
{
	std::ifstream ifs(path.c_str());
	return ifs.good();
}

int main()
{
	printHeader("Setup Intern and Bureaucrats");
	Intern intern;
	Bureaucrat top("Top", 1);
	Bureaucrat exec45("Exec45", 45);
	Bureaucrat mid("Mid", 70);
	Bureaucrat low("Low", 150);

	printHeader("Create valid forms via Intern");
	AForm* f1 = 0;
	AForm* f2 = 0;
	AForm* f3 = 0;
	try { f1 = intern.makeForm("shrubbery creation", "garden"); } catch (std::exception& e) { std::cout << "Create error: " << e.what() << "\n"; }
	try { f2 = intern.makeForm("robotomy request", "Bender"); } catch (std::exception& e) { std::cout << "Create error: " << e.what() << "\n"; }
	try { f3 = intern.makeForm("presidential pardon", "Arthur Dent"); } catch (std::exception& e) { std::cout << "Create error: " << e.what() << "\n"; }

	printHeader("Create invalid form name (expect throw)");
	try { AForm* bad = intern.makeForm("coffee request", "me"); (void)bad; }
	catch (std::exception& e) { std::cout << "Caught: " << e.what() << "\n"; }

	printHeader("Signing via Bureaucrats");
	if (f1) { low.signForm(*f1); exec45.signForm(*f1); }      // low fails, exec45 signs
	if (f2) { mid.signForm(*f2); }                             // mid can sign (72 needed)
	if (f3) { mid.signForm(*f3); top.signForm(*f3); }          // mid fails, top signs

	printHeader("Executing forms");
	if (f1)
	{
		low.executeForm(*f1);          // fail (exec 137)
		exec45.executeForm(*f1);       // ok -> file
		std::cout << "garden_shrubbery " << (fileExists("garden_shrubbery") ? "exists.\n" : "missing.\n");
	}
	if (f2)
	{
		mid.executeForm(*f2);          // fail (exec 45)
		exec45.executeForm(*f2);       // ok, stochastic result
		exec45.executeForm(*f2);
		exec45.executeForm(*f2);
	}
	if (f3)
	{
		mid.executeForm(*f3);          // fail (exec 5)
		top.executeForm(*f3);          // ok
	}

	printHeader("Cleanup");
	delete f1; f1 = 0;
	delete f2; f2 = 0;
	delete f3; f3 = 0;

	printHeader("Edge: Make, sign, execute all in one go");
	try
	{
		AForm* quick = intern.makeForm("shrubbery creation", "yard");
		exec45.signForm(*quick);
		exec45.executeForm(*quick);
		std::cout << "yard_shrubbery " << (fileExists("yard_shrubbery") ? "exists.\n" : "missing.\n");
		delete quick;
	}
	catch (std::exception& e) { std::cout << "Unexpected: " << e.what() << "\n"; }

	return 0;
}
