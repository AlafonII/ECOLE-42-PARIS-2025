/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:40:01 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 17:42:36 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONG_ANIMAL_HPP
#define WRONG_ANIMAL_HPP

#include <string>
#include <iostream>

class WrongAnimal {
protected:
	std::string	_type;
public:
	WrongAnimal(void);
	WrongAnimal(WrongAnimal const & src);
	WrongAnimal(std::string const & type);
	virtual ~WrongAnimal(void);

	WrongAnimal &	operator=(WrongAnimal const & src);

	std::string const 	&getType(void) const;
	void				makeSound(void) const;
};

#endif
