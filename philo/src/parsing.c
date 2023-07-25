/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:03 by mchampag          #+#    #+#             */
/*   Updated: 2023/07/25 10:41:35 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	long	sign;
	long	converted;

	sign = 1;
	converted = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		if ((converted * 10) + (*str - '0') < converted && sign == 1)
			return (-1);
		else if ((converted * 10) + (*str - '0') < converted && sign == -1)
			return (0);
		converted *= 10;
		converted += *str++ - '0';
	}
	return (sign * converted);
}

bool	is_valid_int(char *str)
{
	int			sign;
	long long	value;

	sign = 1;
	value = 0;
	if (*str == '-')
	{
		sign = -1;
		str++;
		if (!*str)
			return (false);
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		value *= 10;
		value += *str - '0';
		if ((value * sign) > 2147483647 || (value * sign) < -2147483648)
			return (false);
		str++;
	}
	if (*str && (*str < '0' || *str > '9'))
		return (false);
	return (true);
}
