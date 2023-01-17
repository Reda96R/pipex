/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 14:38:27 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/17 21:43:31 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_parent(t_process_info *p_info)
{
	int	i;

	i = 0;
	close(p_info->infile);
	close(p_info->outfile);
	while (p_info->cmd_path[i])
	{
		free(p_info->cmd_path[i]);
		i++;
	}
	free(p_info->cmd_path);
}

void	ft_free_child(t_process_info *p_info)
{
	int	i;

	i = 0;
	while (p_info->cmd_args[i])
	{
		free(p_info->cmd_args[i]);
		i++;
	}
	free(p_info->cmd_args);
	free(p_info->cmd);
}
