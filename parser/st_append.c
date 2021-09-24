#include "parser.h"

// t_string使う系の関数がいっぱい出てきそうだったら、接頭語st + '_' + 関数名にしたい
// とりあえずparser directoryに入ってる（parser_testで今は使ってるけど、実際に使うとしたらparserの次のphase）
char	*st_append(char *s1, t_string const *s2)
{
	size_t	len_s1;
	char	*rtn;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
		len_s1 = 0;
	else
		len_s1 = ft_strlen(s1);
	rtn = (char *)malloc(sizeof(char) * (len_s1 + s2->len + 1));
	if (!rtn)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len_s1 && s1[j])
		rtn[i++] = s1[j++];
	j = 0;
	while (j < s2->len)
		rtn[i++] = s2->start[j++];
	rtn[i] = '\0';
	free(s1);
	return (rtn);
}
