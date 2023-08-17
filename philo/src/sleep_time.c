/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:14 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/17 00:00:37 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// Check if philo is dead
// if not print what he's doing
// time_t         tv_sec      seconds
// suseconds_t    tv_usec     microseconds
// UNIX time in milliseconds
long int	get_time(bool time_from_start, long int start)
{
	struct timeval	time;
	long int		time_in_milliseconds;

	if (time_from_start == true)
		return (get_time(0, 0) - start);
	gettimeofday(&time, NULL);
	time_in_milliseconds = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (time_in_milliseconds);
}

bool	make_it_sleep(t_philo *p, long int time_to_stop)
{
	long int	delay;

	delay = p->time_last_meal;
	while (1)
	{
		delay = time_to_stop - get_time(0, 0);
		if (delay <= 0)
			return (false);
		usleep(100);
	}
}
