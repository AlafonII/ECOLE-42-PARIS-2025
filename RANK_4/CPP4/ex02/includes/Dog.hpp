/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:02:50 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 19:30:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include <string>
#include <iostream>
#include "AAnimal.hpp"
#include "Brain.hpp"

class Dog : public AAnimal {
private:
	Brain	*_brain;
public:
	Dog(void);
	Dog(Dog const &src);
	~Dog(void);

	Dog	&operator=(Dog const &src);

	Brain	*getBrain(void) const;
	void	makeSound(void) const;
};

#endif
