/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:57:54 by adichou           #+#    #+#             */
/*   Updated: 2025/09/14 22:22:50 by adichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	get_forks(t_philosopher *ps)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (ps->l_fork < ps->r_fork)
	{
		first = ps->l_fork;
		second = ps->r_fork;
	}
	else
	{
		first = ps->r_fork;
		second = ps->l_fork;
	}
	take_fork(ps, first);
	if (ps->program->nb_philo == 1)
	{
		philo_sleep(ps->program, ps->program->time_to_die);
		pthread_mutex_lock(&(ps->program->printf_mutex));
		printf("%ld %d died\n", get_p_time(ps), ps->id);
		pthread_mutex_unlock(&(ps->program->printf_mutex));
		ps->program->should_program_stop = 1;
		pthread_mutex_unlock(first);
		pthread_exit(NULL);
	}
	take_fork(ps, second);
}

void	put_forks_back(t_philosopher *philosoph)
{
	pthread_mutex_unlock(philosoph->l_fork);
	if (philosoph->program->nb_philo > 1)
		pthread_mutex_unlock(philosoph->r_fork);
}

void	start(t_philosopher *philosoph)
{
	int	max_meals;

	max_meals = philosoph->program->nbr_of_time_they_eat;
	while (!philosoph->program->should_program_stop)
	{
		get_forks(philosoph);
		eat(philosoph);
		put_forks_back(philosoph);
		if (max_meals > 0)
		{
			pthread_mutex_lock(&philosoph->program->data_mutex);
			if (philosoph->meals_eaten >= max_meals)
			{
				pthread_mutex_unlock(&philosoph->program->data_mutex);
				break ;
			}
			pthread_mutex_unlock(&philosoph->program->data_mutex);
		}
		psleep(philosoph);
		think(philosoph);
	}
}

void	*run_philo(void *arg)
{
	t_philosopher	*philosoph;

	philosoph = (t_philosopher *)arg;
	if (philosoph->id % 2 != 0)
		start(philosoph);
	else
	{
		usleep(philosoph->program->time_to_eat * 500);
		start(philosoph);
	}
	pthread_exit(NULL);
}

int	check_death(t_philosopher *philos, t_program *prog)
{
	int		i;
	long	now;

	i = 0;
	while (i < prog->nb_philo)
	{
		pthread_mutex_lock(&prog->data_mutex);
		now = get_time();
		if (!prog->should_program_stop
			&& (now - philos[i].last_meal_time) > prog->time_to_die)
		{
			prog->should_program_stop = 1;
			pthread_mutex_unlock(&prog->data_mutex);
			pthread_mutex_lock(&prog->printf_mutex);
			printf("%ld %d died\n", get_p_time(&philos[i]), philos[i].id);
			pthread_mutex_unlock(&prog->printf_mutex);
			return (1);
		}
		pthread_mutex_unlock(&prog->data_mutex);
		i++;
	}
	return (0);
}
