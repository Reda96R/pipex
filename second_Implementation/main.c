/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:56:44 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/18 17:11:34 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char *av[], char *env[])
{
	t_process_info	p_info;

	if (ac == 5)
	{
		p_info.infile = open(av[1], O_RDONLY);
		p_info.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_WRONLY, 0666);
		if (ft_file_checker(&p_info, ac, av))
			pipex(av, env, &p_info);
	}
	else
		ft_msg(ERR_INPUT);
	return (0);
}
