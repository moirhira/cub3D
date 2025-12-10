/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 20:06:42 by moirhira          #+#    #+#             */
/*   Updated: 2025/12/10 15:44:02 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	strip_newline(char *line)
{
	int	len;

	if (!line)
		return ;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
	}
}

char	**append_line(char **map, char *line, int rows)
{
	char	**new_map;
	int		i;

	new_map = malloc((rows + 2) * sizeof(char *));
	if (!new_map)
		return (printf("Error\nFrom malloc!\n"), NULL);
	i = 0;
	while (i < rows)
	{
		new_map[i] = map[i];
		i++;
	}
	new_map[i++] = line;
	new_map[i] = NULL;
	free(map);
	return (new_map);
}

int	process_map_line(t_game *game, char *line, int *rows, int *max_width)
{
	strip_newline(line);
	if (ft_strlen(line) > *max_width)
		*max_width = ft_strlen(line);
	game->map->map_arr = append_line(game->map->map_arr, line, *rows);
	if (!game->map->map_arr)
		return (free(line), 0);
	(*rows)++;
	return (1);
}

int	read_map_lines(t_game *game, int fd, char *first_line, int *rows, int *max_width)
{
	char	*line;

	line = first_line;
	while (line != NULL)
	{
		if (ft_isempty(line))
		{
			if (*rows > 0)
			{
				free(line);
				free_split(game->map->map_arr);
				return (printf("Error\nEmpty line inside map definition.\n"), 0);
			}
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (!process_map_line(game, line, rows, max_width))
			return (0);
		line = get_next_line(fd);
	}
	return (1);
}

int	parse_map(t_game *game, int fd, char *first_line)
{
	int	max_width;
	int	rows;

	max_width = 0;
	rows = 0;
	game->map->map_arr = NULL;
	if (!read_map_lines(game, fd, first_line, &rows, &max_width))
		return (0);
	if (rows == 0)
		return (printf("Error\nMissing map!\n"), 0);
	game->map->height = rows;
	game->map->width = max_width;
	return (validate_map(game));
}
