/* -MODULE----------------------------------------------------------------------
Phorward Foundation Toolkit
Copyright (C) 2006-2014 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ contact<at>phorward<dash>software<dot>com
All rights reserved. See LICENSE for more information.

File:	system.c
Author:	Jan Max Meyer
Usage:	System functions for various usages.
----------------------------------------------------------------------------- */

#include "phorward.h"

/** Figures out a filepath by searching in a PATH definition.

//filename// is the filename to be searched for.

//directories// is a string specifying the directories to search in.
If this is (char*)NULL, the environment variable PATH will be used and
evaluated by using getenv(). The path can be split with multuple pathes
by a character that depends on the current platform (Unix: ":", Windows: ";").

Returns a static pointer to the absolute path that contains the file specifed
as filename, else it will return (char*)NULL.
*/
char* pwhich( char* filename, char* directories )
{
	static char	path	[ BUFSIZ + 1 ];
	char*			start;
	char*			end;

	if( !( filename && *filename ) )
	{
		WRONGPARAM;
		return (char*)NULL;
	}

	if( !directories )
		directories = getenv( "PATH" );

	start = directories;
	while( start && *start )
	{
		if( !( end = strchr( start, PDIRSEP ) ) )
			end = start + pstrlen( start );

		if( ( end - start ) + pstrlen( filename ) <= BUFSIZ )
		{
			sprintf( path, "%.*s%c%s",
				( end - start ) - ( ( *( end - 1 ) == PPATHSEP ) ? 1 : 0 ),
					start, PPATHSEP, filename );

			if( pfileexists( path ) )
				return path;
		}

		if( ! *end )
			break;

		start = end + 1;
	}

	return (char*)NULL;
}

/** Returns the basename of a file.

//path// is the file path pointer.

Returns a pointer to the basename, which is a part of //path//. */
char* pbasename( char* path )
{
	char*		basename;

	PROC( "pbasename" );
	PARMS( "path", "%s", path );

	basename = strrchr( path, PPATHSEP );
	VARS( "basename", "%s", basename ? basename+1 : path );
	RETURN( ( basename ) ? ++basename : path );
}

/** Checks for file existence.

//filename// is the path to a file that will be checked.

Returns TRUE on success, FALSE if not. */
pboolean pfileexists( char* filename )
{
	PROC( "pfileexists" );
	PARMS( "filename", "%s", filename );

	if( !
	#ifndef _WIN32
		access( filename, F_OK )
	#else
		access( filename, 0 )
	#endif
	)
	{
		MSG( "File exists!" );
		RETURN( TRUE );
	}

	MSG( "File does not exist!" );
	RETURN( FALSE );
}

/** Maps the content of an entire file into memory.

//cont// is the file content return pointer.
//filename// is the path to file to be mapped

The function returns ERR_OK on success, a value >0 if the file could not be
found, or an ERR-define according to its meaning. */
int map_file( char** cont, char* filename )
{
	FILE*	f;
	char*	c;

	PROC( "map_file" );
	PARMS( "cont", "%p", cont );
	PARMS( "filename", "%s", filename );

	/* Check parameters */
	if( !( cont && filename && *filename ) )
	{
		MSG( "Incomplete parameters!" );
		RETURN( ERR_PARMS );
	}

	/* Open file */
	if( !( f = fopen( filename, "rb" ) ) )
	{
		MSG( "File could not be opened - wrong path?" );
		RETURN( 1 );
	}

	/* Allocate memory for file */
	fseek( f, 0L, SEEK_END );
	if( !( c = *cont = (char*)pmalloc( ( ftell( f ) + 1 )
			* sizeof( char ) ) ) )
	{
		MSG( "Unable to allocate required memory" );

		fclose( f );
		RETURN( ERR_MEM );
	}

	/* Read entire file into buffer */
	fseek( f, 0L, SEEK_SET );

	while( !feof( f ) )
		*(c++) = fgetc( f );

	/* Case: File is empty */
	if( c == *cont )
		c++;

	*( c - 1 ) = '\0';

	fclose( f );

	VARS( "Entire file", "%s", *cont );
	MSG( "All right!" );

	RETURN( ERR_OK );
}

