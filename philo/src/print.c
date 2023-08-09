/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:07 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/09 15:16:25 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

bool	print_state(t_philo *p, char *msg_state)
{
	// bool	end;
	
	// if (alive)
	// {
	// 	end = false;
	// pthread_mutex_lock(&p->end);
	// if (p->ending == true)
	// 	end = true;
	// pthread_mutex_unlock(&p->end);
	// if (end)
	// 	return (true);
	// }
	pthread_mutex_lock(&p->t->print);
	printf("%lu %d %s\n", get_time(p, MS), p->philo_id, msg_state);
	pthread_mutex_unlock(&p->t->print);
	return (false);
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
