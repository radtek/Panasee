//���ļ���Ҫ�������һЩ�����ͷ�ļ�
//�����õ��Ŀ�:socket,�������һЩ������ģ�������array,farray,buffer�ȵ�

// Base Class Include Header
extern	"C"
	{
#	include	<stdio.h>
#	include	<string.h>
	};
#	include	<windows.h>
#	include	<winsock.h>
#	include	<share.h>
#	include	"cctypes.h"
#	include	"safemem.h"
#	include	"util.h"
#	include	"array.thh"
#	include	"array.tcc"
#	include	"pqueue.thh"
#	include	"pqueue.tcc"
#	include	"farray.thh"
#	include	"buffer.thh"

// Sometimes it's easier to do something like this than guess
// for all the various systems.. 
#ifndef	SEEK_SET
#	define	SEEK_SET	0
#endif

