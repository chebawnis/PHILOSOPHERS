/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:21:49 by adichou           #+#    #+#             */
/*   Updated: 2025/05/03 05:45:49 by adichou          ###   ########.fr       */
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
#include <stdlib.h>

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
	printf("ac = %d\n", ac);
	if (ac != 5 && ac != 6)
		return (0);
	if (ft_atoi(av[1]) <= 0 || ft_atoi(av[2]) <= 0
		|| ft_atoi(av[3]) <= 0 || ft_atoi(av[4]) <= 0)
		return (0);
	if (!isnum(av[1]) || !isnum(av[2]) || !isnum(av[3]) || !isnum(av[4]))
		return (0);
	if (ac == 6 && (ft_atoi(av[5]) <= 0 || !isnum(av[5])))
		return (0);
	return (1);
}

typedef struct s_timeval
{
	time_t			tv_sec;
	__suseconds_t	tv_usec;
} t_timeval;

typedef struct s_philosopher
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		l_fork;
	pthread_mutex_t		r_fork;
}	t_philosopher;

typedef struct s_program
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nbr_of_time_they_eat;
}	t_program;


typedef struct s_count_add
{
	pthread_mutex_t		lock;
	int					counter;
}	t_count_add;


void	*function_1(void *arg)
{
	int	i;

	i = 0;
	t_count_add *struct_1;
	struct_1 = (t_count_add *)arg;
	while (i < 100000)
	{
		pthread_mutex_lock(&(struct_1->lock));
		struct_1->counter ++;
		pthread_mutex_unlock(&(struct_1->lock));
		i ++;
	}
	pthread_exit(NULL);
}

void	*function_2(void *arg)
{
	int	i;

	i = 0;
	t_count_add *struct_1;
	struct_1 = (t_count_add *)arg;
	while (i < 100000)
	{
		pthread_mutex_lock(&(struct_1->lock));
		struct_1->counter ++;
		pthread_mutex_unlock(&(struct_1->lock));
		i ++;
	}
	pthread_exit(NULL);
}

// int main(int ac, char **av)
// {
// 	t_count_add		*struct_1;
	
// 	pthread_t		thread_1;
// 	pthread_t		thread_2;

// 	struct_1 = malloc(sizeof(t_count_add));
// 	pthread_mutex_init(&(struct_1->lock), NULL);
// 	struct_1->counter = 0;
		
// 	pthread_create(&thread_1, NULL, function_1, struct_1);
// 	pthread_create(&thread_2, NULL, function_2, struct_1);

// 	pthread_join(thread_1, NULL);
// 	pthread_join(thread_2, NULL);

// 	printf("counter = %d\n", struct_1->counter);
// 	return (0);
// }

t_philosopher	*init_all_philosophers(int	nbr_of_philosophers)
{
	t_philosopher	*tab;
	int				i;

	tab = malloc(sizeof(t_philosopher) * nbr_of_philosophers);
	i = 0;
	if (!tab)
		return NULL;
	while (i < nbr_of_philosophers)
	{
		tab[i].id = i + 1;
		i ++;
	}
	
	return (tab);
}

void	philosophers(char *av)
{
	t_philosopher	*tab;

	tab = init_all_philosophers(av[1]);
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