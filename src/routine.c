#include "../philo.h"

static bool	sleep(t_philo *p);
{
	long int	delay;
	long int	time_to_stop;

	if (state = eating)
	{
		pthread_mutex_lock(&p->t->main);
		time_to_stop =  p->time_last_meal + p->t->time_sleep + p->t->time_eat;
		pthread_mutex_unlock(&p->t->main);
	}
	else if (sleeping)
	{
		pthread_mutex_lock(&p->t->main);
		time_to_stop = p->time_last_meal + p->t->time_eat;
		pthread_mutex_unlock(&p->t->main);
	}
	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0)
			break;
		usleep(20);
	}
}

bool	print_state(t_philo *p, char *state) // char fork
{
	pthread_mutex_lock(&p->print);
	printf("%lu %d %s\n", get_time(p, MS), p->philo, state);
	pthread_mutex_unlock(&p->print);
	return (false);
}

// When a hungry philosopher has both his forks at the same time, he eats
// without relasing his forks. When he has finised eating, he puts down
// both of his forks and starts thinking again.
static bool	is_eating_sleeping(t_philo *p, bool sleeping)
{

	p->time_last_meal = get_time(0, 0);
	print_state(p, "is eating");
	if (p->meal_to_eat != -1) 
	{
		p->meal_to_eat -= 1;
		if (p->meal_to_eat == 0)
			return (false);
	}
	pthread_mutex_unlock(&p->fork_left);
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_lock(&p->t->main);
	if (p->t->dead == true)
	{
		pthread_mutex_unlock(&p->t->main);
		return (false);
	}
	pthread_mutex_lock(&p->t->main);
	print_state(p, "is sleeping");
	return (true);
}

// When a philosopher gets hungry he tries to pick up the two forks that are
// closest to him (left and right). A philosopher may pick up only one fork
// at a time.
// Cannot pick up a fork that is already in the hand of a neighbour.
static bool	is_taking_forks(t_philo *p)
{
	if (is_dead(p))
		return (true);
	pthread_mutex_lock(&p->forks);
	if (p->forks_are_free == true)
	{
		pthread_mutex_lock(&p->forks);
		pthread_mutex_lock(p->fork_right);
		print_state(p, "has taken a fork");
		if (is_dead(p))
			return (true);
		pthread_mutex_lock(&p->forks);
		p->forks_are_free = false;
		pthread_mutex_unlock(&p->forks);
		pthread_mutex_lock(&p->fork_left);
		print_state(p, "has taken a fork");
		return (true);
	}
	pthread_mutex_unlock(&p->forks);
	return (false);
}	

// When a philosopher thinks, he does not interact with his colleagues.
void	*philosophers_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	p->time_last_meal = p->t->time;
	while (1)
	{
		if (is_taking_forks(p)
			|| is_eating_sleeping(p, SLEEPING))
			break;
		print_state(p, "is thinking");
	}
	return ((void*)0);
}
