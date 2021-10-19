#include "utils.h"

char	*strjoin_three(const char *s1, const char *s2, const char *s3)
{
	const size_t	len_s1 = ft_strlen(s1);
	const size_t	len_s2 = ft_strlen(s2);
	const size_t	len_s3 = ft_strlen(s3);
	char			*joined;

	joined = malloc(sizeof(*joined) * (len_s1 + len_s2 + len_s3 + 1));
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len_s1 + len_s2 + len_s3 + 1);
	ft_strlcat(joined, s2, len_s1 + len_s2 + len_s3 + 1);
	ft_strlcat(joined, s3, len_s1 + len_s2 + len_s3 + 1);
	return (joined);
}