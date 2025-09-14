#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_program
{
	int             nb_philo;
	int             time_to_die;
	int             time_to_eat;
	int             time_to_sleep;
	int             nbr_of_time_they_eat;
	long            start_time;
	int             should_program_stop;
	pthread_mutex_t printf_mutex;
	pthread_mutex_t data_mutex;
}   t_program;

typedef struct s_philosopher
{
	int             id;
	pthread_t       thread;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
	long            last_meal_time;
	int             meals_eaten;
	int             state;
	t_program       *program;
}   t_philosopher;

/* -------- utils1.c -------- */
long    get_time(void);
long    get_p_time(t_philosopher *philosoph);
void    philo_sleep(t_program *prog, int ms);
int     ft_atoi(const char *nptr);
int     is_char_num(char c);

/* -------- utils2.c -------- */
int     isnum(char *str);
int     verify_args(int ac, char **av);
t_program   *init_program(char **av);
void    display_program_struct(t_program *program);
void    display_philo_struct(t_philosopher *program);

/* -------- utils3.c -------- */
pthread_mutex_t *init_all_forks(int nbr_of_philosophers);
t_philosopher   *init_philos(char **av, pthread_mutex_t *ftab, t_program *p);
void    eat(t_philosopher *philosoph);
void    psleep(t_philosopher *philosoph);
void    think(t_philosopher *philosoph);

/* -------- utils4.c -------- */
void    get_forks(t_philosopher *philosoph);
void    put_forks_back(t_philosopher *philosoph);
void    start(t_philosopher *philosoph);
void    *run_philo(void *arg);
int     check_death(t_philosopher *philos, t_program *prog);

/* -------- main.c -------- */
int     check_all_ate(t_philosopher *philos, t_program *prog);
void    *monitor(void *arg);
void    start_threads(t_philosopher *p_tab, pthread_mutex_t *ftab, t_program *p);
void    philosophers(char **av);

#endif
