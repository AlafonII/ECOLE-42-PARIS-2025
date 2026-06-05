/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:02:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 18:24:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include <string>
#include <iostream>
#include "Animal.hpp"
#include "Brain.hpp"

class Cat : public Animal {
private:
	Brain	*_brain;
public:
	Cat(void);
	Cat(Cat const &src);
	~Cat(void);

	Cat	&operator=(Cat const &src);

	Brain	*getBrain(void) const;
	void	makeSound(void) const;
};

#endif
