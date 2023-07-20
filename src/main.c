#include "../philo.h"

static void	detect_death(t_table *t)
{
	int			i;
	long int	time_without_eating;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&t->p[i].last_meal);
		time_without_eating = get_time(0, 0) - t->p[i].time_last_meal;
		pthread_mutex_unlock(&t->p[i].last_meal);
		if (time_without_eating > t->p[i].time_to_die)
		{
			pthread_mutex_lock(&t->dead);
			t->is_dead = true;
			pthread_mutex_unlock(&t->dead);
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
	pthread_mutex_lock(&t->meal);
	if (t->nbr_of_meal == 0)
		return ;
	pthread_mutex_unlock(&t->meal);
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
		if (pthread_mutex_init(&t->p[i].fork_left, NULL)
			|| pthread_mutex_init(&t->p[i].last_meal, NULL))
			return (false);
		t->p[i].fork_right = &t->p[(i + 1) % t->nbr_of_philo].fork_left;
		t->p[i].state = THINK;
		t->p[i].philo_id = i + 1;
		t->p[i].time_to_die = t->time_to_die;
		t->p[i].time_to_eat = t->time_to_eat;
		t->p[i].time_to_sleep = t->time_to_sleep;
		if (t->nbr_of_meal == -1)
			t->p[i].meal_to_eat = -1;
		else
			t->p[i].meal_to_eat = t->nbr_of_meal / t->nbr_of_philo;
		t->p[i].time_last_meal = t->actual_time;
		t->p[i].t = t;
	}
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->dead, NULL)
		|| pthread_mutex_init(&t->meal, NULL)
		|| pthread_mutex_init(&t->time, NULL)
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
		t->nbr_of_meal = -1;
		// t->nbr_of_meal = t->nbr_of_philo;
	t->is_dead = false;
	t->end = t->nbr_of_philo;
	t->actual_time = get_time(0, 0);
	return (true);
}


// number_of_philosophers time_to_die time_to_eat time_to_sleep
//[number_of_times_each_philosopher_must_eat]

//◦number_of_philosophers: The number of philosophers and also the number
//of forks.
//◦time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
//milliseconds since the beginning of their last meal or the beginning of the sim-
//ulation, they die.
//◦time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
//During that time, they will need to hold two forks.
//◦time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
//◦number_of_times_each_philosopher_must_eat (optional argument): If all
//philosophers have eaten at least number_of_times_each_philosopher_must_eat
//times, the simulation stops. If not specified, the simulation stops when a
//philosopher dies.
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
	{
		ft_putstr_fd("initialisation failure\n", 2);
		return (EXIT_FAILURE);
	}
	philosophers_process(&t);
	// destroy_mutex(&t);
	return (EXIT_SUCCESS);
}
