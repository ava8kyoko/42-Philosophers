#include "../philo.h"

static void	detect_death(t_table *t)
{
	int			i;
	// long int	t_die;
	long int	time_without_eating;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&t->m_time_last_meal);
		time_without_eating = get_time(0, 0) - t->p[i].time_last_meal;
		pthread_mutex_unlock(&t->m_time_last_meal);
		if (time_without_eating > t->p[i].time_to_die)
		{
			pthread_mutex_lock(&t->m_dead);
			t->dead = true;
			pthread_mutex_unlock(&t->m_dead);
			print_state(&t->p[i], "is dead");
			return ;
		}
		i = (i + 1) % t->nbr_of_philo;
	}
}

static void	philosophers_process(t_table *t)
{
	int	id;

	id = -1;
	while (++id < t->nbr_of_philo)
	{	
		pthread_create(&t->p[id].thread, NULL, &philosophers_routine, &t->p[id]);
		pthread_detach(t->p[id].thread);
	}
	pthread_mutex_lock(&t->m_meal);
	if (t->nbr_of_meal == 0)
		return ;
	pthread_mutex_unlock(&t->m_meal);
	detect_death(t);
}

// -> The threads could synchronize their accesses via a mutex, each one locking
// that mutex before accessing a and unlocking it afterward.
// As the mutex option demonstrates, avoiding a data race does not require ensuring
// a specific order of operations, such as the child thread modifying a before
// the main thread reads it; it is sufficient (for avoiding a data race) to ensure
// that for a given execution, one access will happen before the other.

// p[id].fork_right = n + 1, so I use (id + 1) % t->nbr_of_philo to create a buckle,
// i.e. it starts at id = 1 and ends with id = 0
static bool	init_philo(t_table *t)
{
	int	i;

	i = -1;
	while (++i < t->nbr_of_philo)
	{
		if (pthread_mutex_init(&t->p[i].fork, NULL)
			|| pthread_mutex_init(&t->p[i].m_time_last_meal, NULL))
		return (false);
	scanf(" ");
		// t->p[i].fork_left = &t->p[(i + 1) % t->nbr_of_philo].fork_right;
		t->p[i].state = THINK;
		t->p[i].philo_id = i + 1;
		t->p[i].time_to_die = t->time_to_die;
		t->p[i].time_to_eat = t->time_to_eat;
		t->p[i].time_to_sleep = t->time_to_sleep;
		t->p[i].meal_to_eat = t->nbr_of_meal / t->nbr_of_philo;
		t->p[i].time_last_meal = t->time;
		t->p[i].t = t;
	}
	scanf(" ");
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->m_die, NULL)
		|| pthread_mutex_init(&t->m_meal, NULL)
		|| pthread_mutex_init(&t->m_time, NULL)
		|| pthread_mutex_init(&t->print, NULL))
		return (false);
	t->nbr_of_philo = ft_atoi(argv[1]);
	t->time_to_die = ft_atoi(argv[2]);
	t->time_to_eat = ft_atoi(argv[3]);
	t->time_to_sleep = ft_atoi(argv[4]);
	if (t->nbr_of_philo < 1 || t->time_to_die < 0
		|| t->time_to_eat < 0 || t->time_to_sleep < 0)
		return (0);
	if (argc == 6)
		t->nbr_of_meal = ft_atoi(argv[5]) * t->nbr_of_philo;
	else
		t->nbr_of_meal = t->nbr_of_philo;
	t->end = t->nbr_of_philo;
	t->time = get_time(0, 0);
	return (true);
}

int	main(int argc, char **argv)
{
	t_table	t;
	int		i;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("invalid argc\n", 2);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (argv[++i])
	{
		if (is_valid_int(argv[i]) == FAIL)
		{
			ft_putstr_fd("invalid argv\n", 2);
			return (EXIT_FAILURE);
		}
	}
	if (init_table(&t, argc, argv) == FAIL || init_philo(&t) == FAIL)
		ft_putstr_fd("initialisation failure\n", 2);
			return (EXIT_FAILURE);
	philosophers_process(&t);
	destroy_mutex(&t, 't');
	return (EXIT_SUCCESS);
}
