/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 04:38:29 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/01/15 23:08:36 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

void	ft_putstr(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

int	ft_atoi_check(char *str, char *msg)
{
	int i;
	int sign;
	long long n;

	n = 0;
	i = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if ((str[i] == '+' || str[i] == '-') && str[i] && str[i + 1])
		if (str[i++] == '-')
			sign *= -1;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (printf("\x1b[1;31mError: invalid %s: %s\n", msg, str), -1);
		n = n * 10 + (str[i++] - '0');
		if ((n * sign) > INT32_MAX || (n * sign) < INT32_MIN)
			return (printf("\x1b[1;31mError: invalid %s: %s\n", msg, str), -1);
	}
	if (n * sign <= 0)
		return (printf("\x1b[1;31mError: invalid %s: %s\n", msg, str), -1);
	return (n * sign);
}

t_list	*ft_lstnew(int x,t_data *data)
{
	t_list	*node;
	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->n_of_philo=x;
	gettimeofday(&node->last_eat,NULL);
	node->next = NULL;
	node->prev = NULL;
	pthread_mutex_init(&node->forks,NULL);
	node->number_eat=0;
	node->data=data;
	return (node);
}


void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*node;

	node = *(lst);
	if (!new)
		return ;
	if (*(lst))
	{
		
		while (node->next && node->next!=*(lst))
			node = node->next;
		node->next = new;
		new->prev = node;
		new->next=*(lst);
		(*lst)->prev=new;
	}
	else
		*(lst) = new;
	
}