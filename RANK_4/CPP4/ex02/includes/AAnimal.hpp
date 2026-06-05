/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAnimal.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:48 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 19:44:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AANIMAL_HPP
#define AANIMAL_HPP

#include <string>
#include <iostream>

class AAnimal {
protected:
	std::string _type;
public:
	AAnimal(void);
	AAnimal(AAnimal const &src);
	virtual ~AAnimal(void);

	AAnimal	&operator=(AAnimal const &src);

	std::string const	&getType(void) const;
	virtual void		makeSound(void) const = 0;
};

#endif
