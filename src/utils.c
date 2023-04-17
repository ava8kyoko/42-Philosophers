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

bool	check_meal(t_philo *p)
{
	if (p->meal_to_eat == 0)
	{
		pthread_mutex_lock(&p->t->m_meal);
		// printf("p->t->nbr_of_meal : %d\n", p->t->nbr_of_meal);
		if (p->t->nbr_of_meal != 1)
			p->t->nbr_of_meal -= 1;
		else
			return (false);
		pthread_mutex_unlock(&p->t->m_meal);
		p->meal_to_eat = -2;
	}
	if (p->meal_to_eat == -2)
	{
		pthread_mutex_lock(&p->t->m_meal);
		p->meal_to_eat = p->t->nbr_of_meal;
		pthread_mutex_unlock(&p->t->m_meal);
		if (p->meal_to_eat <= 0) // verifier si fonctionne avec == 0
			return (false);
		p->meal_to_eat = -2;
	}
	return (true);
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

void	destroy_mutex(t_table *t, char flag)
{
	int	i;
	if (flag == 't')
	{
		pthread_mutex_lock(&t->m_die);
		pthread_mutex_unlock(&t->m_die);
		pthread_mutex_destroy(&t->m_die);
		pthread_mutex_lock(&t->m_meal);
		pthread_mutex_unlock(&t->m_meal);
		pthread_mutex_destroy(&t->m_meal);
		pthread_mutex_lock(&t->m_time);
		pthread_mutex_unlock(&t->m_time);
		pthread_mutex_destroy(&t->m_time);
		pthread_mutex_lock(&t->print);
		pthread_mutex_unlock(&t->print);
		pthread_mutex_destroy(&t->print);
	}
	else if (flag == 'p')
	{
		i = -1;
		while (++i < t->nbr_of_philo)
		{
			pthread_mutex_lock(&t->fork[i]);
			pthread_mutex_unlock(&t->fork[i]);
			pthread_mutex_destroy(&t->fork[i]);
			pthread_mutex_lock(&t->m_time_last_meal[i]);
			pthread_mutex_unlock(&t->m_time_last_meal[i]);
			pthread_mutex_destroy(&t->m_time_last_meal[i]);
		}
	}
}