/** Implementation of a command-line option interpreter.

This function works similar to the getopt() functions of the GNU Standard
Library, but uses a different style of parameter submit.

It supports both short- and long- option-style parameters.
This function is currently under recent development relating to the issues it
is used for. It can't be seen as compatible or feature-proven, and does not
follow a clear concept right now.

//opt// is a pointer to a buffer with enough space to store the requested
parameter to. For short parameters, this is only one char, for long-parameters
the full name. The string will be zero-terminated.

//param// is a pointer to store a possible parameter value to, if the detected
option allows for parameters.

//next// receives the index in argv of the next evaluated option. It can be
left (pint*)NULL. It points to the next valid index in argv[] after all
parameters have been evaluated. Check it for < argc, to point to valid data.

//argc// is the argument count as taken from the main() function.

//argv// are the argument values as taken from the main() function.

//optstr// contains the possible short-options. This is a string where each
character defines an option. If an option takes a parameter, a colon (:) is
submitted.
E.g. "abc:def:g". The Options "-c" and "-f" will take a parameter that is
returned to param. This parameter can be (char*)NULL.

//loptstr// contains the possible long-options. This is a string containing all
long option names, each separated by a blank. Long options taking parameters
have an attached colon (:) to the name.
E.g. "hello world: next" defines three long options, where option 'world' takes
one parameter that is returned to param. This parameter can be (char*)NULL.

//idx// is the index of the requested option, 0 for the first option behind
argv[0].

The function returns ERR_OK, if the parameter with the given index was
successfully evaluated. It returns 1, if there are sill command-line parameters,
but not as part of options. The parameter param will receive the given pointer.
It returns ERR_FAILURE if no more options could be read, or if an option could
not be evaluated (unknown option). In such case, param will hold a string to the
option that is now known.
*/
pint pgetopt( char* opt, char** param, pint* next,
				pint argc, char** argv, char* optstr,
					char* loptstr, pint idx )
{
	BOOLEAN		has_parm;
	BOOLEAN		found;
	BOOLEAN		lopt;
	int			i;
	char*		str;
	char*		pos;
	char*		del;
	int			cnt		= 0;
	static char	optinfo	[ 2+1 ];

	PROC( "pgetopt" );
	PARMS( "opt", "%p", opt );
	PARMS( "param", "%p", param );
	PARMS( "next", "%p", next );
	PARMS( "argc", "%d", argc );
	PARMS( "argv", "%p", argv );
	PARMS( "optstr", "%s", optstr );
	PARMS( "loptstr", "%s", loptstr );
	PARMS( "idx", "%d", idx );

	if( !( opt && param && idx >= 0 && ( optstr || loptstr ) ) )
	{
		RETURN( ERR_PARMS );
	}

	*param = (char*)NULL;
	if( next )
		*next = 1;

	if( argc < 2 )
		RETURN( 1 );

	for( i = 1; i < argc; i++ )
	{
		found = FALSE;
		str = argv[i];
		lopt = FALSE;

		if( *str == '-' )
		{
			str++;
			if( *str == '-' )
			{
				str++;
				lopt = TRUE;
			}
		}
		else
		{
			*param = str;

			if( next )
				*next = i;

			RETURN( 1 );
		}

		if( !lopt && optstr && *optstr )
		{
			while( *str && idx >= 0 )
			{
				found = FALSE;
				for( pos = optstr; *pos; pos++ )
				{
					if( *pos == ':' )
						continue;

					if( *pos == *str )
					{
						if( *( pos + 1 ) == ':' )
						{
							if( argc > ++i )
							{
								if( !idx )
									*param = argv[ i ];
							}

						}

						if( !idx )
						{
							psprintf( opt, "%c", *pos );

							if( next )
								*next = i + 1;

							RETURN( ERR_OK );
						}

						found = TRUE;
						break;
					}
				}

				if( !found )
				{
					sprintf( optinfo, "-%c", *str );
					*param = optinfo;
					RETURN( ERR_FAILURE );
				}

				idx--;
				str++;
			}
		}
		else if( lopt && loptstr && *loptstr )
		{
			for( pos = loptstr; *pos; pos = ( *del ? del + 1 : del ) )
			{
				if( !( del = strstr( pos, " " ) ) )
					del = pos + pstrlen( pos );

				if( del == pos )
					continue;

				if( del > pos && *( del - 1 ) == ':' )
				{
					has_parm = TRUE;
					del--;
				}
				else
					has_parm = FALSE;

				if( strncmp( pos, str, del - pos ) == 0 )
				{
					if( has_parm && argc > ++i )
					{
						if( !idx )
							*param = argv[ i ];
					}

					if( !idx )
					{
						psprintf( opt, "%.*s", (int)( del - pos ), str );

						if( next )
							*next = i + 1;

						RETURN( ERR_OK );
					}

					found = TRUE;
					break;
				}
			}

			if( !found )
			{
				*param = argv[ i ];

				if( next )
					*next = i + 1;

				RETURN( ERR_FAILURE );
			}

			idx--;
		}
		else
			break;
	}

	if( next )
		*next = i;

	RETURN( ERR_FAILURE );
}

