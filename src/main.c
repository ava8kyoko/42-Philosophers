#include "../philo.h"

static void	detect_death(t_table *t)
{
	int	id;
	long int	t_die;
	long int	t_meal;

	pthread_mutex_lock(&t->m_die);
	t_die = t->time_to_die;
	pthread_mutex_unlock(&t->m_die);
	id = 0;
	while (1)
	{
		pthread_mutex_lock(&t->m_last_meal);
		t_meal = t->p[id].time_last_meal;
		pthread_mutex_unlock(&t->m_last_meal);
		if (get_time(0, 0) - t_meal > t_die)
		{
			print_state(t->p, "is dead");
			pthread_mutex_lock(&t->m_dead);
			t->dead = true;
			pthread_mutex_unlock(&t->m_dead);
			return ;
		}
		id = (id + 1) % t->nbr_of_philo;
	}
}

static void	philosophers_process(t_table *t)
{
	int	id;

	id = -1;

	while (++id < t->nbr_of_philo)
	{
		if (t->p->philo_id % 2 == 0)
		{
			print_state(t->p, "is thinking");
			usleep(15000);
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
	while (++id < t->nbr_of_philo)
	{
		if (pthread_mutex_init(&t->p[id].fork_left, NULL))
			return (false);
		t->p[id].fork_right = &t->p[(id + 1) % t->nbr_of_philo].fork_left;
		t->p[id].state = THINK;
		t->p[id].philo_id = id + 1;
		t->p[id].time_to_die = t->time_to_die;
		t->p[id].time_to_eat = t->time_to_eat;
		t->p[id].time_to_sleep = t->time_to_sleep;
		t->p[id].meal_to_eat = t->nbr_of_meal;
		t->p[id].meal_to_eat = t->nbr_of_meal;
		t->p[id].time_last_meal = t->time;
		t->p[id].t = t;
	}
	t->p[id].fork_right = &t->p[0].fork_left;
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->m_dead, NULL)
		|| pthread_mutex_init(&t->m_die, NULL)
		|| pthread_mutex_init(&t->m_meal, NULL)
		|| pthread_mutex_init(&t->m_last_meal, NULL)
		|| pthread_mutex_init(&t->m_time, NULL)
		|| pthread_mutex_init(&t->health, NULL)
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
		t->nbr_of_meal = ft_atoi(argv[5]);
	else
		t->nbr_of_meal = -1;
	t->end = t->nbr_of_philo;
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
