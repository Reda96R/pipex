/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_processes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:12:01 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/17 21:43:40 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_fchild(t_process_info p_info, char *av[], char *env[])
{
	dup2(p_info.ends[1], 1);
	close (p_info.ends[0]);
	dup2(p_info.infile, 0);
	p_info.cmd_args = ft_split(av[2], ' ');
	p_info.cmd = ft_cmdfinder(p_info.cmd_path, p_info.cmd_args[0]);
	if (!p_info.cmd)
	{
		ft_free_child(&p_info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	execve(p_info.cmd, p_info.cmd_args, env);
}

void	ft_schild(t_process_info p_info, char *av[], char *env[])
{
	dup2(p_info.ends[0], 0);
	close (p_info.ends[1]);
	dup2(p_info.outfile, 1);
	p_info.cmd_args = ft_split(av[3], ' ');
	p_info.cmd = ft_cmdfinder(p_info.cmd_path, p_info.cmd_args[0]);
	if (!p_info.cmd)
	{
		ft_free_child(&p_info);
		ft_msg(ERR_CMD);
		exit(1);
	}
	if (execve(p_info.cmd, p_info.cmd_args, env) == -1)
		perror("execve failed");
}
