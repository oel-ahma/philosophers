/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ahma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 16:05:09 by oel-ahma          #+#    #+#             */
/*   Updated: 2021/10/11 18:51:34 by oel-ahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_threads(int philo_nbr, t_philo *philo_data, pthread_t **philo)
{
	int	i;

	*philo = malloc(sizeof(pthread_t) * philo_nbr);
	if (!(*philo))
		return (0);
	i = 0;
	while (i < philo_nbr)
	{
		usleep(50);
		if (pthread_create(&(*philo)[i], NULL, &dinner, (void *)&philo_data[i]))
			break ;
		i++;
	}
	i = 0;
	while (i < philo_nbr)
		pthread_join((*philo)[i++], NULL);
	i = 0;
	while (i < philo_nbr)
		pthread_mutex_destroy(&(philo_data->mutex->forks[i++]));
	pthread_mutex_destroy(&philo_data->mutex->someone_died);
	pthread_mutex_destroy(&philo_data->mutex->print);
	pthread_mutex_destroy(&philo_data->is_eating);
	pthread_mutex_destroy(&philo_data->mutex->meals_services);
	free(*philo);
	return (0);
}

int	get_values(t_philo **philo_data, struct timeval tv_start,
		int i, char **av)
{
	if (pthread_mutex_init(&((*philo_data)[i].is_eating), NULL))
		return (1);
	(*philo_data)[i].id = i;
	(*philo_data)[i].start_time = tv_start;
	(*philo_data)[i].last_meal = tv_start;
	(*philo_data)[i].time_to_die = ft_atoi(av[2]);
	(*philo_data)[i].time_to_eat = ft_atoi(av[3]);
	(*philo_data)[i].time_to_sleep = ft_atoi(av[4]);
	(*philo_data)[i].optional_param = 0;
	(*philo_data)[i].meals_to_eat = -1;
	if ((*philo_data)[i].time_to_die <= 0 || (*philo_data)[i].time_to_eat <= 0
			|| (*philo_data)[i].time_to_sleep <= 0)
		return (printf("Error Invalid Arguments!\n"));
	return (0);
}

int	init_prog(int philo_nbr, char **av, t_philo **philo_data, t_mutex *mutex)
{
	int				i;
	struct timeval	tv_start;

	gettimeofday(&tv_start, NULL);
	*philo_data = malloc(sizeof(t_philo) * philo_nbr);
	if (!(*philo_data))
		return (1);
	i = 0;
	while (i < philo_nbr)
	{
		(*philo_data)[i].id = i;
		(*philo_data)[i].philo_nbr = philo_nbr;
		if (get_values(philo_data, tv_start, i, av))
			return (2);
		if (av[5])
		{
			(*philo_data)[i].optional_param = 1;
			(*philo_data)[i].meals_to_eat = ft_atoi(av[5]);
		}
		i++;
	}
	i = 0;
	while (i < philo_nbr)
		(*philo_data)[i++].mutex = mutex;
	return (0);
}

int	init_mutex(t_mutex *mutex, int philo_nbr)
{
	int	i;

	mutex->nobody_dead = 1;
	mutex->forks = malloc(sizeof(pthread_mutex_t) * philo_nbr);
	if (!mutex->forks)
		return (1);
	if (pthread_mutex_init(&mutex->someone_died, NULL))
		return (1);
	if (pthread_mutex_init(&mutex->print, NULL))
		return (1);
	if (pthread_mutex_init(&mutex->meals_services, NULL))
		return (1);
	i = 0;
	while (i < philo_nbr)
	{
		if (pthread_mutex_init(&mutex->forks[i++], NULL))
			return (printf("Error\n"));
	}
	return (0);
}

int	main(int ac, char **av)
{
	pthread_t	*philo;
	t_mutex		mutex;
	t_philo		*philo_data;
	int			philo_nbr;

	if (ac < 5 || ac > 6)
		return (printf("Error Invalid Arguments!\n"));
	philo_nbr = ft_atoi(av[1]);
	if (philo_nbr <= 0)
		return (printf("Error Invalid Arguments!\n"));
	if (init_mutex(&mutex, philo_nbr))
		return (1);
	if (init_prog(philo_nbr, av, &philo_data, &mutex))
	{
		free(philo_data);
		free(mutex.forks);
		return (0);
	}
	philo_threads(philo_nbr, philo_data, &philo);
	free(mutex.forks);
	free(philo_data);
	return (0);
}
