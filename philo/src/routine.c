/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:11 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/11 20:32:03 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// Ameliorer ici ?? 
// test ./philo 100 110 100 100
// 201 18 is sleeping
// 201 16 is sleeping
// 200 49 is dead

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
		if (get_time(0, 0) - p->time_last_meal> p->time_to_die)
			return (true);
		p->time_last_meal = get_time(0, 0);
		time_to_stop = p->time_last_meal + p->time_to_eat;
		if (print_state(p, "is eating"))
			return (true); // ajouter temps avant de mourrir resultat// peut-etre dand cette variable
		// usleep(p->time_to_eat);
		p->state = SLEEP;
	}
	else if (p->state == SLEEP)
	{
		if (print_state(p, "is sleeping"))
			return (true);
		time_to_stop = p->time_last_meal + p->time_to_eat + p->time_to_sleep;
		// usleep(p->time_to_sleep);
		p->state = THINK;
	}
	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0) // peut-etre ajouter verification ici
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
	if (need_to_sleep(p))
		return (true);
	pthread_mutex_unlock(&p->fork_left);
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_lock(&p->meal);
	if (p->meal_to_eat != -1) 
		p->meal_to_eat -= 1;
	pthread_mutex_unlock(&p->meal);
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
			if (print_state(p, "has taken a fork_left"))
				return (true);
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
		usleep(100); //50
	}
	while (1)
	{
		if (p->state == THINK && is_taking_forks(p))
			break ;
		if (p->state == EAT && is_eating(p))
			break ;
		if (p->state == SLEEP && need_to_sleep(p))
			break ;
		if ((p->state == THINK && print_state(p, "is thinking")))
			break ;
	}
	pthread_mutex_lock(&p->dead_philo_to_main);
	p->is_dead = true;
	pthread_mutex_unlock(&p->dead_philo_to_main);
	return ((void *)0);
}
