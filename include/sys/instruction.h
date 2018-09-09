
#define NULL 0
#define N_MAX_DIR SECTOR_SIZE/sizeof(struct dir_entry)
char* strtok(char *s, const char *delim);
char * mystrcpy(char *dst, const char *src);

typedef struct Instruction
{
	char ins_type[30];
	char para[6][100];
	int n_para;
}Ins;

typedef struct Directorys
{
	struct dir_entry dirs[N_MAX_DIR];
	int end_dirs;
}Dirs;

struct inode* cur_pde;



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

	/*s为空，并且上次剩余值也为空，则直接返回NULL，否则s为last或当前值中有值的一方*/
	if (s == NULL && (s = last) == NULL)
		return NULL;


	int found = 0;//是否找到与delim匹配的字符

				  //处理连续的待匹配的字符
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


void ins_ls(Dirs dirs)
{
	for (int i = 0; i < dirs.end_dirs; i++)
	{
		if (dirs.dirs[i].inode_nr != 0)
		{
			printf("%s", dirs.dirs[i].name);
			if (get_inode(ROOT_DEV, dirs.dirs[i].inode_nr)->i_mode == I_DIRECTORY)
				printf("   ---dir");
			printf("\n");
		}
			
	}
	return;
}

Dirs read_dir(struct inode* pinode)
{
Dirs dirs;
	int dir_blk0_nr = pinode->i_start_sect;
	int nr_dir_blks = (pinode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
		pinode->i_size / DIR_ENTRY_SIZE; /**
										 * including unused slots
										 * (the file has been deleted
										 * but the slot is still there)
										 */
	int m = 0;
	struct dir_entry * pde;
	for (int i = 0; i < nr_dir_blks; i++)
	{
		RD_SECT(pinode->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		for (int j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++)
		{
			if (m >= nr_dir_entries)
				break;
			dirs.dirs[j] = *pde;
			dirs.end_dirs = j + 1;
			
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}
	return dirs;


}

struct inode* find_inode(char* filename,struct inode* pde)
{
	
	Dirs dirs = read_dir(pde);

	for (int i = 0; i < N_MAX_DIR; i++)
	{
		if (dirs.dirs[i].inode_nr != 0)
			if (strcmp(dirs.dirs[i].name, filename) == 0)
				{
					return get_inode(ROOT_DEV,dirs.dirs[i].inode_nr);
				}
				
	}
	return NULL;
}

int ins_cd(char* path)
{
	char filename[512];
	char* t;
	struct inode* pde;
	if (strip_path(filename, path, &pde) == -1)return -1;
	else
	{
		
		cur_pde = pde;
		printf("entered! inode_nr:%d\n",pde->i_num);

		
	}

}

