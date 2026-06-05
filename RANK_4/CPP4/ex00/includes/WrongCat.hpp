/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:41:09 by alex              #+#    #+#             */
/*   Updated: 2025/06/26 17:42:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONG_CAT_HPP
#define WRONG_CAT_HPP

# include "WrongAnimal.hpp"
#include <iostream>

class WrongCat : public WrongAnimal {
public:
	WrongCat(void);
	WrongCat(WrongCat const & src);
	~WrongCat(void);

	WrongCat &	operator=(WrongCat const & src);

	void	makeSound(void) const;
};

#endif
