/* -MODULE----------------------------------------------------------------------
Phorward Foundation Toolkit
Copyright (C) 2006-2014 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ contact<at>phorward<dash>software<dot>com
All rights reserved. See LICENSE for more information.

File:	scan.c
Usage:	Phorward Parsing Library
		THIS SOURCE IS UNDER DEVELOPMENT AND EXPERIMENTAL.
----------------------------------------------------------------------------- */

#include "phorward.h"

pboolean pp_sym_in_input( ppsym* sym, char* start, char** end )
{
	pboolean	ret		= FALSE;

	if( !( sym && start && end ) )
	{
		WRONGPARAM;
		return FALSE;
	}

	if( !sym->type )
		/* Not a valid symbol for recognition */
		return FALSE;

	switch( sym->type )
	{
		case PPSYMTYPE_CCL:
		printf( ">%s<\n", start );
			if( p_ccl_test( sym->ccl, *start ) )
			{
				*end = start + 1;
				return TRUE;
			}
			break;

		case PPSYMTYPE_STRING:
			if( !strncmp( sym->name, start, strlen( sym->name ) ) )
			{
				*end = start + strlen( sym->name );
				return TRUE;
			}
			break;

		case PPSYMTYPE_SPECIAL:
			/* TODO: Other special symbols? */
			if( ! **end )
				return TRUE;

			break;

		default:
			MISSINGCASE;
	}

	return FALSE;
}
