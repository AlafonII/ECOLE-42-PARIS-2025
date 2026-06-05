/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 17:43:03 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include <iostream>

class Animal {
protected:
	std::string _type;
public:
	Animal(void);
	Animal(std::string const &type);
	Animal(Animal const &src);
	virtual ~Animal(void);

	Animal	&operator=(Animal const &src);

	std::string const	&getType(void) const;
	virtual void		makeSound(void) const;
};

#endif
