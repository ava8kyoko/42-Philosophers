#include "../philo.h"

static void	need_to_sleep(t_philo *p)
{
	long int	time_to_stop;
	long int	delay;

	time_to_stop = 0;
	if (p->state == EAT)
	{
		time_to_stop =  p->time_last_meal + p->time_to_eat;
		p->state = SLEEP;
	}
	else if (p->state == SLEEP)
	{
		time_to_stop = p->time_to_sleep;
		p->state = THINK;
	}
	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0)
			break;
		usleep(500); //10000
	}
}

static bool is_sleeping(t_philo *p)
{
	bool	dead_state;

	if (p->state == SLEEP)
	{
		pthread_mutex_lock(&p->t->m_dead);
		dead_state = p->t->dead;
		pthread_mutex_unlock(&p->t->m_dead);
		if (dead_state == true)
			return (false);
		print_state(p, "is sleeping");
		need_to_sleep(p);
	}
	return (true);
}

// When a hungry philosopher has both his forks at the same time, he eats
// without relasing his forks. When he has finised eating, he puts down
// both of his forks and starts thinking again.
static bool	is_eating(t_philo *p)
{
	if (p->state == EAT)
	{
		if (is_dead(p))
			return (false);
		pthread_mutex_lock(&p->t->m_last_meal);
		p->time_last_meal = get_time(0, 0);
		pthread_mutex_unlock(&p->t->m_last_meal);
		if (p->meal_to_eat != -1) 
		{
			p->meal_to_eat -= 1;
			if (p->meal_to_eat == 0)
				return (false);
		}
		print_state(p, "is eating");
		need_to_sleep(p);
		pthread_mutex_unlock(&p->fork_left);
		pthread_mutex_unlock(p->fork_right);
	}
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
	if (p->state == THINK || p->state == FORK_RIGHT)
	{
		pthread_mutex_lock(&p->fork_left);
		print_state(p, "has taken a fork_left");
		if (p->state == FORK_RIGHT)
			p->state = EAT;
		else if (p->state == THINK)
			p->state = FORK_LEFT;
		if (is_dead(p))
			return (false);
	}
	if (p->state == THINK || p->state == FORK_LEFT)
	{
		pthread_mutex_lock(p->fork_right);
		print_state(p, "has taken a fork_right");
		if (p->state == FORK_LEFT)
			p->state = EAT;
		else if (p->state == THINK)
			p->state = FORK_RIGHT;
		if (is_dead(p))
			return (false);
	}
	return (true);
}	

// When a philosopher thinks, he does not interact with his colleagues.
void	*philosophers_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	pthread_mutex_lock(&p->t->m_time);
	p->time_last_meal = p->t->time;
	pthread_mutex_unlock(&p->t->m_time);
	while (1)
	{
		if (is_taking_forks(p) == false)
			break;
		if (is_eating(p) == false)
			break;
		if (is_sleeping(p) == false)
			break;
		if (p->state == THINK)
			print_state(p, "is thinking");
		if (p->meal_to_eat == 0)
		{
			pthread_mutex_lock(&p->t->m_meal);
			p->t->nbr_of_meal -= 1; // manque confition fin meal dans main
			pthread_mutex_unlock(&p->t->m_meal);
		}
	}
	return ((void*)0);
}
