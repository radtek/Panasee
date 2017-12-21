//#	include	"common.h"

char	*
ReverseLookup(int	tk)
	{
	switch ((int) tk)
		{
		case	TK_EOF:	return("TK_EOF");
		case	TK_END:	return("TK_END");
		case	TK_ARRAY: return("TK_ARRAY");
		case	TK_CLASS: return("TK_CLASS");
		case	TK_COMPOSITE: return("TK_COMPOSITE");

		default:
			return("undefined");
		}
	}



ostream	&	operator	<< (ostream	&os, Token	&tok)
	{
	os << "(" << tok.LineNumber << ") ["  << ReverseLookup((int) tok)
		 << "] '" << (char *) tok << "'";
	return ( os );
	}


void
Error(Token	&tok, char	*context)
	{
	cerr << "Error: (" << context << ") " << tok << "terminating\n";
	exit(1);
	}

