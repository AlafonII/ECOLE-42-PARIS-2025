/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intern.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:41:20 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:41:31 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERN_HPP
#define INTERN_HPP

#include <string>
#include <exception>

class AForm;

class Intern
{
public:
	// Canonical
	Intern();
	Intern(const Intern& other);
	Intern& operator=(const Intern& other);
	~Intern();

	// Factory
	AForm*	makeForm(const std::string& name, const std::string& target) const;

	// Exception
	class UnknownFormException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};
};

#endif
