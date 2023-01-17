/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:52:13 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/17 21:37:23 by rerayyad         ###   ########.fr       */
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
# include <errno.h>

# define ERR_FORK "Fork"
# define ERR_INFILE "infile error"
# define ERR_OUTFILE "outfile error"
# define ERR_INPUT "invalide input: More or less than 5 arguments.\n"
# define ERR_PIPE "pipe error"
# define ERR_CMD "command not found.\n"
# define ERR_ENV "command not found"
# define ERR_EXECVE "command not found"

typedef struct s_p_info
{
	pid_t	fchild;
	pid_t	schild;
	int		ends[2];
	int		infile;
	int		status;
	int		outfile;
	char	*path;
	char	**cmd_path;
	char	**cmd_args;
	char	*cmd;
}t_process_info;

//ft_processes.c
void	ft_fchild(t_process_info p_info, char *av[], char *env[]);
void	ft_schild(t_process_info p_info, char *av[], char *env[]);

//ft_free.c
void	ft_free_parent(t_process_info *p_info);
void	ft_free_child(t_process_info *p_info);

//ft_helpers.c
char	**ft_split(char *s, char c);
int		ft_strlen(char *str);
char	*ft_cmdfinder(char **path, char *cmd);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, int n);

//ft_errors.c
int		ft_msg(char *err);
void	ft_errormsg(char *err);
#endif
