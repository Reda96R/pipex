/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 14:38:27 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/18 16:58:32 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_msg(char *err)
{
	write (2, err, ft_strlen(err));
	exit(1);
}

void	ft_errormsg(char *err)
{
	perror(err);
	exit (1);
}

int	ft_file_checker(t_process_info *p_info, int ac, char *av[])
{
	if (p_info->infile < 0)
	{
		ft_errormsg(av[1]);
		close(p_info->infile);
		return (0);
	}
	if (p_info->outfile < 0)
	{
		ft_errormsg(av[ac - 1]);
		close(p_info->outfile);
		return (0);
	}
	return (1);
}

void	ft_free_parent(t_process_info *p_info)
{
	int	i;

	i = 0;
	close(p_info->infile);
	close(p_info->outfile);
	while (p_info->cmd_path && p_info->cmd_path[i])
	{
		free(p_info->cmd_path[i]);
		i++;
	}
	if (p_info->cmd_path)
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
