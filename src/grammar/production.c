/* -MODULE----------------------------------------------------------------------
Phorward Foundation Toolkit
Copyright (C) 2006-2014 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ contact<at>phorward<dash>software<dot>com
All rights reserved. See LICENSE for more information.

File:	production.c
Usage:
----------------------------------------------------------------------------- */

#include "phorward.h"

/* Constructor */

pgproduction* pg_production_create( pgnonterminal* lhs, ... )
{
	pgproduction*	p;
	pgsymbol*		sym;
	va_list			args;

	if( !( lhs && pg_symbol_get_type( lhs ) == PGSYMTYPE_NONTERMINAL ) )
	{
		WRONGPARAM;
		return (pgproduction*)NULL;
	}

	p = (pgproduction*)plist_malloc( lhs->grammar->productions );

	p->rhs = plist_create( 0, PLIST_MOD_PTR |
								PLIST_MOD_UNIQUE |
									PLIST_MOD_RECYCLE );

	p->select = plist_create( 0, PLIST_MOD_PTR | PLIST_MOD_RECYCLE );

	/* Connect with grammar */
	p->grammar = pg_symbol_get_grammar( lhs );

	/* Connect with left-hand side */
	p->lhs = lhs;
	plist_push( lhs->productions, p );

	/* Fill in right-hand side symbols */
	va_start( args, lhs );

	while( ( sym = va_arg( args, pgsymbol* ) ) )
		pg_production_append( p, sym );

	va_end( args );

	return p;
}

/* Destructor */

pgproduction* pg_production_drop( pgproduction* p )
{
	if( !p )
		return (pgproduction*)NULL;

	pfree( p->strval );

	p->select = plist_free( p->select );

	return (pgproduction*)NULL;
}

/* Debug */

char* pg_production_to_string( pgproduction* p )
{
	plistel*	e;
	pgsymbol*	sym;

	if( ( !p ) )
	{
		WRONGPARAM;
		return "";
	}

	p->strval = (char*)pfree( p->strval );

	p->strval = pstrcatstr( p->strval, p->lhs->name, FALSE );
	p->strval = pstrcatstr( p->strval, " : ", FALSE );

	plist_for( p->rhs, e )
	{
		sym = (pgsymbol*)plist_access( e );

		if( e != plist_first( p->rhs ) )
			p->strval = pstrcatstr( p->strval, " ", FALSE );

		if( pg_symbol_is_terminal( sym ) )
			p->strval = pstrcatstr( p->strval, "@", FALSE );

		p->strval = pstrcatstr( p->strval, sym->name, FALSE );
	}

	return p->strval;
}

void pg_production_print( pgproduction* p, FILE* f )
{
	int			i;
	pgsymbol*	sym;

	if( ( !p ) )
	{
		WRONGPARAM;
		return;
	}

	if( !f )
		f = stderr;

	pg_symbol_print( p->lhs, f );
	fprintf( f, " : " );

	for( i = 0; ( sym = pg_production_get_rhs( p, i ) ); i++ )
	{
		if( i > 0 )
			fprintf( f, " " );

		pg_symbol_print( sym, f );
	}

	fprintf( f, "\n" );
}

/* Append right-hand side */

pboolean pg_production_append( pgproduction* p, pgsymbol* sym )
{
	if( !( p && sym ) )
	{
		WRONGPARAM;
		return FALSE;
	}

	if( !plist_push( p->rhs, sym ) )
		return FALSE;

	return TRUE;
}

/* Append right-hand side with key */

pboolean pg_production_append_with_key(
	pgproduction* p, char* key, pgsymbol* sym )
{
	if( !( p && sym ) )
	{
		WRONGPARAM;
		return FALSE;
	}

	if( !( key && *key ) )
		return pg_production_append( p, sym );

	if( !plist_insert( p->rhs, (plistel*)NULL, key, sym ) )
		return FALSE;

	return TRUE;
}

/* Retrieve: By offset from grammar */

pgproduction* pg_production_get( pggrammar* grammar, int i )
{
	if( !( grammar && i >= 0 ) )
	{
		WRONGPARAM;
		return (pgproduction*)NULL;
	}

	return (pgproduction*)plist_access( plist_get( grammar->productions, i ) );
}

/* Retrieve: By offset from keft-hand side */

pgproduction* pg_production_get_by_lhs( pgnonterminal* lhs, int i )
{
	if( !( pg_symbol_is_nonterminal( lhs ) && i >= 0 ) )
	{
		WRONGPARAM;
		return (pgproduction*)NULL;
	}

	return (pgproduction*)plist_access( plist_get( lhs->productions, i ) );
}

/* Retrieve: Right-hand side item */

pgsymbol* pg_production_get_rhs( pgproduction* p, int i )
{
	if( !( p && i >= 0 ) )
	{
		WRONGPARAM;
		return (pgsymbol*)NULL;
	}

	return (pgsymbol*)plist_access( plist_get( p->rhs, i ) );
}

/* Retrieve: Right-hand side item by key */

pgsymbol* pg_production_get_rhs_by_key( pgproduction* p, char* key )
{
	if( !( p && key && *key ) )
	{
		WRONGPARAM;
		return (pgsymbol*)NULL;
	}

	return (pgsymbol*)plist_get_by_key( p->rhs, key );
}

/* Attribute: id */

/* GET ONLY! */
int pg_production_get_id( pgproduction* p )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return -1;
	}

	return plist_offset( plist_get_by_ptr( p->grammar->productions, p ) );
}

/* Attribute: grammar */

/* GET ONLY! */
pggrammar* pg_production_get_grammar( pgproduction* p )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return (pggrammar*)NULL;
	}

	return p->grammar;
}

/* Attribute: lhs */

/* GET ONLY! */
pgnonterminal* pg_production_get_lhs( pgproduction* p )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return (pgnonterminal*)NULL;
	}

	return p->lhs;
}

/* Attribute: rhs_length */

/* GET ONLY! */
int pg_production_get_rhs_length( pgproduction* p )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return 0;
	}

	return plist_count( p->rhs );
}

/* Attribute: asttype */

pgasttype* pg_production_get_asttype( pgproduction* p )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return (pgasttype*)NULL;
	}

	return p->asttype;
}

pboolean pg_production_set_asttype( pgproduction* p, pgasttype* type )
{
	if( !( p ) )
	{
		WRONGPARAM;
		return FALSE;
	}

	p->asttype = type;
	return TRUE;
}
