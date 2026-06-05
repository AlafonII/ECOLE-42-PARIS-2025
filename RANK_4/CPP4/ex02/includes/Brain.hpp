/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:00:46 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 18:20:37 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <string>
#include <iostream>

class Brain {
public:
	Brain(void);
	Brain(Brain const & src);
	~Brain(void);

	static int const	ideaNum = 100;

	Brain &	operator=(Brain const &src);

	std::string const	&getIdea(int idx) const;
	void				setIdea(std::string const & idea, int idx);
private:
	std::string	_ideas[ideaNum];
};

#endif