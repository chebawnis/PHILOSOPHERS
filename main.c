/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:21:49 by adichou           #+#    #+#             */
/*   Updated: 2025/05/20 20:31:54 by adichou          ###   ########.fr       */
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

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef struct s_program
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nbr_of_time_they_eat;
	int		should_program_stop;
}	t_program;

typedef struct s_philosopher
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	t_program			*program;
}	t_philosopher;

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sgn;
	int	res;

	i = 0;
	sgn = 1;
	res = 0;
	while ((nptr[i] == ' ') || (nptr[i] >= 9 && nptr[i] <= 13))
		i ++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sgn = -1;
		i++;
	}
	while (nptr[i] != '\0' && nptr[i] >= 48 && nptr[i] <= 57)
	{
		res = res * 10 + (nptr[i] - 48);
		i ++;
	}
	return (res * sgn);
}

int	is_char_num(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);	
}

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

t_program		*init_program(char **av)
{
	t_program	*res;
	
	res = malloc(sizeof(t_program));
	if (!res)
		return NULL;
		res->nb_philo = atoi(av[1]);
	res->time_to_die = atoi(av[2]);
	res->time_to_eat = atoi(av[3]);
	res->time_to_sleep = atoi(av[4]);
	if (av[5])
		res->nbr_of_time_they_eat = atoi(av[5]);
		res->should_program_stop = 0;
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

pthread_mutex_t	*init_all_forks(int	nbr_of_philosophers)
{
	int	i;
	pthread_mutex_t	*res;
	
	i = 0;
	res = malloc(nbr_of_philosophers * sizeof(pthread_mutex_t));
	if (!res)
		return NULL;
	while (i < nbr_of_philosophers)
	{
		pthread_mutex_init(&res[i], NULL);
		i ++;
	}
	return (res);
}

t_philosopher	*init_all_philosophers(char **av, pthread_mutex_t *fork_tab, t_program *program)
{
	t_philosopher	*tab;
	int				i;

	tab = malloc(sizeof(t_philosopher) * atoi(av[1]));
	i = 0;
	if (!tab)
		return NULL;
	tab[i].r_fork = &fork_tab[atoi(av[1])];
	while (i < atoi(av[1]))
	{
		tab[i].id = i + 1;
		tab[i].l_fork = &fork_tab[i + 1];
		if (i != 0)
			tab[i].r_fork = &fork_tab[i];
		tab[i].program = program;
		i ++;
	}
	return (tab);
}

void	*run_philo(void *arg)
{
	t_philosopher	*philosoph;

	philosoph = (t_philosopher *)arg;
	display_philo_struct(philosoph);
	while (1)
	{
		printf("Philosopher number %d is eating\n", philosoph->id);
		usleep(philosoph->program->time_to_eat * 1000);
		printf("Philosopher number %d is sleeping\n", philosoph->id);
		usleep(philosoph->program->time_to_sleep * 1000);
		printf("Philosopher number %d is thinking\n", philosoph->id);
		usleep(philosoph->program->time_to_die * 1000);
	}
	pthread_exit(NULL);
}

void	start_threads(t_philosopher *p_tab, pthread_mutex_t *f_tab, t_program *program)
{
	int	i;

	i = 0;
	while (i < program->nb_philo)
	{
		pthread_create(&p_tab[i].thread, NULL, run_philo, &p_tab[i]);
		i ++;
	}
	i = 0;
	while (i < program->nb_philo)
	{
		pthread_join(p_tab[i].thread, NULL);
		i ++;
	}
}

void	philosophers(char **av)
{
	t_program		*program;
	pthread_mutex_t	*fork_tab;
	t_philosopher	*philo_tab;

	program = init_program(av);
	// display_program_struct(program);
	fork_tab = init_all_forks(atoi(av[1]));
	philo_tab = init_all_philosophers(av, fork_tab, program);
	start_threads(philo_tab, fork_tab, program);
}

int main(int ac, char **av)
{
	if (!verify_args(ac, av))
	{
		printf("Arguments invalides\n");
		return (0);
	}
	philosophers(av);
	return (0);
}
