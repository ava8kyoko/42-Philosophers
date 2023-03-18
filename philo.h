#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

# define FORK 'F'
# define EAT 'E'
# define SLEEP 'S'
# define THINK 'T'
# define DIE 'D'
# define LEFT 'L'
# define RIGHT 'R'
# define LEFT_RIGHT '2'
# define MS 1
# define SLEEPING 1

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				philo;
	int				meal_to_eat;
	long int		time_last_meal;
	t_table			*t;
	pthread_t		thread;
	pthread_mutex_t	fork_left;
	pthread_mutex_t	*fork_right;
}				t_philo;

typedef struct s_table
{
	int				nbr_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nbr_meal;
	int				end;
	long int		time;
	bool			dead;
	t_philo			p[500];
	pthread_mutex_t	lock; // ??
}				t_table;

void			*philosophers_routine(void *arg);
long int		get_time(t_philo *p, bool ms);
int				ft_atoi(const char *str);
bool			is_valid_int(char *str);

#endif