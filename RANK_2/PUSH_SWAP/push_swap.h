/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:19:58 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 13:05:32 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# ifndef CHUNK_SIZE
#  define CHUNK_SIZE 5
# endif

# include <stdbool.h>
# include <limits.h>
# include "libft/libft.h"
# include <stdlib.h>

//----------------struct-------------//

typedef struct s_data
{
	int		n;
	int		chunk_cnt;
	int		chunk_sz;
	int		total_pushed;
	int		*sorted;
}		t_data;

typedef struct s_chunk
{
	int		pivot;
	int		need;
}		t_chunk;

typedef struct s_stack
{
	int		*arr;
	int		size;
}		t_stack;

//----------------swap----------------//

void	sa(t_stack *a);
void	sb(t_stack *b);
void	ss(t_stack *a, t_stack *b);

//----------------push----------------//

void	pa(t_stack *a, t_stack *b);
void	pb(t_stack *a, t_stack *b);

//---------------rotate---------------//

void	ra(t_stack *a);
void	rb(t_stack *b);
void	rr(t_stack *a, t_stack *b);

//-----------reverse_rotate-----------//

void	rra(t_stack *a);
void	rrb(t_stack *b);
void	rrr(t_stack *a, t_stack *b);

//------------stack_maker-------------//

void	free_stack(char **arr);
void	stack_create(int argc, char **argv, t_stack *a);
int		stack_size(int argc, char **argv);

//--------------sorting---------------//

void	small_sort(t_stack *a, t_stack *b);
void	sort_array(int *arr, int n);
int		*create_sorted_copy(t_stack *a);
int		compute_chunk_count(int n);
int		compute_chunk_size(int n, int chunk_count);
int		chunk_need(int i, t_data d);
void	push_chunk_to_b(t_stack *a, t_stack *b, t_chunk c);
int		find_max_index(t_stack *b);
void	push_all_back_to_a(t_stack *a, t_stack *b);
void	process_chunks(t_data *d, t_stack *a, t_stack *b);
void	turk_sort(t_stack *a, t_stack *b);

//--------------sorting_utils---------//

int		find_min_index(t_stack *a);
void	push_min_to_b(t_stack *a, t_stack *b);
void	sort_three(t_stack *a);
int		is_sorted(t_stack *a);

//------------error_check-------------//

int		ft_is_int(const char *str);
int		ft_in_int_range(const char *str);
int		ft_has_duplicate(int *arr, int size);
int		count_args(int argc, char **argv);
int		fill_array(int argc, char **argv, int *arr);
int		ft_check_double_strings(int argc, char **argv);
int		ft_error_checker(int argc, char **argv);

#endif