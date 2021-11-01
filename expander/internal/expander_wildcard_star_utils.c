#include "expander_internal.h"

char	*remove_multi_stars(char *data)
{
	size_t	i;
	size_t	j;
	int		status;

	i = 0;
	status = OUTSIDE;
	while (data[i])
	{
		status = quotation_status(data[i], status);
		if (status == OUTSIDE && data[i] == '*')
		{
			j = i;
			while (data[j] == '*')
				j++;
			ft_memmove(&data[i + 1], &data[j], ft_strlen(&data[j]) + 1);
		}
		i++;
	}
	return (data);
}

bool	contain_stars_as_pattern(char *original_data)
{
	int		status;
	size_t	i;
	bool	valid_stars;

	i = 0;
	valid_stars = false;
	if (!ft_strcmp(original_data, "*"))
		valid_stars = true;
	while (original_data[i])
	{
		status = quotation_status(original_data[i], status);
		if (status != OUTSIDE && original_data[i] == '*')
			valid_stars = true;
		i++;
	}
	return (valid_stars);
}
