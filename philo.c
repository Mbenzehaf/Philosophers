/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 00:14:35 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/02/07 00:14:37 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

size_t time_to_ms(struct timeval time_val)
{
	return ((size_t)(time_val.tv_sec * 1000 + time_val.tv_usec / 1000));
}
size_t ft_timenow_ms()
{
	struct timeval time_now;
	gettimeofday(&time_now,NULL);
	return (time_to_ms(time_now));
}

void	ft_sleep(t_list *philo,int *is_died)
{
	int var;

	var =(int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
	if(*is_died)
	{
		return ;
	}else if(var - (var % philo->data->eat_time) >= philo->data->die_time)
	{
		*is_died = 1;
		printf("[%0.8i] %0.3i died\n",var - (var % philo->data->eat_time),philo->n_of_philo);
		return ;
	}
	printf("[%0.8i] %0.3i is sleeping\n",var - (var % philo->data->eat_time) ,philo->n_of_philo);
	usleep(philo->data->sleep_time * 1000);
		var =(int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
	if(*is_died)
		return ;
	if(var - (var % philo->data->sleep_time) >= philo->data->die_time)
	{
		*is_died = 1;
		printf("[%0.8i] %0.3i died\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
		return ;
	}
	printf("[%0.8i] %0.3i is thinking\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
}

void	ft_eat(t_list *philo,int *is_died)
{
	int var ;
	if(*is_died)
		return ;
	var =(int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
	if(var - (var % philo->data->eat_time) >= philo->data->die_time)
	{
		*is_died = 1;
		printf("[%0.8i] %0.3i died\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
		return ;
	}
	pthread_mutex_lock(&philo->forks);
	if(*is_died)
		return ;
	var = (int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
		if(var - (var % philo->data->eat_time) < philo->data->die_time && !*is_died)
			printf("[%0.8i] %0.3i has taken a fork\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
		else
			{
				*is_died = 1;
				return ;
			}
	pthread_mutex_lock(&philo->next->forks);
	if(*is_died)
		return ; 
		var = (int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
	if((var - (var % philo->data->eat_time))>= philo->data->die_time)
	{
		*is_died = 1;
		printf("[%0.8i] %0.3i died\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
	pthread_mutex_unlock(&philo->forks);
	pthread_mutex_unlock(&philo->next->forks);
		return ;
	}
	var = (int )(ft_timenow_ms() - time_to_ms(philo->last_eat));
	printf("[%0.8i] %0.3i is eating\n",(int )(ft_timenow_ms() - time_to_ms(philo->data->time_val)),philo->n_of_philo);
	gettimeofday(&philo->last_eat,NULL);
	usleep(philo->data->eat_time*1000);
	pthread_mutex_unlock(&philo->forks);
	pthread_mutex_unlock(&philo->next->forks);
	ft_sleep(philo,is_died);
}
/*
void ft_eat(t_list *philo)
{
	pthread_mutex_lock(&philo->next->forks);
	if((int )(ft_timenow_ms() - time_to_ms(philo->last_eat))>= philo->data->die_time)
	{
		printf("[%0.8i] %0.3i died:)\n",(int )(ft_timenow_ms()-time_to_ms(philo->data->time_val)),philo->n_of_philo);
		exit(1);
	}
	pthread_mutex_lock(&philo->forks);
	printf("[%0.8i] %0.3i eat:)\n",(int )(ft_timenow_ms()-time_to_ms(philo->data->time_val)),philo->n_of_philo);
	usleep(philo->data->eat_time * 1000);
	pthread_mutex_lock(&philo->forks);
	pthread_mutex_lock(&philo->next->forks);
}*/

void	*routine(void *philos)
{
	t_list *philo;
	static int is_died;

	philo = (t_list *)philos;

	if(philo->n_of_philo % 2 == 0)
		usleep(philo->data->eat_time * 1000);
	else if(!philo->next)
			usleep(philo->data->die_time * 1000);
	while (!is_died)
	{
		if(!is_died)
			ft_eat(philo,&is_died);
		else
			return NULL;
		
	}
	return (NULL);
}

/*int  ft_philo(t_list *philo)
{
	int i;
	t_data *data;
	
	i = 0;
	data = philo->data;
	
	while (i < data->philo_count )
	{
		if(pthread_create(&philo->pth,NULL,&routine,(void *)philo))
			return 1;
		if(philo->next)
			philo = philo->next;
		i++;
	}
	i = 0;
	//printf("----->%i\n",philo->n_of_philo);
	while (i < data->philo_count)
	{
		
		if(pthread_join(philo->pth,NULL))
			return 1;
	
		//pthread_mutex_destroy(&philo->forks);
		philo = philo->next;
		i++;
	}
	
	return (0);	
}*/

int	ft_philo(t_list *philo)
{
	int		i;
	t_data	*data;
	t_list	**philo_tab;


	i = 0;
	data = philo->data;
	philo_tab = malloc(sizeof(t_list *) * (data->philo_count + 1));
	while (i < data->philo_count)
	{
		philo_tab[i] = philo;
		philo = philo->next;
		i++;
	}
	philo_tab[i] = NULL;
	i = 0;
	while (philo_tab[i])
	{
		pthread_create(&philo_tab[i]->pth,NULL,routine,(void *)philo_tab[i]);
		i++;	
	}
	i=0;
	while (philo_tab[i])
	{
		pthread_join(philo_tab[i]->pth,NULL);
		i++;	
	}
	return(0);
}

int ft_initializ(t_data *data,char **av,int ac)
{
	data->philo_count = ft_atoi_check(av[1], "number_of_philosophers");
	if(data->philo_count==-1)
		return (1);
	data->die_time = ft_atoi_check(av[2], "time_to_die");
	if(data->die_time==-1)
		return (1);
	data->eat_time = ft_atoi_check(av[3], "time_to_eat");
	if(data->eat_time==-1)
		return (1);
	data->sleep_time = ft_atoi_check(av[4], "time_to_sleep");
	if(data->sleep_time==-1)
		return (1);
	if (ac == 6)
			data->repeat_times = ft_atoi_check(av[5],
								"number_of_times_each_philosopher_must_eat");
		if(data->repeat_times==-1)
			return (1);
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
	int error;

	i = 0;
	philo = NULL;
	if (ac == 5 || ac == 6)
	{
		error=ft_initializ(&data,av,ac);
		if(error)
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
