#include "philo.h"

int	convert_nbr(const char *str, int i)
{
	int		nbr;
	int		tmp;

	nbr = 0;
	tmp = 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr + (str[i] - 48) * tmp;
		i--;
		tmp = tmp * 10;
	}
	return (nbr);
}

int	ft_atoi(const char *str)
{
	int		signe;
	int		i;

	i = 0;
	signe = 1;
	while ((str[i] != '\0') && ((str[i] == ' ') || (str[i] == '\t')
				|| (str[i] == '\n') || (str[i] == '\v')
				|| (str[i] == '\f') || (str[i] == '\r')))
		i++;
	if (str[i] == '-')
	{
		signe = signe * -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	return (convert_nbr(str, i - 1) * signe);
}

void	ft_sleep(int how_long)
{
	struct timeval	timeofnow;
	struct timeval	timeofafter;
	int				time;

	time = 0;
	gettimeofday(&timeofnow, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&timeofafter, NULL);
		time = (timeofafter.tv_sec - timeofnow.tv_sec)
			* 1000000 + (timeofafter.tv_usec - timeofnow.tv_usec);
		if (time > how_long || time < 0)
			break ;
	}
}

void print_status(t_philo *philo_data, char *str, int color)
{
    struct timeval tv;
    long int time;

    pthread_mutex_lock(&philo_data->mutex->print);
    gettimeofday(&tv, NULL);
    time = (tv.tv_sec - philo_data->start_time.tv_sec) * 1000 + (tv.tv_usec - philo_data->start_time.tv_usec) / 1000;
    pthread_mutex_lock(&philo_data->mutex->someone_died);
    if (philo_data->mutex->nobody_dead == 1)
    {
        if (color == 1)
            printf(RED "IT's %ld : PHILO %d %s\n" reset, time, philo_data->id + 1, str);
        if (color == 2)
            printf(YEL "IT's %ld : PHILO %d %s\n" reset, time, philo_data->id + 1, str);
        if (color == 0)
            printf("IT's %ld : PHILO %d %s\n", time, philo_data->id + 1, str);
    }
    pthread_mutex_unlock(&philo_data->mutex->someone_died);
    pthread_mutex_unlock(&philo_data->mutex->print);
}