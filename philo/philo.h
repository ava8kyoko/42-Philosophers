/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:26:01 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/14 16:03:44 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK_LEFT 76
# define FORK_RIGHT 82
# define EAT 69
# define SLEEP 83
# define THINK 84

typedef struct s_table	t_table;

typedef struct s_philo
{
	bool			ending;
	char			state;
	int				meal_to_eat;
	int				philo_id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long int		time_last_meal;
	long int		time_start;
	pthread_mutex_t	end_main_to_philo;
	pthread_mutex_t	fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	meal;
	pthread_mutex_t	time;
	pthread_t		thread;
	t_table			*t;
}				t_philo;

typedef struct s_table
{
	int				end_routine;
	int				nbr_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_meal;
	long int		actual_time;
	t_philo			p[200];
	pthread_mutex_t	print;
}				t_table;

void		ft_putstr_fd(char *str, int fd);
bool		print_state(t_philo *p, char *state);

long int	get_time(t_philo *p, char ms);
bool 		make_it_sleep(t_philo *p, long int time_to_stop);
// void		destroy_mutex(t_table *t);

void		*philosophers_routine(void *arg);

bool		is_valid_int(char *str);
int			ft_atoi(const char *str);

int			main(int argc, char **argv);

#endif
