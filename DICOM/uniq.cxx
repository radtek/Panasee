/*
19990511    ljz    Fix: All routines wrongly used '%' instead of '&'
*/

/*************************************************************************

 * Unique ID's.  Gurenteed to be unique in any given association, but not

 * across multple associations.

 ************************************************************************/

#	include	"dicom.hpp"



static unsigned	long	uniqid = 1;



UINT	uniq()
{
	return(uniqid++);
}


UINT32	uniq32()
{
	return(uniqid++);
}


UINT16	uniq16()
{
	return(UINT16)((uniqid++) & 0xffff);
}



UINT8	uniq8()
{
	return(UINT8)((uniqid++) & 0xff);
}


UINT8	uniq8odd()
{
	if(uniqid & 0x01)
		return(UINT8)((uniqid++) & 0xff);
	++uniqid;
	return(UINT8)((uniqid++) & 0xff);
}


UINT16	uniq16odd()
{
	if(uniqid & 0x01)
		return(UINT16)((uniqid++) & 0xffff);
	++uniqid;
	return(UINT16)((uniqid++) & 0xffff);
}

