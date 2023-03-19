#include "../philo.h"

static void	detect_death(t_table *t)
{
	int	i;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&t->p[i].health);
		if (get_time(0, 0) - t->p[i].time_last_meal > t->time_die)
		{
			pthread_mutex_unlock(&t->p[i].health);
			pthread_mutex_lock(&t->p[i].print);
			printf("%lu %d is dead\n", get_time(&t->p[i], MS), t->p[i].philo);
			pthread_mutex_unlock(&t->p[i].print);
			t->dead = true;
			return ;
		}
		pthread_mutex_unlock(&t->main);
		i = (i + 1) % t->nbr_philo;
		// usleep(500);
		// i++;
	}
}

static void	philosophers_process(t_table *t)
{
	int	i;

	i = -1;
	while (++i < t->nbr_philo)
	{
		if (p->philo % 2 == 0)
		{
			print_state(p, "is thinking");
			usleep(500);
		}
		pthread_create(&t->p[i].thread, NULL, &philosophers_routine, &t->p[i]);
		pthread_detach(t->p[i].thread); // detach
	}
	detect_death(t);
}

static bool	init_philo(t_table *t)
{
	int	i;

	i = -1;
	while (++i < t->nbr_philo)
	{
		if (pthread_mutex_init(&t->p[i].fork_left, NULL)
			|| pthread_mutex_init(&t->p[i].health, NULL)
			|| pthread_mutex_init(&t->p[i].print, NULL)
			|| pthread_mutex_init(&t->p[i].forks, NULL))
			return (false);
		t->p[i].fork_right = &t->p[(i + 1) % t->nbr_philo].fork_left;
		t->p[i].forks_are_free = true;
		t->p[i].philo = i + 1;
		t->p[i].die = t->time_die;
		t->p[i].eat = t->time_eat;
		t->p[i].sleep = t->time_sleep;
		t->p[i].meal_to_eat = t->nbr_meal;
		t->p[i].meal_to_eat = t->nbr_meal;
		t->p[i].time_last_meal = t->time;
		t->p[i].t = t;
	}
	t->p[i].fork_right = &t->p[0].fork_left;
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
