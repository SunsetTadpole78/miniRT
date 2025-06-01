/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:52:59 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 01:24:12 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static inline int		parse_lines(char *content);
static inline int		parse_object(char *line, int *comment);
static inline int		check_comments(char **splited, int *comment);
static inline void	*get_parser_by_id(char *id);
/* -------------------------------------------------------------------------- */

int	parse_map(char *path)
{
	int		len;
	int		fd;
	char	*content;
	int		code;

	len = ft_strlen(path);
	if (len < 3 || ft_strncmp(path + (len - 3), ".rt", 4) != 0)
		return (ft_error(EXTENSION_E, ERR_PREFIX, 1));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (ft_error(MAP_NOT_FOUND_E, ERR_PREFIX, 2));
	content = ft_get_contents(fd);
	if (!content)
		return (ft_error(MALLOC_E, ERR_PREFIX, 3));
	code = parse_lines(content);
	if (code != 0)
		code += 3;
	free(content);
	close(fd);
	return (code);
}

static int	parse_lines(char *content)
{
	char	**lines;
	int		i;
	int		code;
	int		comment;

	lines = ft_split(content, '\n');
	if (!lines)
		return (ft_error(MALLOC_E, ERR_PREFIX, 1));
	i = 0;
	code = 2;
	comment = 0;
	while (lines[i])
	{
		if (lines[i][0])
		{
			code += parse_object(lines[i], &comment) * 2;
			if (code != 2)
				break ;
		}
		i++;
	}
	ft_free_str_array(lines);
	if (comment)
		return (ft_error(UNCLOSED_COM_E, ERR_PREFIX, 2));
	return (code - 2);
}

static int	parse_object(char *line, int *comment)
{
	char		**splited;
	int			code;
	void		*(*method)(char **values);
	t_object	*object;

	splited = ft_split_charset(line, " \t\r\v\f");
	if (!splited)
		return (ft_error(MALLOC_E, ERR_PREFIX, 1));
	code = 0;
	if (splited[0] && !check_comments(splited, comment))
	{
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
		}
	}
	ft_free_str_array(splited);
	return (code);
}

static inline int	check_comments(char **splited, int *comment)
{
	if (ft_strncmp(splited[0], "//", 2) == 0
		|| ft_strncmp(splited[0], "#", 1) == 0)
		return (1);
	else if (ft_strncmp(splited[0], "/*", 2) == 0)
	{
		*comment = 1;
		return (1);
	}
	else if (ft_strncmp(splited[0], "*/", 3) == 0)
	{
		if (!*comment)
			return (ft_error(UNOPENED_COM_E, ERR_PREFIX, 1));
		*comment = 0;
		return (1);
	}
	return (*comment);
}

static inline void	*get_parser_by_id(char *id)
{
	t_methods	*methods;

	methods = get_methods_by_id(id);
	if (!methods)
		return (NULL);
	return (methods->parser);
}
