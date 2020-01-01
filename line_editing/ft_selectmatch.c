/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_selectmatch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llachgar <llachgar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 06:23:37 by llachgar          #+#    #+#             */
/*   Updated: 2020/01/01 11:53:02 by llachgar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include <math.h>

static int		readkey(void)
{
	long key;

	key = 0;
	read(0, &key, sizeof(long));
	return (key);
}

t_data *listmatches(t_data *tmp, t_cmd *l, int count, int sel)
{
    t_point p;
    t_data *choice;
    int i;
    int max_row;
    int max_col;

    p.c = 0;
    p.r = 0;
    i = 0;
    max_col = l->w.ws_col / (l->max_m + 1);
    max_row = ceil((double)count / (double)max_col);
    ft_putstr_fd(CL, 0);
    while (tmp)
    {
        if (i == sel)
            choice = tmp;
        ft_putstr_fd(tgoto(CM, p.c, p.r), 0);
        (sel == i) ? ft_putstr_fd(SO, 0) : 0;
        ft_putstr_fd(tmp->data, 0);
        p.c += (p.r == max_row - 1) ? l->max_m + 2 : 0;
		(p.r == max_row - 1) ? (p.r = 0) : p.r++;
        (sel == i) ? ft_putstr_fd(SE, 0) : 0;
        tmp = tmp->next;
        i++;
    }
    return (choice);
}

int matchkey(int key, int count, int *sel)
{
    int i;

    if (key == BOTTOM_K)
        *sel = (*sel == count - 1) ? 0 : *sel + 1;
    if (key == TOP_K)
       *sel = (*sel == 0) ? count - 1 : *sel - 1; 
    if (key == ESC_K)
    {
        ft_putstr_fd(TE,0);
        ft_putstr_fd(VE,0);
        return (0);
    }
    if (key == INTER_K)
        return (1);
    return (-1);
}

void complet(t_data *choice, t_cmd *l, char *match)
{
    int i;

    i = ft_strlen(match);
    ft_putstr_fd(TE,0);
    ft_putstr_fd(VE,0);
    while (choice->data[i])
    {
        l->key = choice->data[i];
        add_at(l);
        i++;
    }
}

void    print_matches(t_data *matches, t_cmd *l, int count, char *match)
{
    int key;
    int sel;
    int res;
    t_data *choice;

    ft_putstr(TI);
    ft_putstr_fd(VI,0);
    sel = 0;
    while (1337)
    {
        choice = listmatches(matches, l, count, sel);
        key = readkey();
        res = matchkey(key, count, &sel);
        if (res == 0)
            break ;
        if (res == 1)
        {
            complet(choice, l, match);
            break ;
        }
    }
    
}