/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:00:52 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 19:41:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Brain.hpp"

/*
 * -----------------------------------------------------------
 * 						CONSTRUCTOR
 * -----------------------------------------------------------
 */

// Default: use default stats
Brain::Brain(void) {
		std::cout << "Brain default constructor called" << std::endl;
}

// Copy: use assignment operator to clone all fields
Brain::Brain(Brain const &src) {
	std::cout << "Brain copy constructor called" << std::endl;
	*this = src;
}

// Assignment: clone fields if not self
Brain &	Brain::operator=(Brain const &src) {
	std::cout << "Brain assignment operator called" << std::endl;
	for (int i = 0; i < this->ideaNum; i++)
		this->_ideas[i] = src._ideas[i];
	return (*this);
}

/*
 * -----------------------------------------------------------
 * 						GETTERS
 * -----------------------------------------------------------
 */

std::string const	&Brain::getIdea(int idx) const {
	if (idx < 0)
		idx = 0;
	return (this->_ideas[idx % ideaNum]);
}

/*
 * -----------------------------------------------------------
 * 						SETTERS
 * -----------------------------------------------------------
 */

void	Brain::setIdea(std::string const &idea, int idx) {
	if (idx < 0)
		return ;
	this->_ideas[idx % ideaNum] = idea;
}

/*
 * -----------------------------------------------------------
 * 						DECONSTRUCTOR
 * -----------------------------------------------------------
 */

Brain::~Brain(void) {
	std::cout << "Brain destructor called"<< std::endl;
}
