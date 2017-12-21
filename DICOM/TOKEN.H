class	Token
	{
	public:
		int	LineNumber;
		int		TK_Value;
		char	Symbol [ MAXLEN + 1 ];
	public:
			Token() { Symbol[0] = '\0'; };
		inline 	operator	char * ()
			{ return (Symbol); };
		inline	operator	int ()
			{ return ((int)TK_Value); };
		inline 	Token	&	operator	= (Token &Tk)
			{ TK_Value = Tk.TK_Value; strcpy(Symbol, Tk.Symbol);
			  LineNumber = Tk.LineNumber;
			  return (*this); };
		inline	BOOL	operator	== (Token &Tk)
			{ if(Tk.TK_Value != TK_Value) return ( FALSE );
			  if(strcmp(Symbol, Tk.Symbol)) return ( FALSE );
			  return ( TRUE ); };
		friend	ostream & operator << (ostream &, Token &);
	};
		
enum	{	TK_EOF	= 1,
			TK_END, TK_ARRAY, TK_CLASS, TK_COMPOSITE, TK_COMMENT };	

typedef	struct	_TOKSYM
	{
	int	TK_Value;
	char	*TK_Name;
	}	TOKSYM;

TOKSYM Tokens[]	=	 {	{ TK_END, "end" },
					{ TK_ARRAY, "array" },
					{ TK_CLASS, "class" },
					{ TK_COMPOSITE, "composite" },
					{ TK_COMMENT, "//" },
					{ 0, "\0" } };

