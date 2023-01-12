/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_processes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:12:01 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/12 17:26:47 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    ft_child(t_process_info process_info, char *av[], char *env[])
{

    dup2(process_info.ends[1], 1);
    close (process_info.ends[0]);
    dup2(process_info.infile, 0);
    process_info.cmd_args = ft_split(av[2], ' ');
    process_info.cmd = ft_getcmd(process_info.cmd_path, process_info.cmd_args[0]);
    if (!process_info.cmd)
    {
        ft_free_child(&process_info);
        ft_msg(ERR_CMD);
        exit(1);
    }
    execve(process_info.cmd, process_info.cmd_args, env);
}

void    ft_parent(t_process_info process_info, char *av[], char *env[])
{
    int status;

    waitpid(-1, &status, 0);
    dup2(process_info.ends[0], 0);
    close (process_info.ends[1]);
    dup2(process_info.outfile, 1);
    process_info.cmd_args = ft_split(av[3], ' ');
    process_info.cmd = ft_getcmd(process_info.cmd_path, process_info.cmd_args[0]);
    if (!process_info.cmd)
    {
        ft_free_parent(&process_info);
        ft_msg(ERR_CMD);
        exit(1);
    }
    execve(process_info.cmd, process_info.cmd_args, env);
}