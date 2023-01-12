/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:09:28 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/12 12:15:37 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int ft_msg(char *err)
{
    write (2, err, ft_strlen(err));   
    exit(1);
}

void ft_errormsg(char *err)
{
    perror(err);
    exit (1);
}