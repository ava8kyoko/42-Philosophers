/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:11 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/09 15:35:45 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static bool	need_to_sleep(t_philo *p)
{
	long int	time_to_stop;
	long int	delay;

	time_to_stop = 0;
	if (p->state == EAT)
	{
		usleep(p->time_to_eat);
		p->state = SLEEP;
		time_to_stop = p->time_last_meal + p->time_to_eat;
		if (time_to_stop > p->time_to_die)
			return (true);
		print_state(p, "is eating");
		return (false); 
	}
	else if (p->state == SLEEP)
	{
		print_state(p, "is sleeping");
		time_to_stop = p->time_last_meal + p->time_to_eat + p->time_to_sleep;
		if (time_to_stop > p->time_to_die)
			return (true);
		// usleep(p->time_to_sleep);
		p->state = THINK;
	}
	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0)
			break ;
		usleep(100); //250
	}
	return (false);
}

// When a hungry philosopher has both his forks at the same time, he eats
// without relasing his forks. When he has finished eating, he puts down
// both of his forks and starts thinking again.
static bool	is_eating(t_philo *p)
{
	p->time_last_meal = get_time(0, 0);
	if (need_to_sleep(p))
		return (true);
	pthread_mutex_unlock(&p->fork_left);
	pthread_mutex_unlock(p->fork_right);
	if (p->meal_to_eat != -1) 
		p->meal_to_eat -= 1;
	pthread_mutex_lock(&p->meal);
	return (false);
}

// When a philosopher gets hungry he tries to pick up the two forks that are
// closest to him (left and right). A philosopher may pick up only one fork
// at a time.
// Cannot pick up a fork that is already in the hand of a neighbour.
static bool	is_taking_forks(t_philo *p)
{
	while (p->state != EAT)
	{
		if (p->state == THINK || p->state == FORK_RIGHT)
		{
			pthread_mutex_lock(&p->fork_left);
			print_state(p, "has taken a fork_left");
			if (p->state == FORK_RIGHT)
				p->state = EAT;
			else if (p->state == THINK)
				p->state = FORK_LEFT;
		}
		else if (p->state == FORK_LEFT)
		{
			pthread_mutex_lock(p->fork_right);
			if (print_state(p, "has taken a fork_right"))
				return (true);
			if (p->state == FORK_LEFT)
				p->state = EAT;
			else if (p->state == THINK)
				p->state = FORK_RIGHT;
		}
	}
	return (false);
}

// When a philosopher thinks, he does not interact with his colleagues.
void	*philosophers_routine(void *arg)
{
	t_philo	*p;

	p = arg;

	if (p->philo_id % 2 == 0)
	{
		print_state(p, "is thinking");
		usleep(200); //50
	}
	while (1)
	{
		if (p->state == THINK && is_taking_forks(p))
			break ;
		if (p->state == EAT && is_eating(p))
			break ;
		if (p->state == SLEEP && need_to_sleep(p))
			break ;
		if (p->state == THINK && print_state(p, "is thinking"))
			break ;
	}
	p->is_dead = true;
	return ((void *)0);
}
