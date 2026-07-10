/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhedo-ga <vhedo-ga <vhedo-ga@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:05:40 by vhedo-ga          #+#    #+#             */
/*   Updated: 2025/10/24 12:23:47 by vhedo-ga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_store_or_newline(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*ft_append_and_free(char *total, char *buf)
{
	char	*tmp;

	tmp = ft_strjoin(total, buf);
	free(total);
	return (tmp);
}

static char	*ft_read_and_store(int fd, char *total)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	while (ft_store_or_newline(total) == -1)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
			return (ft_handle_read_error(buf, total));
		if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		if (!total)
			total = ft_strdup(buf);
		else
			total = ft_append_and_free(total, buf);
	}
	free(buf);
	return (total);
}

static char	*ft_extract_line(char **total_ptr)
{
	char	*line;
	char	*new_total;
	int		i;

	i = ft_store_or_newline(*total_ptr);
	if (i >= 0)
	{
		line = ft_substr(*total_ptr, 0, i + 1);
		new_total = ft_substr(*total_ptr, i + 1, ft_strlen(*total_ptr) - i - 1);
		free(*total_ptr);
		if (new_total && *new_total)
			*total_ptr = new_total;
		else
		{
			free(new_total);
			*total_ptr = NULL;
		}
	}
	else
	{
		line = ft_substr(*total_ptr, 0, ft_strlen(*total_ptr));
		free(*total_ptr);
		*total_ptr = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*total;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	total = ft_read_and_store(fd, total);
	if (!total)
		return (NULL);
	return (ft_extract_line(&total));
}
