/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:56:35 by adichou           #+#    #+#             */
/*   Updated: 2025/09/14 21:58:41 by adichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_char_num(str[i]))
			return (0);
		i ++;
	}
	return (1);
}

int	verify_args(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (0);
	if (ft_atoi(av[1]) <= 0 || ft_atoi(av[2]) <= 0
		|| ft_atoi(av[3]) <= 0 || ft_atoi(av[4]) <= 0)
		return (0);
	if (!isnum(av[1]) || !isnum(av[2]) || !isnum(av[3]) || !isnum(av[4]))
		return (0);
	if (ac == 6 && (ft_atoi(av[5]) <= 0 || !isnum(av[5])))
		return (0);
	if (ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60 || ft_atoi(av[4]) < 60)
		return (0);
	return (1);
}

t_program	*init_program(char **av)
{
	t_program	*res;

	res = malloc(sizeof(t_program));
	if (!res)
		return (NULL);
	res->start_time = get_time();
	res->nb_philo = ft_atoi(av[1]);
	res->time_to_die = ft_atoi(av[2]);
	res->time_to_eat = ft_atoi(av[3]);
	res->time_to_sleep = ft_atoi(av[4]);
	res->nbr_of_time_they_eat = 0;
	if (av[5])
		res->nbr_of_time_they_eat = ft_atoi(av[5]);
	res->should_program_stop = 0;
	pthread_mutex_init(&res->printf_mutex, NULL);
	pthread_mutex_init(&res->data_mutex, NULL);
	return (res);
}

void	display_program_struct(t_program *program)
{
	printf("nb_philo = %d\n", program->nb_philo);
	printf("time_to_die = %d\n", program->time_to_die);
	printf("time_to_eat = %d\n", program->time_to_eat);
	printf("time_to_sleep = %d\n", program->time_to_sleep);
	printf("nbr_of_time_they_eat = %d\n", program->nbr_of_time_they_eat);
}

void	display_philo_struct(t_philosopher *program)
{
	printf("philo_id = %d\n", program->id);
}
