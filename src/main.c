#include "../philo.h"

static void	detect_death(t_table *t)
{
	int	id;

	id = 0;
	while (1)
	{
		pthread_mutex_lock(&t->p[id].health);
		if (get_time(0, 0) - t->p[id].time_last_meal > t->time_die)
		{
			// pthread_mutex_unlock(&t->p[i].health);
			print_state(t->p, "is dead");
			t->dead = true;
			return ;
		}
		pthread_mutex_unlock(&t->main);
		id = (id + 1) % t->nbr_philo;
		// usleep(500);
	}
}

static void	philosophers_process(t_table *t)
{
	int	id;

	id = -1;
	while (++id < t->nbr_philo)
	{
		if (t->p->philo % 2 == 0)
		{
			pthread_mutex_lock(&t->p[id].print);
			print_state(t->p, "is thinking");
			pthread_mutex_lock(&t->p[id].print);
			usleep(500);
		}
		pthread_create(&t->p[id].thread, NULL, &philosophers_routine, &t->p[id]);
		pthread_detach(t->p[id].thread); // detach
	}
	detect_death(t);
}

// -> The threads could synchronize their accesses via a mutex, each one locking
// that mutex before accessing a and unlocking it afterward.
// As the mutex option demonstrates, avoiding a data race does not require ensuring
// a specific order of operations, such as the child thread modifying a before
// the main thread reads it; it is sufficient (for avoiding a data race) to ensure
// that for a given execution, one access will happen before the other.
static bool	init_philo(t_table *t)
{
	int	id;

	id = -1;
	while (++id < t->nbr_philo)
	{
		if (pthread_mutex_init(&t->p[id].fork_left, NULL)
			|| pthread_mutex_init(&t->p[id].health, NULL)
			|| pthread_mutex_init(&t->p[id].print, NULL)
			|| pthread_mutex_init(&t->p[id].forks, NULL))
			return (false);
		t->p[id].fork_right = &t->p[(id + 1) % t->nbr_philo].fork_left;
		t->p[id].forks_are_free = true;
		t->p[id].philo = id + 1;
		t->p[id].die = t->time_die;
		t->p[id].eat = t->time_eat;
		t->p[id].sleep = t->time_sleep;
		t->p[id].meal_to_eat = t->nbr_meal;
		t->p[id].meal_to_eat = t->nbr_meal;
		t->p[id].time_last_meal = t->time;
		t->p[id].t = t;
	}
	t->p[id].fork_right = &t->p[0].fork_left;
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->main, NULL))
		return (false);
	t->nbr_philo = ft_atoi(argv[1]);
	t->time_die = ft_atoi(argv[2]);
	t->time_eat = ft_atoi(argv[3]);
	t->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		t->nbr_meal = ft_atoi(argv[5]);
	else
		t->nbr_meal = -1;
	t->end = t->nbr_philo;
	t->dead = false;
	t->time = get_time(0, 0);
	return (true);
}

int	main(int argc, char **argv)
{
	t_table	t;
	int		i;

	if (argc == 5 || argc == 6)
	{
		i = 0;
		while (argv[++i])
		{
			if (!is_valid_int(argv[i]))
				return (0); // error ??
		}
		if (init_table(&t, argc, argv) && init_philo(&t))
			philosophers_process(&t);
	}
	return (0);
}
