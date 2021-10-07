/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ahma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 16:04:54 by oel-ahma          #+#    #+#             */
/*   Updated: 2021/10/06 16:04:55 by oel-ahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_ur_forks_and_eat(t_philo *philo_data, int right)
{
    struct timeval tv;
    long int time;

    pthread_mutex_lock(&philo_data->mutex->forks[philo_data->id]);
    print_status(philo_data, "took the 1st fork!", 0);
    pthread_mutex_lock(&philo_data->mutex->forks[right]);
    print_status(philo_data, "took the 2nd fork!", 0);
    pthread_mutex_lock(&philo_data->is_eating);
    print_status(philo_data, "is eating!", 0);
    if (philo_data->optional_param == 1)
    {
        pthread_mutex_lock(&philo_data->mutex->meals_services);
        philo_data->meals_to_eat--;
        pthread_mutex_unlock(&philo_data->mutex->meals_services);
    }
    gettimeofday(&philo_data->last_meal, NULL);
    ft_sleep(philo_data->time_to_eat * 1000);
    pthread_mutex_unlock(&philo_data->mutex->forks[philo_data->id]);
    pthread_mutex_unlock(&philo_data->mutex->forks[right]);
    pthread_mutex_unlock(&philo_data->is_eating);
}

int	life_is_taken(long int time, t_philo *philo_data)
{
	if ((time > philo_data->time_to_die && philo_data->meals_to_eat != 0)
		|| time < 0)
	{
		print_status(philo_data, "is DEAD! :(", 1);
		pthread_mutex_lock(&(philo_data->mutex->someone_died));
		philo_data->mutex->nobody_dead = 0;
		pthread_mutex_unlock(&(philo_data->mutex->someone_died));
		return (1);
	}
	pthread_mutex_lock(&(philo_data->mutex->meals_services));
	if (philo_data->meals_to_eat == 0)
		return (1);
	pthread_mutex_unlock(&(philo_data->mutex->meals_services));
	return (0);
}

int	check_if_alive(t_philo *philo_data)
{
	pthread_mutex_lock(&(philo_data->mutex->someone_died));
	if (!philo_data->mutex->nobody_dead || !philo_data->meals_to_eat)
	{
		pthread_mutex_unlock(&(philo_data->mutex->someone_died));
		return (0);
	}
	pthread_mutex_unlock(&(philo_data->mutex->someone_died));
	return (1);
}

void	*start_check(void *param)
{
	struct timeval	timeofnow;
	long int		time;
	t_philo			*philo_data;

	philo_data = (t_philo *)param;
	if (philo_data->philo_nbr == 1)
	{
		print_status(philo_data, "is DEAD! :(", 1);
		return (param);
	}
	while (1)
	{
		usleep(50);
		pthread_mutex_lock(&(philo_data->is_eating));
		gettimeofday(&timeofnow, NULL);
		time = (timeofnow.tv_sec - philo_data->last_meal.tv_sec)
			* 1000 + (timeofnow.tv_usec - philo_data->last_meal.tv_usec) / 1000;
        if (life_is_taken(time, philo_data))
			break ;
		pthread_mutex_unlock(&(philo_data->is_eating));
	}
	pthread_mutex_unlock(&(philo_data->is_eating));
	return (param);
}

void *dinner(void *param)
{
    pthread_t	dead;
    t_philo *philo_data;
    int right;
    int check;

    philo_data = (t_philo *)param;
    right = (philo_data->id + 1) % philo_data->philo_nbr;
    check = pthread_create(&dead, NULL, &start_check, (void *)philo_data);
    if (check)
		philo_data->mutex->nobody_dead = 0;
    while (!check && philo_data->philo_nbr != 1)
    {
        print_status(philo_data, "is thinking!", 0);
        take_ur_forks_and_eat(philo_data, right);
        print_status(philo_data, "is sleeping!", 0);
        ft_sleep(philo_data->time_to_sleep * 1000);
        if (!check_if_alive(philo_data))
            break;
    }
    pthread_join(dead, NULL);
    return (NULL);
}