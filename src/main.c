/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:21:49 by adichou           #+#    #+#             */
/*   Updated: 2025/09/14 23:56:06 by adichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/////////////////////////////////PHILOSOPHERS///////////////////////////////////
//
// Arguments a donner:
//
// -  nbr_of_philosophers -> nombre de philosophes et de fourchettes
// -          time_to_die -> temps en ms avant que le philosophe meurt de faim
// -          time_to_eat -> temps que prends un philosophe pour manger
// -        time_to_sleep -> temps que prends un philosophe a dormir
// - nbr_of_time_they_eat -> nombre de fois quils doivent tous manger puis fin
//
////////////////////////////////////////////////////////////////////////////////

#include "../philosophers.h"

int	check_all_ate(t_philosopher *philos, t_program *prog)
{
	int	i;
	int	all;

	if (prog->nbr_of_time_they_eat <= 0)
		return (0);
	all = 1;
	i = 0;
	while (i < prog->nb_philo)
	{
		pthread_mutex_lock(&prog->data_mutex);
		if (philos[i].meals_eaten < prog->nbr_of_time_they_eat)
			all = 0;
		pthread_mutex_unlock(&prog->data_mutex);
		i++;
	}
	if (all)
	{
		pthread_mutex_lock(&prog->data_mutex);
		prog->should_program_stop = 1;
		pthread_mutex_unlock(&prog->data_mutex);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_philosopher	*philos;
	t_program		*prog;

	philos = (t_philosopher *)arg;
	prog = philos[0].program;
	while (1)
	{
		if (check_death(philos, prog))
			return (NULL);
		if (check_all_ate(philos, prog))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}

void	start_threads(t_philosopher *p_tab, t_program *p)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < p->nb_philo)
	{
		usleep(50);
		if (pthread_create(&p_tab[i].thread, NULL, run_philo, &p_tab[i]) != 0)
		{
			p->should_program_stop = 1;
			return ;
		}
		i ++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, p_tab) != 0)
	{
		p->should_program_stop = 1;
		return ;
	}
	i = 0;
	while (i < p->nb_philo)
		pthread_join(p_tab[i ++].thread, NULL);
	pthread_join(monitor_thread, NULL);
}

void	philosophers(char **av)
{
	t_program		*program;
	pthread_mutex_t	*fork_tab;
	t_philosopher	*philo_tab;
	int				i;

	i = 0;
	program = init_program(av);
	if (!program)
		return ;
	fork_tab = init_all_forks(ft_atoi(av[1]));
	philo_tab = init_philos(av, fork_tab, program);
	start_threads(philo_tab, program);
	while (i < program->nb_philo)
		pthread_mutex_destroy(&fork_tab[i ++]);
	pthread_mutex_destroy(&program->printf_mutex);
	pthread_mutex_destroy(&program->data_mutex);
	free(program);
	free(fork_tab);
	free(philo_tab);
}

int	main(int ac, char **av)
{
	if (!verify_args(ac, av))
	{
		printf("Arguments invalides\n");
		return (1);
	}
	philosophers(av);
	return (0);
}
