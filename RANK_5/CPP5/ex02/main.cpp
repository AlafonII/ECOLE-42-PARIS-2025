/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:12:52 by alex              #+#    #+#             */
/*   Updated: 2025/09/02 10:45:04 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/Bureaucrat.hpp"
#include "incl/AForm.hpp"
#include "incl/ShrubberyCreationForm.hpp"
#include "incl/RobotomyRequestForm.hpp"
#include "incl/PresidentialPardonForm.hpp"

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
	printHeader("Construct concrete forms + Bureaucrats");
	Bureaucrat top("Top", 1);
	Bureaucrat exec45("Exec45", 45);
	Bureaucrat mid("Mid", 70);
	Bureaucrat low("Low", 150);

	ShrubberyCreationForm shrub("garden");
	RobotomyRequestForm robo("Bender");
	PresidentialPardonForm pardon("Arthur Dent");

	std::cout << shrub << "\n" << robo << "\n" << pardon << "\n";
	std::cout << top << "\n" << exec45 << "\n" << mid << "\n" << low << "\n";

	printHeader("Signing rules (too low vs ok)");
	try { low.signForm(shrub); } catch (...) {}
	try { exec45.signForm(shrub); } catch (...) {}
	try { mid.signForm(robo); } catch (...) {}
	try { exec45.signForm(robo); } catch (...) {}
	try { top.signForm(pardon); } catch (...) {}

	std::cout << shrub << "\n" << robo << "\n" << pardon << "\n";

	printHeader("Execute without signing (expect throw)");
	try
	{
		ShrubberyCreationForm tmp("nope");
		low.executeForm(tmp);
	}
	catch (...) { std::cout << "Caught by caller (should have been reported above).\n"; }

	printHeader("Execute with insufficient grade (expect throw)");
	try { low.executeForm(shrub); } catch (...) {}
	try { mid.executeForm(pardon); } catch (...) {}

	printHeader("Execute OK: Shrubbery -> file");
	try
	{
		exec45.executeForm(shrub);
		std::string path = "garden_shrubbery";
		std::cout << "file " << path << (fileExists(path) ? " exists.\n" : " missing.\n");
	}
	catch (...) {}

	printHeader("Execute OK: Robotomy (run multiple times)");
	for (int i = 0; i < 5; ++i)
	{
		try { exec45.executeForm(robo); } catch (...) {}
	}

	printHeader("Execute OK: Presidential Pardon");
	try { top.executeForm(pardon); } catch (...) {}

	printHeader("Copy / assign forms (signed state & target)");
	try
	{
		ShrubberyCreationForm a("yard");
		exec45.signForm(a);
		ShrubberyCreationForm b(a);
		std::cout << "copy b of a: " << b << "\n";

		ShrubberyCreationForm c("park");
		c = a;
		std::cout << "after assign c=a: " << c << "\n";
	}
	catch (std::exception& e) { std::cout << "Unexpected: " << e.what() << "\n"; }

	printHeader("Bureaucrat grade boundaries with execute/sign");
	try
	{
		Bureaucrat nearTop("NearTop", 2);
		nearTop.incrementGrade(); // -> 1
		nearTop.signForm(pardon);
		nearTop.executeForm(pardon);
	}
	catch (std::exception& e) { std::cout << "Unexpected: " << e.what() << "\n"; }

	return 0;
}
