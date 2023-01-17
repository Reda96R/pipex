/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 14:38:27 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/12 16:42:08 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    ft_free_parent(t_process_info *process_info)
{
    int i;

    i = 0;
    close(process_info->infile);
    close(process_info->outfile);
    while (process_info->cmd_path[i])
    {
        free(process_info->cmd_path[i]);
        i++;
    }
    free(process_info->cmd_path);
}

void    ft_free_child(t_process_info *process_info)
{
    int i;

    i = 0;
    while (process_info->cmd_args[i])
    {
        free(process_info->cmd_args[i]);
        i++;
    }
    free(process_info->cmd_args);
    free(process_info->cmd);
}