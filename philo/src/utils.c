#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
long int	get_time(t_philo *p, char ms)
{
	struct timeval	time;

	if (ms == MS)
		return (get_time(0, 0) - p->time_start);
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

// void is_sated(t_philo *p, bool eat)
// {
// 	int	sated;

// 	sated = CONTINUE;
// 	if (eat)
// 	{
// 		pthread_mutex_lock(&p->t->meal);
// 		p->meal_to_eat--;
// 		pthread_mutex_unlock(&p->t->meal);
// 	}
// 	// else
// 	// {
// 	// 	pthread_mutex_lock(&p->t->end);
// 	// 	if (p->t->end_routine == SATED)
// 	// 		sated = SATED;
// 	// 	pthread_mutex_unlock(&p->t->end);
// 	// }
// 	// return (sated);
// }

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

// int	is_dead(t_philo *p)
// {
// 	int	dead;
	
// 	dead = CONTINUE;
// 	// usleep(50); // does it fix data race? creates delay
// 	pthread_mutex_lock(&p->t->end);
// 	if (p->t->end_routine == DEAD)
// 		dead = DEAD;
// 	pthread_mutex_unlock(&p->t->end);
// 	return (dead);
// }

void	destroy_mutex(t_table *t)
{
	int	i;

	// pthread_mutex_lock(&t->print);
	pthread_mutex_unlock(&t->print);
	pthread_mutex_destroy(&t->print);
	i = -1;
	while (++i < t->nbr_of_philo)
	{
		// pthread_mutex_lock(&t->p[i].fork_left);
		pthread_mutex_unlock(&t->p[i].fork_left);
		pthread_mutex_destroy(&t->p[i].fork_left);
		// pthread_mutex_lock(&t->p[i].last_meal);
		pthread_mutex_unlock(&t->p[i].last_meal);
		pthread_mutex_destroy(&t->p[i].last_meal);
		// pthread_mutex_lock(&t->p[i].meal);
		pthread_mutex_unlock(&t->p[i].meal);
		pthread_mutex_destroy(&t->p[i].meal);
	}
}
