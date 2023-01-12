/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:56:44 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/12 17:23:12 by rerayyad         ###   ########.fr       */
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
        process_info.path = ft_path_finder(env); 
        process_info.cmd_path = ft_split(process_info.cmd_path, ':');
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
}