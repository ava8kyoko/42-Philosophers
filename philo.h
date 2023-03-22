#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK_LEFT 'L'
# define FORK_RIGHT 'R'
# define EAT 'E'
# define SLEEP 'S'
# define THINK 'T'
# define DIE 'D'
# define MS 'M'
# define MAIN_UNLOCK 'U'
# define SLEEPING 1

typedef struct s_table	t_table;

typedef struct s_philo
{
	char			state;
	int				philo_id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_to_eat;
	long int		time_last_meal;
	t_table			*t;
	pthread_t		thread;
	pthread_mutex_t	fork_left;
	pthread_mutex_t	*fork_right;
}				t_philo;

typedef struct s_table
{
	int				nbr_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_meal;
	int				end;
	long int		time;
	bool			dead;
	t_philo			p[200];
	pthread_mutex_t	m_die;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_meal;
	pthread_mutex_t	m_last_meal;
	pthread_mutex_t	m_time;
	pthread_mutex_t	print;
	pthread_mutex_t	health;
}				t_table;

int			main(int argc, char **argv);

void		*philosophers_routine(void *arg);

bool		print_state(t_philo *p, char *state);
long int	get_time(t_philo *p, char ms);
long int	is_dead(t_philo *p);
int			ft_atoi(const char *str);
bool		is_valid_int(char *str);

#endif
