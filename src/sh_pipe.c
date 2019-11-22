/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hastid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 00:50:25 by hastid            #+#    #+#             */
/*   Updated: 2019/11/22 06:20:10 by hastid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_shell.h"

t_pipe	*add_pipe(t_cmdl *cmdl)
{
	t_pipe	*pi;

	if (!(pi = (t_pipe *)malloc(sizeof(t_pipe))))
		return (0);
	pi->cmdl = cmdl;
	pi->next = 0;
	return (pi);
}

int		add_pipes(t_pipe **pipes, char *line)
{
	t_tok	*toks;
	t_cmdl	*cmdl;
	t_pipe	*temp;

	if (!(toks = split_tokens(line)))
		return (1);
	if (!(cmdl = save_to_excute(toks)))
		return (1);
	if (!(*pipes))
	{
		if (!(*pipes = add_pipe(cmdl)))
			return (1);
	}
	else
	{
		temp = *pipes;
		while (temp->next)
			temp = temp->next;
		if (!(temp->next = add_pipe(cmdl)))
			return (1);
	}
	free_tokens(toks);
	return (0);
}

int		child_process(int inp, int out, t_pipe *pipes, char **env)
{
	t_fd	*lrd;

	if (dup2(inp, 0) == -1)
		return (ft_perror(0, "duplicate input failed"));
	close(inp);
	if (pipes->next)
		if (dup2(out, 1) == -1)
			return (ft_perror(0, "duplicate output failed"));
	close(out);
	if (pipes->cmdl->rd)
	{
		lrd = pipes->cmdl->lrd;
		while (lrd)
		{
			if (lrd->sec == -1)
				close(lrd->fir);
			else
				dup2(lrd->sec, lrd->fir);
			lrd = lrd->next;
		}
	}
	if (execute(pipes->cmdl, env))
		return (1);
	return (0);
}

int		execute_pipe(t_pipe *pipes, char **env)
{
	int	pid;
	int	inp;
	int	pi[2];

	inp = 0;
	while (pipes)
	{
		if (pipe(pi) == -1)
			return (ft_perror(0, "pipe failed"));
		if ((pid = fork()) == -1)
			return (ft_perror(0, "fork failed"));
		if (pid == 0)
			if (child_process(inp, pi[1], pipes, env))
				return (1);
		wait(&pid);
		if (inp)
			close(inp);
		inp = pi[0];
		close(pi[1]);
		pipes = pipes->next;
	}
	close(pi[0]);
	return (0);
}

int		split_pipe(char *line, char **env)
{
	char	*tmp;
	t_pipe	*pipes;

	pipes = 0;
	while (*line)
	{
		while (*line && check_space(*line))
			line++;
		if (*line)
		{
			if (!(line = sub_line(&tmp, line, '|')))
				return (1);
			if (add_pipes(&pipes, tmp))
				return (1);
			ft_memdel((void **)&tmp);
		}
	}
	if (execute_pipe(pipes, env))
		return (1);
	free_pipes(pipes);
	return (0);
}