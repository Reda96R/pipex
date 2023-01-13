/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdfinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:25:10 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/13 12:30:13 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char    *ft_cmdfinder(char **path, char *cmd)
{
    char    *str;
    char    *command;

    while (*path)
    {
        str = ft_strjoin(*path, "/");
        command = ft_strjoin(str, cmd);
        free(str);
        if (access(command, 0) == 0)
            return (command);
        free(command);
        path++;
    }
    return (NULL);
}