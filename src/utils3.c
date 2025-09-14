/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:57:14 by adichou           #+#    #+#             */
/*   Updated: 2025/09/14 21:58:41 by adichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

pthread_mutex_t	*init_all_forks(int nbr_of_philosophers)
{
	int				i;
	pthread_mutex_t	*res;

	res = malloc(nbr_of_philosophers * sizeof(pthread_mutex_t));
	if (!res)
		return (NULL);
	i = 0;
	while (i < nbr_of_philosophers)
	{
		if (pthread_mutex_init(&res[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&res[i]);
			free(res);
			return (NULL);
		}
		i ++;
	}
	return (res);
}

t_philosopher	*init_philos(char **av, pthread_mutex_t *ftab, t_program *p)
{
	t_philosopher	*tab;
	int				i;
	int				nb_philo;

	nb_philo = ft_atoi(av[1]);
	tab = malloc(sizeof(t_philosopher) * nb_philo);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < nb_philo)
	{
		tab[i].id = i + 1;
		tab[i].l_fork = &ftab[i];
		tab[i].r_fork = &ftab[(i + 1) % nb_philo];
		tab[i].program = p;
		tab[i].last_meal_time = p->start_time;
		tab[i].meals_eaten = 0;
		tab[i].state = 0;
		i ++;
	}
	return (tab);
}

void	eat(t_philosopher *philosoph)
{
	pthread_mutex_lock(&philosoph->program->data_mutex);
	philosoph->last_meal_time = get_time();
	philosoph->state = 1;
	pthread_mutex_unlock(&philosoph->program->data_mutex);
	pthread_mutex_lock(&(philosoph->program->printf_mutex));
	if (!philosoph->program->should_program_stop)
		printf("%ld %d is eating\n", get_p_time(philosoph), philosoph->id);
	pthread_mutex_unlock(&(philosoph->program->printf_mutex));
	philo_sleep(philosoph->program, philosoph->program->time_to_eat);
	pthread_mutex_lock(&philosoph->program->data_mutex);
	philosoph->meals_eaten ++;
	pthread_mutex_unlock(&philosoph->program->data_mutex);
}

void	psleep(t_philosopher *philosoph)
{
	pthread_mutex_lock(&(philosoph->program->printf_mutex));
	if (!philosoph->program->should_program_stop)
		printf("%ld %d is sleeping\n", get_p_time(philosoph), philosoph->id);
	pthread_mutex_unlock(&(philosoph->program->printf_mutex));
	philo_sleep(philosoph->program, philosoph->program->time_to_sleep);
}

void	think(t_philosopher *philosoph)
{
	pthread_mutex_lock(&(philosoph->program->printf_mutex));
	if (!philosoph->program->should_program_stop)
		printf("%ld %d is thinking\n", get_p_time(philosoph), philosoph->id);
	pthread_mutex_unlock(&(philosoph->program->printf_mutex));
}
