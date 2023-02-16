/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 00:19:33 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/02/07 00:19:35 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				philo_count;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				repeat_times;

	struct timeval	time_val;

}					t_data;

typedef struct s_list
{
	int				n_of_philo;
	struct timeval	last_eat;
	struct s_list	*next;
	struct s_list	*prev;
	pthread_mutex_t	forks;
	pthread_t		pth;
	int				number_eat;
	t_data			*data;

}					t_list;

void				ft_putstr(char *s, int fd);
int					ft_atoi_check(char *str, char *msg);
void				ft_lstadd_back(t_list **lst, t_list *new);
t_list				*ft_lstnew(int x, t_data *data);
int	ft_eat_sleep_think(t_list *philo, int *is_died);

#endif