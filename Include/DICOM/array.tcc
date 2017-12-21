
#ifndef __ARRAY_TCC_
#define __ARRAY_TCC_

/*
template	<class	DATATYPE>
	Array<DATATYPE>	::	Array ()
	{
	ArraySize = 0;
	first = NULL;
	last = NULL;
	ClearType = 1;
	}
*/

template	<class	DATATYPE>
	Array<DATATYPE>	::	Array (UINT	CT)
	{
	ArraySize = 0;
	first = NULL;
	last = NULL;
	LastAccess = NULL;
	LastAccessNumber = 0;
	ClearType = CT;
	}

template	<class	DATATYPE>
	Array<DATATYPE>	::	~Array()
	{
	if(ClearType == 1)
		{
			RemoveAll();
		}
	}

template	<class	DATATYPE>
DATATYPE	&	Array<DATATYPE>	::	Add(DATATYPE	&Value)
	{

	// record current end-of-chain element

	DataLink<DATATYPE>	*dl = last;

	// chain on new element at tail of chain

	last = new DataLink<DATATYPE>;
	last->Data = Value;

	// set new element's backward pointer to point to former
	// end-of-chain element

	last->prev = dl;

	// set former end-of-chain's next pointer to point to new element

	if(dl)
	{
		dl->next = last;
	}
	else
	{
		// there was previously no "last" element so the one just
		// allocated must be the first

		first = last;
	}

	++ArraySize;
	return ( Value );
	}

template	<class	DATATYPE>
DATATYPE	&	Array<DATATYPE>	::	Get(unsigned int	Index)
	{
	DataLink<DATATYPE>	*dl;
	unsigned	int		rIndex = Index;
	
	if ( Index >= ArraySize )
		{
		//fprintf(stderr, "Returning NULL Data\n");
		//return ( NullData );
		dl = NULL;
		return ( (DATATYPE &) *dl );	// Invoke a seg fault
		}

	if ( LastAccess )
		{
		if ((LastAccessNumber + 1) == Index )
			{
			LastAccess = LastAccess->next;
			++LastAccessNumber;
			return ( LastAccess->Data );
			}
		}

	// locate requested element by following pointer chain
	// decide which is faster -- scan from head or scan from tail

	if(Index < ArraySize / 2)
		{

		// requested element closer to head -- scan forward

		dl = first;
		++Index;
		while(--Index > 0)
			{
			dl = dl->next;
			}
		}
		else
		{

		// requested element closer to tail -- scan backwards

		dl = last;
		Index = (ArraySize - Index);
		while(--Index > 0)
			{
			dl = dl->prev;
			}
		}

	LastAccess = dl;
	LastAccessNumber = rIndex;
	return ( dl->Data );
	}

template	<class	DATATYPE>
BOOL	Array<DATATYPE>	::	RemoveAt(unsigned int Index)
	{
	DataLink<DATATYPE>	*dl;

	LastAccess = NULL;
	LastAccessNumber = 0;

	if( Index >= ArraySize )
		{
		//fprintf(stderr, "Attempting to remove non-existance node\n");
		//return ( NullData );
		return( FALSE );
		}

	// follow pointer chain from head or tail to requested element
	// depending upon which chain is shorter

	if(Index < ArraySize / 2)
		{
		// element closer to head => follow chain forward from first
		dl = first;
		++Index;
		while(--Index > 0)
			{
			dl = dl->next;
			}
		}
		else
		{
		// element closer to tail => follow chain backward from last
		dl = last;
		Index = (ArraySize - Index);
		while(--Index > 0)
			{
			dl = dl->prev;
			}
		}

	// relink chain around element to be deleted
	// fix first or last pointer if element to delete is first or last

	if(dl->prev)
		dl->prev->next = dl->next;
	else
		first = dl->next;

	if(dl->next)
		dl->next->prev = dl->prev;
	else
		last = dl->prev;

	delete dl;
	--ArraySize;
	return ( TRUE );
	}

/****************************************************************
 * Test Functions
 *
 *
 ****************************************************************/

#	ifdef	BUILD_TEST_UNITS

int
testfunc()
	{
	unsigned	int	Index;
	Array<int>	AInt;

	Index = 0;
	while(Index < 10)
		{
		AInt.Add(Index * 2);
		++Index;
		}
	Index = 0;
	while(Index < 10)
		{
		printf("Val: %d : %d\n", Index, AInt.RemoveAt(0));
		++Index;
		}
	return ( 0 );
	}

int
main()
	{
	testfunc();
	fprintf(stderr, "Display Should be like 'Val: x: 2*x' with x[0..9]\n");
	}

#	endif
#endif