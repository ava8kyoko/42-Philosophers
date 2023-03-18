#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
long int	get_time(t_philo *p, bool ms)
{
	struct timeval	time;

	if (ms)
		return (get_time(0, 0) - p->t->time);
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

static bool	is_dead_print(t_philo *p, char fork, char *state)
{
	pthread_mutex_lock(&p->t->lock);
	if (p->t->dead == true)
	{
		pthread_mutex_unlock(&p->t->lock);
		if (fork == LEFT || fork == LEFT_RIGHT)
			pthread_mutex_unlock(&p->fork_left);
		if (fork == RIGHT || fork == LEFT_RIGHT)
			pthread_mutex_unlock(p->fork_right);
		return (true);
	}
	printf("%lu %d %s\n", get_time(p, MS), p->philo, state);
	pthread_mutex_unlock(&p->t->lock);
	return (false);
}

static bool	is_sleeping(t_philo *p, long int time_to_stop, bool sleeping)
{
	long int	delay;

	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0)
			break ;
		usleep(500);
	}
	if (sleeping)
	{
		pthread_mutex_unlock(&p->fork_left);
		pthread_mutex_unlock(p->fork_right);
		if (p->t->dead == true)
			return (true);
		printf("%lu %d is sleeping!\n", get_time(p, MS), p->philo);
		pthread_mutex_unlock(&p->t->lock);
	}
	return (false);
}

static bool	is_taking_forks_eating(t_philo *p)
{
	pthread_mutex_lock(&p->fork_left);
	if (is_dead_print(p, LEFT, "has taken a fork"))
		return (true);
	pthread_mutex_lock(p->fork_right);
	p->time_last_meal = get_time(0, 0);
	if (is_dead_print(p, LEFT_RIGHT, "has taken a fork"))
		return (true);
	if (is_dead_print(p, LEFT_RIGHT, "is eating!"))
		return (true);
	if (p->t->nbr_meal != -1) 
		p->meal_to_eat -= 1;
	// if (p->eated_meal == p->meal_count)
	// 	return (true);
	return (false);
}

void	*philosophers_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	if (p->philo % 2 == 0)
		usleep(500); // 450
	p->time_last_meal = p->t->time;
	while (1)
	{
		if (is_taking_forks_eating(p)
			|| is_sleeping(p, p->time_last_meal + p->t->time_eat, SLEEPING)
			|| p->meal_to_eat == 0)  // check if necessary
			break ;
		is_sleeping(p, p->time_last_meal + p->t->time_sleep + p->t->time_eat, 0);
		if (is_dead_print(p, 0, "is thinking!"))
			break ;
	}
	p->t->end -= 1;
	return ((void*)0);
}
