#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
long int	get_time(t_philo *p, char ms)
{
	struct timeval	time;
	long int		previous_time;

	if (ms == MS)
	{
		pthread_mutex_lock(&p->t->m_time);
		previous_time = p->t->time;
		pthread_mutex_unlock(&p->t->m_time);
		return (get_time(0, 0) - previous_time);
	}
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

bool	is_dead(t_philo *p)
{
	bool	is_dead;
	
	pthread_mutex_lock(&p->t->m_dead);
	is_dead = p->t->dead;
	pthread_mutex_unlock(&p->t->m_dead);
	if (is_dead)
		return (true);
	return (false);
}
// {	
// 	if (get_time(0, 0) - p->time_last_meal > p->time_to_die)
// 		return (false);
// 	pthread_mutex_lock(&p->t->m_dead);
// 	p->dead = true;
// 	pthread_mutex_unlock(&p->t->m_dead);
// 	return (true);
// }

int	ft_atoi(const char *str)
{
	long	sign;
	long	converted;

	sign = 1;
	converted = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		if ((converted * 10) + (*str - '0') < converted && sign == 1)
			return (-1);
		else if ((converted * 10) + (*str - '0') < converted && sign == -1)
			return (0);
		converted *= 10;
		converted += *str++ - '0';
	}
	return (sign * converted);
}

bool	is_valid_int(char *str)
{
	int			sign;
	long long	value;

	sign = 1;
	value = 0;
	// printf("%s\n", str);
	if (*str == '-')
	{
		sign = -1;
		str++;
		if (!*str)
			return (false);
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		value *= 10;
		value += *str - '0';
		if ((value * sign) > 2147483647 || (value * sign) < -2147483648)
			return (false);
		str++;
	}
	if (*str && (*str < '0' || *str > '9'))
		return (false);
	return (true);
}
