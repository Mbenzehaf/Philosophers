/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 13:15:29 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/02/15 13:15:32 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	ft_time_ms(struct timeval time_val, int time_to)
{
	struct timeval	time_now;
    int total;

	gettimeofday(&time_now, NULL);
    if(!time_to){
	return ((int)((time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
			- (time_val.tv_sec * 1000 + time_val.tv_usec / 1000)));
    }else
    {
         total = ((int)((time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
			- (time_val.tv_sec * 1000 + time_val.tv_usec / 1000)) % time_to);
        if(total < 0 )
            return (0);
        else 
            return (total);
        
    }
}

int ft_check_is_died(t_list *philo,int *is_died,int time_to)
{
    if(*is_died || philo->number_eat == philo->data->repeat_times)
    {
        return (1);
    }
    else if((ft_time_ms(philo->last_eat,0) - ft_time_ms(philo->last_eat,time_to) > philo->data->die_time) || !philo->next)
    {
        *is_died = 1;
        printf("[%0.8i] %0.3i died\n",ft_time_ms(philo->data->time_val,0) - ft_time_ms(philo->data->time_val,time_to) ,philo->n_of_philo);
       // printf("[%0.8i] %0.3i died\n",var - (var % philo->data->eat_time),philo->n_of_philo);
        return (1);
    }
    return (0);
}
void ft_usleep(t_list *philo,int *is_died,int time_to,int timer)
{
	if(philo->data->die_time < timer)
	{
		if(time_to > philo->data->die_time)
		{
			usleep(philo->data->die_time * 1000);
		}else 
		{
			usleep((timer - philo->data->die_time ) * 1000);
			*is_died = 1;
        	printf("[%0.8i] %0.3i died :)\n",ft_time_ms(philo->data->time_val,0) - ft_time_ms(philo->data->time_val,philo->data->die_time) ,philo->n_of_philo);
		}
	}
	else
	{
		usleep(time_to * 1000);
	}
}

int	ft_eat_sleep_think(t_list *philo,int *is_died)
{
    if(ft_check_is_died(philo,is_died,philo->data->eat_time))
            return (1);
    pthread_mutex_lock(&philo->forks);
    pthread_mutex_lock(&philo->next->forks);
    printf("[%0.8i] %0.3i has taken a fork\n",ft_time_ms(philo->data->time_val,0) - ft_time_ms(philo->data->time_val,philo->data->sleep_time) ,philo->n_of_philo);
    printf("[%0.8i] %0.3i is eating\n",ft_time_ms(philo->data->time_val,0)- ft_time_ms(philo->data->time_val,philo->data->sleep_time),philo->n_of_philo);
    gettimeofday(&philo->last_eat,NULL);
     if(ft_check_is_died(philo,is_died,philo->data->sleep_time))
            {
                   pthread_mutex_unlock(&philo->forks);
                   pthread_mutex_unlock(&philo->next->forks);
                return (1);
            }
   // ft_usleep(philo,is_died,philo->data->eat_time,philo->data->eat_time);
   usleep(philo->data->eat_time);
	philo->number_eat++;
    pthread_mutex_unlock(&philo->forks);
    pthread_mutex_unlock(&philo->next->forks);
    if(ft_check_is_died(philo,is_died,philo->data->eat_time))
        return (1);
    printf("[%0.8i] %0.3i is sleeping\n",ft_time_ms(philo->data->time_val,0) - ft_time_ms(philo->data->time_val,philo->data->eat_time), philo->n_of_philo);
   //ft_usleep(philo,is_died,philo->data->sleep_time,philo->data->eat_time + philo->data->sleep_time);
   usleep(philo->data->sleep_time);
     if(ft_check_is_died(philo,is_died,philo->data->sleep_time))
        return (1);
     printf("[%0.8i] %0.3i is thinking\n",ft_time_ms(philo->data->time_val,0) - ft_time_ms(philo->data->time_val,philo->data->eat_time),philo->n_of_philo);
    return (0);
}
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
/*void	*routine(void *philos)
{
	t_list		*philo;
	static int	is_died;

	philo = (t_list *)philos;
	if(philo->n_of_philo % 2 == 0)
		usleep(philo->data->eat_time * 1000);
	else if(!philo->next)
			usleep(philo->data->die_time * 1000);
	while (!is_died)
	{
		if(!is_died)
			ft_eat_sleep_think(philo,&is_died);
		else
			return (NULL);
	}
	return (NULL);
}*/
/*size_t time_to_ms(struct timeval time_val)
{
	return ;
}
size_t	ft_timenow_ms(void)
{
	struct timeval time_now;
	gettimeofday(&time_now,NULL);
	return (time_to_ms(time_now));
}*/