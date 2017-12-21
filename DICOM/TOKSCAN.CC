int	linenum = 1;

int
EvalTok(char	*string)
	{
	int	index;

	index = 0;
	while(Tokens[index].TK_Value)
		{
		if(!strcmp(Tokens[index].TK_Name, string))
			return ( Tokens[index].TK_Value );
		++index;
		}
	return ( 0 );
	}

BOOL
__iswhitespace(char	ch)
	{
	switch ( ch )
		{
		//case	'\n':
		case		10:
		case		13:
		case	' ':
		case	'\t':
		case	EOF:
			return ( TRUE );
		}
	return ( FALSE );
	}

Token		GetToken()
	{
	BOOL	Lock;
	Token	Tok;
	char	ch;
	int		index=0;
	char	string [256];

	memset(string, 0, 256);
	Lock = FALSE;
	while ( ! Lock )
		{
		ch = getchar();
		if(ch=='\n')
			++linenum;
		if ( __iswhitespace(ch) )
			{
			if(index)
				{
				strcpy((char*)Tok, string);
				Tok.TK_Value = EvalTok(string);
				Tok.LineNumber = linenum;
				return(Tok);
				}
			else
				{
				if((ch==EOF)||(feof(stdin)))
					{
					Tok.TK_Value = TK_EOF;
					Tok.LineNumber = linenum;
					return ( Tok );
					}
				}
			}
		else
			{
			if(ch==EOF)
				{
				Tok.TK_Value = TK_EOF; return ( Tok );
				}	
			string[index] = ch;
			++index;
			if(index==2)
				{
				if(!strcmp(string, "//"))
					{
					index = 0;
					gout << "//";
					ch = getchar();
					while(ch!='\n')
						{
						gout << ch; ch=getchar();
						if(ch==EOF) { Tok.TK_Value = TK_EOF; return(Tok);}
						}
					gout << "\n";
					++linenum;
					}
				}
			}
		}
	return ( Tok );
	}			

