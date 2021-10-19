/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ahma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 16:08:40 by oel-ahma          #+#    #+#             */
/*   Updated: 2021/10/11 18:10:23 by oel-ahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_mutex
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	someone_died;
	pthread_mutex_t	print;
	pthread_mutex_t	meals_services;
	int				nobody_dead;
}				t_mutex;

typedef struct s_philo
{
	struct timeval	start_time;
	struct timeval	last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_to_eat;
	int				optional_param;
	int				philo_nbr;
	pthread_mutex_t	is_eating;
	t_mutex			*mutex;
	pthread_t		philo[5];
	int				id;

}				t_philo;

void	*dinner(void *param);
int		ft_atoi(const char *str);
void	ft_sleep(int how_long);
void	print_status(t_philo *philo_data, char *str);

#endif
