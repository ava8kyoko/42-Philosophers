/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:07 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/12 15:20:24 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
// If a philosopher is dead or if all meals are eaten, return true
// and stops all processes.
// Else, prints state message and return false.
*/
bool	print_state(t_philo *p, char *msg_state)
{
	bool dead_end;
	
	dead_end = false;
	pthread_mutex_lock(&p->end_main_to_philo);
	if (p->ending)
		dead_end = true;
	pthread_mutex_unlock(&p->end_main_to_philo);
	if (dead_end == false)
	{
		pthread_mutex_lock(&p->t->print);
		printf("%lu %d %s\n", get_time(p, true), p->philo_id, msg_state);
		pthread_mutex_unlock(&p->t->print);
	}
	return (dead_end);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}
