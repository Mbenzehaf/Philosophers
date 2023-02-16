/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 00:13:44 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/02/07 00:13:46 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	*routine(void *philos)
{
	t_list		*philo;
	static int	is_died;

	philo = (t_list *)philos;
	if(!philo->next)
        usleep(philo->data->die_time * 1000);
    if (philo->n_of_philo % 2 == 0)
		usleep(philo->data->eat_time * 1000);
    
	while (!is_died){
		if (ft_eat_sleep_think(philo, &is_died))
			    break ;
    }
	return (NULL);
}

int	ft_philo(t_list *philo)
{
	int		i;
	t_data	*data;

	i = 0;
	data = philo->data;
	while (i < data->philo_count)
	{
		if (pthread_create(&philo->pth, NULL, &routine, (void *)philo))
			return (1);
		if (philo->next)
			philo = philo->next;
		i++;
	}
	i = 0;
	//printf("----->%i\n",philo->n_of_philo);
	while (i < data->philo_count)
	{
		if (pthread_join(philo->pth,NULL))
			return (1);
		//pthread_mutex_destroy(&philo->forks);
		philo = philo->next;
		i++;
	}
	return (0);
}

int	ft_initializ(t_data *data, char **av, int ac)
{
	data->philo_count = ft_atoi_check(av[1], "number_of_philosophers");
	if (data->philo_count == -1)
		return (1);
	data->die_time = ft_atoi_check(av[2], "time_to_die");
	if (data->die_time == -1)
		return (1);
	data->eat_time = ft_atoi_check(av[3], "time_to_eat");
	if (data->eat_time == -1)
		return (1);
	data->sleep_time = ft_atoi_check(av[4], "time_to_sleep");
	if (data->sleep_time == -1)
		return (1);
	if (ac == 6){
		data->repeat_times = ft_atoi_check(av[5],
											"number_of_times_each_philosopher_must_eat");
	if (data->repeat_times == -1)
		return (1);}
	else
		data->repeat_times = -1;
	gettimeofday(&data->time_val, NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	t_data	data;
	t_list	*philo;
	int		i;
	int		error;

	i = 0;
	philo = NULL;
	if (ac == 5 || ac == 6)
	{
		error = ft_initializ(&data, av, ac);
		if (error)
		{
			return (error);
		}
		while (i < data.philo_count)
			ft_lstadd_back(&philo, ft_lstnew(++i, &data));
		ft_philo(philo);
	}
	else
		(printf("\x1b[1;7;33m[invalid number of arguments]\n"), exit(1), 0);
	return (0);
}