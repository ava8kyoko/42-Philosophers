#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
long int	get_time(t_philo *p, char ms)
{
	struct timeval	time;

	if (ms == MS)
	{
		pthread_mutex_lock(&p->t->main);
		return (get_time(0, MAIN_UNLOCK) - p->t->time);
	}
	if (ms == MAIN_UNLOCK)
		pthread_mutex_unlock(&p->t->main);
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

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
