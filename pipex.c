/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:56:44 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/13 16:14:06 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *ft_path_finder(char *env[])
{
    while (ft_strncmp("PATH", *env, 4))
        env++;
    return (*env + 5);
}

int main(int ac, char *av[], char *env[])
{   
    t_process_info  process_info;

    if (ac == 5)
    {
        process_info.infile = open(av[1], O_RDONLY);
        if (process_info.infile < 0)
            ft_errormsg(ERR_INFILE);
        process_info.outfile = open (av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0777);
        if (process_info.outfile < 0)
            ft_errormsg(ERR_OUTFILE);
        process_info.path = ft_path_finder(env); 
        process_info.cmd_path = ft_split(process_info.path, ':');
        pipe (process_info.ends);
        process_info.path = ft_path_finder(env);
        process_info.cmd_path = ft_split(process_info.path, ':');
        process_info.pid = fork();
        if (process_info.pid < 0 )
            ft_errormsg(ERR_FORK);
        if (!process_info.pid)
            ft_child(process_info, av, env);
        else
            ft_parent(process_info, av, env);
        ft_free_parent(&process_info);
        return (0);
    }
    return (ft_msg(ERR_INPUT));
}