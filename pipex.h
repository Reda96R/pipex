/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:52:13 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/12 17:22:16 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <stdio.h>
//# include <get_next_line.h>

# define ERR_FORK "Fork"
# define ERR_INFILE "infile error"
# define ERR_OUTFILE "outfile error"
# define ERR_INPUT "invalide input.\n"
# define ERR_PIPE "pipe error"
# define ERR_CMD "command not found.\n"

typedef struct s_process_info
{
    pid_t   pid;
    int     ends[2];
    int     infile;
    int     outfile;
    char    *path;
    char    **cmd_path;
    char    **cmd_args;
    char    *cmd;
}t_process_info;

//ft_processes.c
void    ft_child(t_process_info process_info, char *av[], char *env[]);
void    ft_parent(t_process_info process_info, char *av[], char *env[]);

//ft_helpers.c
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char *s, char c);
int     ft_strlen(char *str);

//ft_errors.c
int     ft_msg(char *err);
void    ft_errormsg(char *err);
#endif
