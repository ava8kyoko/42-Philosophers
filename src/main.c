#include "../philo.h"

static void	detect_death(t_table *t)
{
	int	i;

	i = 0;
	usleep(500);
	while (1)
	{
		if (t->end == 0)
			return ;
		if (get_time(0, 0) - t->p[i].time_last_meal > t->time_die)
		{
			printf("%lu %d is dead\n", get_time(&t->p[i], MS), t->p[i].philo);
			t->dead = true;
			return ;
		}
		i = (i + 1) % t->nbr_philo;
		usleep(500);
	}
}

static bool	philosophers_process(t_table *t)
{
	int			i;

	i = -1;
	while (++i < t->nbr_philo)
		pthread_create(&t->p[i].thread, NULL, &philosophers_routine, &t->p[i]);
	i = -1;
	detect_death(t);
	while (++i < t->nbr_philo)
		pthread_join(t->p[i].thread, NULL);
	return (true);
}

static bool	init_philo(t_table *t)
{
	int				i;

	i = -1;
	while (++i < t->nbr_philo)
	{
		if (pthread_mutex_init(&t->p[i].fork_left, NULL) != 0)
			return (false);
		t->p[i].fork_right = &t->p[(i + 1) % t->nbr_philo].fork_left;
		t->p[i].philo = i + 1;
		t->p[i].meal_to_eat = t->nbr_meal;
		t->p[i].meal_to_eat = t->nbr_meal; //0;
		t->p[i].time_last_meal = t->time;
		t->p[i].t = t;
	}
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->lock, NULL) != 0)
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
	t_table	table;
	int		i;

	if (argc == 5 || argc == 6)
	{
		i = 0;
		while (argv[++i])
		{
			if (!is_valid_int(argv[i]))
				return (0); // error ??
		}
		if (init_table(&table, argc, argv) && init_philo(&table))
			philosophers_process(&table);
	}
	return (0);
}
