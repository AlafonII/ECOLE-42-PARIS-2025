/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:12:41 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 17:52:43 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lstmap;
	t_list	*mapnode;

	if (!lst || !f || !del)
		return (0);
	lstmap = ft_lstnew((*f)(lst->content));
	if (!lstmap)
		return (NULL);
	mapnode = lstmap;
	while (lst->next)
	{
		mapnode->next = ft_lstnew((*f)(lst->next->content));
		if (!mapnode->next)
		{
			ft_lstclear(&lstmap, del);
			return (NULL);
		}
		mapnode = mapnode->next;
		lst = lst->next;
	}
	return (lstmap);
}
