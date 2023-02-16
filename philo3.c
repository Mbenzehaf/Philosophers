#include "philo.h"

int	ft_time_ms(struct timeval time_val, int time_to)
{
	struct timeval	time_now;
	int				total;

	gettimeofday(&time_now, NULL);
	if (!time_to)
	{
		return ((int)((time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
				- (time_val.tv_sec * 1000 + time_val.tv_usec / 1000)));
	}
	else
	{
		total = ((int)((time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
					- (time_val.tv_sec * 1000 + time_val.tv_usec / 1000))
				% time_to);
		if (total < 0)
			return (ft_time_ms(time_val, 0));
		else
			return (ft_time_ms(time_val, 0) - total);
	}
}

void	msg(t_list *philo, int time_to, int i)
{
	if (i == 0)
		printf("[%0.8i] %0.3i has taken a fork\n",
				ft_time_ms(philo->data->time_val, philo->data->sleep_time),
				philo->n_of_philo);
	else if (i == 1)
		printf("[%0.8i] %0.3i is eating\n", ft_time_ms(philo->data->time_val,
					philo->data->sleep_time), philo->n_of_philo);
	else if (i == 2)
		printf("[%0.8i] %0.3i is sleeping\n", ft_time_ms(philo->data->time_val,
					philo->data->eat_time), philo->n_of_philo);
	else if (i == 3)
		printf("[%0.8i] %0.3i is thinking\n", ft_time_ms(philo->data->time_val,
					philo->data->eat_time), philo->n_of_philo);
	else
		printf("[%0.8i] %0.3i died\n", ft_time_ms(philo->data->time_val,
					time_to), philo->n_of_philo);
}

int	ft_check_is_died(t_list *philo, int *is_died, int time_to)
{
	if (*is_died || philo->number_eat == philo->data->repeat_times)
		return (1);
	else if ((ft_time_ms(philo->last_eat, time_to) > philo->data->die_time) || !philo->next)
	{
		*is_died = 1;
        msg(philo,time_to,4);
		return (1);
	}
	return (0);
}

void	ft_usleep(t_list *philo, int *is_died, int time_to)
{
	/*if (philo->data->die_time < timer)
	{
		if (time_to > philo->data->die_time)
		{
			usleep(philo->data->die_time * 1000);
		}
		else
		{
			usleep((timer - philo->data->die_time) * 1000);
			*is_died = 1;
			printf("[%0.8i] %0.3i died :)\n", ft_time_ms(philo->data->time_val,
						philo->data->die_time), philo->n_of_philo);
		}
	}
	else
	{
		usleep(time_to * 1000);
	}*/
    if(ft_time_ms(philo->last_eat,0) + time_to > philo->data->die_time)
    {
        usleep(philo->data->die_time - ft_time_ms(philo->last_eat,0) * 1000);
        msg(philo,time_to,4);
        *is_died = 1;
    }
    else
    {
        usleep(time_to * 1000);
    } 

}

int	ft_eat_sleep_think(t_list *philo, int *is_died)
{
	pthread_mutex_lock(&philo->forks);
	pthread_mutex_lock(&philo->next->forks);
    msg(philo,0,0);
    msg(philo,0,1);
    gettimeofday(&philo->last_eat,NULL);
	ft_usleep(philo,is_died,philo->data->eat_time);
	pthread_mutex_unlock(&philo->forks);
	pthread_mutex_unlock(&philo->next->forks);
    msg(philo,0,2);
   ft_usleep(philo,is_died,philo->data->sleep_time);
    msg(philo,0,3);
	//return (ft_sleep(philo, is_died,));
    return (0);
}

/*int	ft_eat_sleep_think(t_list *philo,int *is_died)
{
   if(ft_check_is_died(philo,is_died,philo->data->eat_time))
            return (1);
    pthread_mutex_lock(&philo->forks);
    pthread_mutex_lock(&philo->next->forks);
    printf("[%0.8i]
		%0.3i has taken a fork\n",ft_time_ms(philo->data->time_val,0)
		- ft_time_ms(philo->data->time_val,philo->data->sleep_time) ,philo->n_of_philo);
    printf("[%0.8i]
		%0.3i is eating\n",ft_time_ms(philo->data->time_val,0)- ft_time_ms(philo->data->time_val,philo->data->sleep_time),philo->n_of_philo);
   
     if(ft_check_is_died(philo,is_died,philo->data->sleep_time))
            {
                   pthread_mutex_unlock(&philo->forks);
                   pthread_mutex_unlock(&philo->next->forks);
                	return (1);
            }
		gettimeofday(&philo->last_eat,NULL);
   // ft_usleep(philo,is_died,philo->data->eat_time,philo->data->eat_time);
   usleep(philo->data->eat_time * 1000);
	philo->number_eat++;
    pthread_mutex_unlock(&philo->forks);
    pthread_mutex_unlock(&philo->next->forks);
    if(ft_check_is_died(philo,is_died,philo->data->eat_time))
        return (1);
    printf("[%0.8i] %0.3i is sleeping\n",ft_time_ms(philo->data->time_val,0)
		- ft_time_ms(philo->data->time_val,philo->data->eat_time),
		philo->n_of_philo);
   //ft_usleep(philo,is_died,philo->data->sleep_time,philo->data->eat_time
		+ philo->data->sleep_time);
   usleep(philo->data->sleep_time * 1000);
     if(ft_check_is_died(philo,is_died,philo->data->sleep_time))
        return (1);
     printf("[%0.8i] %0.3i is thinking\n",ft_time_ms(philo->data->time_val,0)
		- ft_time_ms(philo->data->time_val,philo->data->eat_time),philo->n_of_philo);
    return (0);
}*/