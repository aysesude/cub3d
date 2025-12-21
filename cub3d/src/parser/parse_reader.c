/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:13:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/21 16:03:11 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	*read_line(int fd)
{
	char	*line;
	int		len;

	line = get_next_line(fd);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

char	*skip_empty_lines(int fd)
{
	char	*line;
	char	*trimmed;

	while (1)
	{
		line = read_line(fd);
		if (!line)
			return (NULL);
		trimmed = ft_strtrim(line, " \t");
		if (trimmed && *trimmed != '\0')
		{
			free(trimmed);
			return (line);
		}
		free(trimmed);
		free(line);
	}
}
