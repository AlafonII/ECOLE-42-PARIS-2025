/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isonlyspace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:28:05 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/05 20:34:49 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isonlyspace(char *txt)
{
	while (*txt)
	{
		if (!ft_isspace(*txt))
			return (0);
		txt++;
	}
	return (1);
}
