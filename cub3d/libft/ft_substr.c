/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:47:43 by raktas            #+#    #+#             */
/*   Updated: 2024/10/20 16:03:00 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (s)
	{
		if (start >= ft_strlen(s) || len == 0 || ft_strlen(s) == 0)
			return (ft_strdup(""));
		i = 0;
		while (i < len && s[i + start])
			i++;
		str = (char *) malloc((sizeof(char) * i) + 1);
		if (!(str))
			return (NULL);
		j = 0;
		while (j < i)
		{
			str[j] = s[start + j];
			j++;
		}
		str[j] = '\0';
		return (str);
	}
	return (NULL);
}
