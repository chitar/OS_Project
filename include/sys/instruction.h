#define NULL 0

char* strtok(char *s, const char *delim);
char * mystrcpy(char *dst, const char *src);

typedef struct Instruction
{
	char ins_type[30];
	char para[6][100];
	int n_para;
}Ins;

Ins read_ins(char* str)
{
	char s[100];
	mystrcpy(s, str);
	Ins ins;
	ins.n_para = 0;
	char* t;
	t = strtok(s, " ");
	mystrcpy(ins.ins_type, t);
	while ((t=strtok(NULL, " "))!=NULL)
	{
		mystrcpy(ins.para[ins.n_para], t);
		ins.n_para++;
	}

	return ins;
		
}



char * mystrcpy(char *dst, const char *src)   //
{
	if (dst == NULL || src == NULL)return NULL;//

	char *ret = dst;  //

	while ((*dst++ = *src++) != '\0'); //

	return ret;
}

char *strtok(char *s, const char *delim)
{
	static char *last;
	char *tok;
	char *ucdelim;
	char *spanp;
	int c, sc;

	/*sΪ�գ������ϴ�ʣ��ֵҲΪ�գ���ֱ�ӷ���NULL������sΪlast��ǰֵ����ֵ��һ��*/
	if (s == NULL && (s = last) == NULL)
		return NULL;


	int found = 0;//�Ƿ��ҵ���delimƥ����ַ�

				  //���������Ĵ�ƥ����ַ�
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;)
	{
		if (c == sc)
			goto cont;
	}
	if (c == 0)
	{
		last = NULL;
		return NULL;
	}

	tok = s - 1;
	while (!found && *s != '\0')
	{
		ucdelim = (char *)delim;
		while (*ucdelim)
		{
			if (*s == *ucdelim)
			{
				found = 1;
				*s = '\0';
				last = s + 1;
				break;
			}
			ucdelim++;
		}
		if (!found)
		{
			s++;
			if (*s == '\0')
				last = NULL;
		}
	}

	return tok;
}

