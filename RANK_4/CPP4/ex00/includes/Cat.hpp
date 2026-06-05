/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:02:58 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 17:31:34 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include <string>
#include <iostream>
#include "Animal.hpp"

class Cat : public Animal {
public:
	Cat(void);
	Cat(Cat const &src);
	~Cat(void);

	Cat	&operator=(Cat const &src);

	void	makeSound(void) const;
};

#endif
