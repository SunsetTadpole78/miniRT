/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:52:59 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 10:58:28 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static int	parse_lines(char *content);
static int	parse_object(char *line);
/* -------------------------------------------------------------------------- */

int	parse_map(char *path)
{
	int		fd;
	char	*content;
	int		code;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (ft_error(MAP_NOT_FOUND_E, ERR_PREFIX, 1));
	content = ft_get_contents(fd);
	if (!content)
		return (ft_error(MALLOC_E, ERR_PREFIX, 2));
	code = parse_lines(content);
	if (code != 0)
		code += 2;
	free(content);
	close(fd);
	return (code);
}

static int	parse_lines(char *content)
{
	char	**lines;
	int		i;
	int		code;

	lines = ft_split(content, '\n');
	if (!lines)
		return (ft_error(MALLOC_E, ERR_PREFIX, 1));
	i = 0;
	code = 0;
	while (lines[i])
	{
		if (lines[i][0])
		{
			code = parse_object(lines[i]);
			if (code != 0)
			{
				code++;
				break ;
			}
		}
		i++;
	}
	ft_free_str_array(lines);
	return (code);
}

static int	parse_object(char *line)
{
	char		**splited;
	int			code;
	void		*(*method)(char **values);
	t_object	*object;

	splited = ft_split_charset(line, " \t\r\v\f");
	if (!splited)
		return (ft_error(MALLOC_E, ERR_PREFIX, 1));
	code = 0;
	method = get_parser_by_id(splited[0]);
	if (!method)
		code = ft_error(OBJ_E, ERR_PREFIX, 2);
	else
	{
		object = method(splited + 1);
		if (!object)
			code = 3;
		else if (!register_object(object))
			code = ft_error(REGISTRATION_E, ERR_PREFIX, 4);
		else
			printf("Register object with id %s\n", object->id);
	}
	ft_free_str_array(splited);
	return (code);
}
