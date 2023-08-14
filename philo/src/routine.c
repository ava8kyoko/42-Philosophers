/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:11 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/14 16:19:19 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// ./philo 200 10010 100 100
// ./philo 200 210 10 10  
// ./philo 100 110 10 10    
// ./philo 100 1010 10 10 
//  ./philo 100 1010 200 200 
// Ameliorer ici ?? 
// test ./philo 100 110 100 100
// 201 18 is sleeping
// 201 16 is sleeping
// 200 49 is dead

static bool	need_to_sleep(t_philo *p)
{
	long int	time_to_stop;

	time_to_stop = 0;
	if (p->state == EAT)
	{
		pthread_mutex_lock(&p->time);
		p->time_last_meal = get_time(0, 0);
		time_to_stop = p->time_last_meal + p->time_to_eat;
		pthread_mutex_unlock(&p->time);
		if (print_state(p, "is eating"))
			return (true); // ajouter temps avant de mourrir resultat// peut-etre dand cette variable
		p->state = SLEEP;
	}
	else if (p->state == SLEEP)
	{
		if (print_state(p, "is sleeping"))
			return (true);
		time_to_stop = p->time_last_meal + p->time_to_eat + p->time_to_sleep;
		p->state = THINK;
	}
	return (make_it_sleep(p, time_to_stop));
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
		// pthread_mutex_lock(&p->end_main_to_philo);
		// if (p->ending)
		// 	return (true);
		// pthread_mutex_unlock(&p->end_main_to_philo);
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
			// pthread_mutex_lock(&p->end_main_to_philo);
			// if (p->ending)
			// 	return (true);
			// pthread_mutex_unlock(&p->end_main_to_philo);
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
	// pthread_mutex_lock(&p->dead_philo_to_main);
	// p->is_dead = true;
	// pthread_mutex_unlock(&p->dead_philo_to_main);
	return ((void *)0);
}
