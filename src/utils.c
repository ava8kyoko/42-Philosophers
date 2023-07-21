#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
long int	get_time(t_philo *p, char ms)
{
	struct timeval	time;
	long int		previous_time; // a supprimer

	if (ms == MS)
	{
		pthread_mutex_lock(&p->t->time);
		previous_time = p->t->actual_time;
		pthread_mutex_unlock(&p->t->time);
		return (get_time(0, 0) - previous_time);
	}
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

bool	check_meal(t_philo *p)
{
	if (p->meal_to_eat == -1)
		return (true);
	
	return (false);
}
	




	// if (p->meal_to_eat == 0)
	// {
	// 	pthread_mutex_lock(&p->t->meal);
	// 	// printf("p->t->nbr_of_meal : %d\n", p->t->nbr_of_meal);
	// 	if (p->t->nbr_of_meal != 1)
	// 		p->t->nbr_of_meal -= 1;
	// 	else
	// 		return (false);
	// 	pthread_mutex_unlock(&p->t->meal);
	// 	p->meal_to_eat = -2;
	// }
	// if (p->meal_to_eat == -2)
	// {
	// 	pthread_mutex_lock(&p->t->meal);
	// 	p->meal_to_eat = p->t->nbr_of_meal;
	// 	pthread_mutex_unlock(&p->t->meal);
	// 	if (p->meal_to_eat <= 0) // verifier si fonctionne avec == 0
	// 		return (false);
	// 	p->meal_to_eat = -2;
	// }
	// return (true);
// }

bool	is_dead(t_philo *p)
{
	bool	is_dead;
	
	is_dead = false;
	pthread_mutex_lock(&p->t->dead);
	if (p->t->is_dead)
		is_dead = true;
	pthread_mutex_unlock(&p->t->dead);
	return (is_dead);
}

void	destroy_mutex(t_table *t)
{
	int	i;

	// pthread_mutex_lock(&t->dead);
	pthread_mutex_unlock(&t->dead);
	pthread_mutex_destroy(&t->dead);
	// pthread_mutex_lock(&t->meal);
	pthread_mutex_unlock(&t->meal);
	pthread_mutex_destroy(&t->meal);
	// pthread_mutex_lock(&t->print);
	pthread_mutex_unlock(&t->print);
	pthread_mutex_destroy(&t->print);
	// pthread_mutex_lock(&t->time);
	pthread_mutex_unlock(&t->time);
	pthread_mutex_destroy(&t->time);
	i = -1;
	while (++i < t->nbr_of_philo)
	{
		// pthread_mutex_lock(&t->p[i].fork_left);
		pthread_mutex_unlock(&t->p[i].fork_left);
		pthread_mutex_destroy(&t->p[i].fork_left);
		// pthread_mutex_lock(&t->p[i].last_meal);
		pthread_mutex_unlock(&t->p[i].last_meal);
		pthread_mutex_destroy(&t->p[i].last_meal);
	}
}
