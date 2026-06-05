/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:37:11 by alafon            #+#    #+#             */
/*   Updated: 2025/06/23 18:16:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HARL_HPP
#define HARL_HPP

#include <string>

/*
** -----------------------------------------------------------------------------
**  Filename: Harl.hpp
** -----------------------------------------------------------------------------
**  Description:
**    Declaration of the Harl class, which provides four levels of log
**    messages (DEBUG, INFO, WARNING, ERROR) and a complain() method that
**    dispatches calls based on a string level.
** -----------------------------------------------------------------------------
*/

class Harl {
private:
	void	debug(void);
	void	info(void);
	void	warning(void);
	void	error(void);
public:
	Harl();
	~Harl(void);
	void	complain(std::string level);
};

#endif
