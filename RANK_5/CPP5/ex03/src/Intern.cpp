/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intern.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:41:42 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:42:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/Intern.hpp"
#include "../incl/AForm.hpp"
#include "../incl/ShrubberyCreationForm.hpp"
#include "../incl/RobotomyRequestForm.hpp"
#include "../incl/PresidentialPardonForm.hpp"
#include <iostream>

// Canonical
Intern::Intern() {}

Intern::Intern(const Intern& other) {
	(void)other;
}

Intern& Intern::operator=(const Intern& other) {
	(void)other; return *this;
}

Intern::~Intern() {}

const char* Intern::UnknownFormException::what() const throw() {
	return "Unknown form name";
}

// Helpers
static AForm*	createShrub(const std::string& t) {
	return new ShrubberyCreationForm(t);
}

static AForm*	createRobo(const std::string& t) {
	return new RobotomyRequestForm(t);
}

static AForm*	createPardon(const std::string& t) {
	return new PresidentialPardonForm(t);
}

AForm* Intern::makeForm(const std::string& name, const std::string& target) const
{
	const std::string	names[3] = { "shrubbery creation", "robotomy request", "presidential pardon" };
	AForm* (*ctors[3])(const std::string&) = { &createShrub, &createRobo, &createPardon };

	for (int i = 0; i < 3; ++i)
	{
		if (name == names[i])
		{
			AForm* f = ctors[i](target);
			std::cout << "Intern creates " << f->getName() << " for target \"" << target << "\".\n";
			return f;
		}
	}
	throw UnknownFormException();
}
