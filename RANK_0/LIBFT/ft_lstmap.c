/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:13:58 by alafon            #+#    #+#             */
/*   Updated: 2024/11/19 18:50:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_l;
	t_list	*new_o;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_l = NULL;
	while (lst)
	{
		new_o = ft_lstnew(f(lst->content));
		if (!new_o)
		{
			ft_lstclear(&new_l, del);
			return (NULL);
		}
		ft_lstadd_back(&new_l, new_o);
		lst = lst->next;
	}
	return (new_l);
}
