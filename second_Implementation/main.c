/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rerayyad <rerayyad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:56:44 by rerayyad          #+#    #+#             */
/*   Updated: 2023/01/17 21:46:21 by rerayyad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_path_finder(char *env[])
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (!*env)
		return (0);
	return (*env + 5);
}

void	ft_close_pipe(t_process_info *p_info)
{
	close(p_info->ends[0]);
	close(p_info->ends[1]);
}

int main(int ac, char *av[], char *env[])
{
	t_process_info	p_info;

	if (ac == 5)
	{
		p_info.infile = open(av[1], O_RDONLY);
		if (p_info.infile < 0)
			ft_errormsg(ERR_INFILE);
		p_info.outfile = open(av[ac - 1], O_TRUNC | O_CREAT | O_RDWR, 0777);
		if (p_info.outfile < 0)
			ft_errormsg(ERR_OUTFILE);
		pipe (p_info.ends);
		if (p_info.ends < 0)
			ft_errormsg(ERR_PIPE);
		p_info.path = ft_path_finder(env);
		if (!p_info.path)
			ft_errormsg(ERR_ENV);
		p_info.cmd_path = ft_split(p_info.path, ':');
		p_info.fchild = fork();
		if (!p_info.fchild)
			ft_fchild(p_info, av, env);
		p_info.schild = fork();
		if (!p_info.schild)
			ft_schild(p_info, av, env);
		ft_close_pipe(&p_info);
		waitpid(p_info.fchild, &p_info.status, 0);
		waitpid(p_info.schild, &p_info.status, 0);
		ft_free_parent(&p_info);
		return (0);
	}
	return (ft_msg(ERR_INPUT));
}
