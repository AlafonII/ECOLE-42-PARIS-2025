/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bureaucrat.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:08:22 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:18:53 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>
#include <exception>

class Form;

class Bureaucrat
{
private:
	const std::string	_name;
	int					_grade;

public:
	// Exceptions
	class GradeTooHighException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

	class GradeTooLowException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

	// Canonical form
	Bureaucrat(); // default to "Unnamed", grade 150
	Bureaucrat(const std::string& name, int grade);
	Bureaucrat(const Bureaucrat& other);
	Bureaucrat& operator=(const Bureaucrat& other);
	~Bureaucrat();

	// Getters
	const std::string&	getName() const;
	int					getGrade() const;

	// Grade ops (1 = highest, 150 = lowest)
	void				incrementGrade(); // grade--
	void				decrementGrade(); // grade++

	void				signForm(Form& form) const;
};

// Stream output
std::ostream& operator<<(std::ostream& os, const Bureaucrat& b);

#endif
