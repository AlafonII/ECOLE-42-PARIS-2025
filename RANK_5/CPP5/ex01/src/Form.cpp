/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Form.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:20:37 by alex              #+#    #+#             */
/*   Updated: 2025/09/02 09:46:13 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/Form.hpp"
#include "../incl/Bureaucrat.hpp"

// ----- Exceptions -----
const char* Form::GradeTooHighException::what() const throw()
{
	return "Form grade too high";
}

const char* Form::GradeTooLowException::what() const throw()
{
	return "Form grade too low";
}

// ----- Utils -----
void Form::checkGradeBounds(int grade)
{
	if (grade < 1)
		throw GradeTooHighException();
	if (grade > 150)
		throw GradeTooLowException();
}

// ----- Canonical form -----
Form::Form() : _name("Unnamed"), _signed(false), _gradeToSign(150), _gradeToExecute(150) {}

Form::Form(const std::string& name, int gradeToSign, int gradeToExecute)
	: _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	checkGradeBounds(_gradeToSign);
	checkGradeBounds(_gradeToExecute);
}

Form::Form(const Form& other)
	: _name(other._name), _signed(other._signed),
	  _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute) {}

Form& Form::operator=(const Form& other)
{
	// const members remain
	if (this != &other)
	{
		_signed = other._signed;
	}
	return *this;
}

Form::~Form() {}

// ----- Getters -----
const std::string& Form::getName() const
{
	return _name;
}

bool Form::isSigned() const
{
	return _signed;
}

int Form::getGradeToSign() const
{
	return _gradeToSign;
}

int Form::getGradeToExecute() const
{
	return _gradeToExecute;
}

// ----- Ops -----
void Form::beSigned(const Bureaucrat& b)
{
	// Grade too low (high) to sign
	if (b.getGrade() > _gradeToSign)
		throw GradeTooLowException();
	_signed = true;
}

std::ostream& operator<<(std::ostream& os, const Form& f)
{
	os << "Form \"" << f.getName() << "\" [signed: "
	   << (f.isSigned() ? "yes" : "no")
	   << "] (sign: " << f.getGradeToSign()
	   << ", exec: " << f.getGradeToExecute() << ")";
	return os;
}
