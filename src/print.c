#include "../philo.h"

bool	print_state(t_philo *p, char *msg_state) // char fork
{
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

void	print_exit_error(char* msg)
{
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}
