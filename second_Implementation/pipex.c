/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:07:47 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/18 16:46:47 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_cmd_finder(char **path, char *cmd)
{
	char	*str;
	char	*command;

	if (!access(cmd, 0))
		return (cmd);
	while (path && *path)
	{
		str = ft_strjoin(*path, "/");
		command = ft_strjoin(str, cmd);
		free(str);
		if (!access(command, 0))
			return (command);
		free(command);
		path++;
	}
	return (NULL);
}

char	*ft_path_finder(char *env[])
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (!*env)
		return (0);
	return (*env + 5);
}

void	ft_close_pipe(t_process_info *p_info)
{
	close(p_info->ends[0]);
	close(p_info->ends[1]);
}

void	pipex(char *av[], char *env[], t_process_info	*p_info)
{
	pipe (p_info->ends);
	if (p_info->ends < 0)
		ft_errormsg(ERR_PIPE);
	p_info->path = ft_path_finder(env);
	// if (!p_info.path)
	// 	ft_errormsg(ERR_ENV);
	p_info->cmd_path = ft_split(p_info->path, ':');
	p_info->fchild = fork();
	if (!p_info->fchild)
		ft_fchild(p_info, av, env);
	p_info->schild = fork();
	if (!p_info->schild)
		ft_schild(p_info, av, env);
	ft_close_pipe(p_info);
	waitpid(p_info->fchild, &p_info->status, 0);
	waitpid(p_info->schild, &p_info->status, 0);
	ft_free_parent(p_info);
}
