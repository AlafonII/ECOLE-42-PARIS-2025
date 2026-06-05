/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:20:37 by alex              #+#    #+#             */
/*   Updated: 2025/08/22 18:30:53 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/AForm.hpp"
#include "../incl/Bureaucrat.hpp"

// ----- Exceptions -----
const char* AForm::GradeTooHighException::what() const throw() {
	return "Form grade too high";
}

const char* AForm::GradeTooLowException::what() const throw() {
	return "Form grade too low";
}

const char* AForm::FormNotSignedException::what() const throw() {
	return "Form is not signed";
}

// ----- Utils -----
void AForm::checkGradeBounds(int grade)
{
	if (grade < 1) throw GradeTooHighException();
	if (grade > 150) throw GradeTooLowException();
}

void AForm::checkExecutable(const Bureaucrat& executor) const
{
	if (!_signed) throw FormNotSignedException();
	if (executor.getGrade() > _gradeToExecute) throw GradeTooLowException();
}

// ----- Canonical -----
AForm::AForm() : _name("Unnamed"), _signed(false), _gradeToSign(150), _gradeToExecute(150) {}

AForm::AForm(const std::string& name, int gSign, int gExec)
	: _name(name), _signed(false), _gradeToSign(gSign), _gradeToExecute(gExec)
{
	checkGradeBounds(_gradeToSign);
	checkGradeBounds(_gradeToExecute);
}
AForm::AForm(const AForm& other)
	: _name(other._name), _signed(other._signed),
	  _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute) {}

AForm& AForm::operator=(const AForm& other)
{
	if (this != &other) _signed = other._signed;
	return *this;
}
AForm::~AForm() {}

// ----- Getters -----
const std::string& AForm::getName() const { return _name; }
bool AForm::isSigned() const { return _signed; }
int AForm::getGradeToSign() const { return _gradeToSign; }
int AForm::getGradeToExecute() const { return _gradeToExecute; }

// ----- Ops -----
void AForm::beSigned(const Bureaucrat& b)
{
	if (b.getGrade() > _gradeToSign) throw GradeTooLowException();
	_signed = true;
}

std::ostream& operator<<(std::ostream& os, const AForm& f)
{
	os << "Form \"" << f.getName() << "\" [signed: " << (f.isSigned() ? "yes" : "no")
	   << "] (sign: " << f.getGradeToSign() << ", exec: " << f.getGradeToExecute() << ")";
	return os;
}
