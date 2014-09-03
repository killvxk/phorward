/*
	This file was automatically generated by pinclude.
	Source files: phorward.tpl.h
*/



#ifndef _PHORWARD_H
#define _PHORWARD_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <stdarg.h>

#ifndef va_copy
#define va_copy( a, b )	(a) = (b)
#endif

#include <limits.h>
#include <time.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef UNICODE
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif



typedef char 					pboolean;	
#ifndef BOOLEAN
#define BOOLEAN					pboolean
#endif

#ifndef boolean
#define boolean 				pboolean
#endif

#ifndef TRUE
#define TRUE					1
#endif

#ifndef FALSE
#define FALSE					0
#endif

#define BOOLEAN_STR( b ) 		( ( b ) ? "TRUE" : "FALSE" )
#define TRUEBOOLEAN( b ) 		( ( b ) ? TRUE : FALSE )


#define ERR_OK					0		
#define ERR_FAILURE				-1		
#define ERR_PARMS				-2		
#define ERR_MEM					-3		
#define ERR_SYSTEM				-4		
#define ERR_UNIMPL				-5		
#define ERR_OTHER				-6		


#define OUTOFMEM				fprintf( stderr, \
									"%s, %d: Ran out of memory\n", \
										__FILE__, __LINE__ ), exit( 1 )

#define WRONGPARAM				fprintf( stderr, \
									"%s, %d: Function called with wrong or " \
									"incomplete parameters, fix your call!\n", \
										__FILE__, __LINE__ )

#define MISSINGCASE				fprintf( stderr, \
									"%s, %d: Missing case enganged, " \
									"please check for correctness.\n", \
									__FILE__, __LINE__ )

#define TODO					fprintf( stderr, \
									"%s, %d: TODO alert! The program ran into "\
									"a module that is not finished yet!\n", \
											__FILE__, __LINE__ )


#ifndef _WIN32
#define PDIRSEP					':'
#define PPATHSEP				'/'
#else
#define PDIRSEP					';'
#define PPATHSEP				'\\'
#endif






#ifndef _DBG_H
#define _DBG_H


#ifdef DEBUG
	#define PROC( name ) \
		static char*	_dbg_proc_name	= name; \
		_dbg_trace( __FILE__, __LINE__, "ENTRY", "%s", _dbg_proc_name )
#else
	#define PROC( name )
#endif


#ifdef DEBUG
	#define RETURN( val ) \
		do \
		{ \
			_dbg_trace( __FILE__, __LINE__, \
				"RETURN", "%s", _dbg_proc_name ); \
			return val; \
		} \
		while( 0 )
#else
	#define RETURN( val ) return val
#endif


#ifdef DEBUG
	#define VOIDRET \
		do \
		{ \
			_dbg_trace( __FILE__, __LINE__, \
				"RETURN", "%s", _dbg_proc_name ); \
			return; \
		} \
		while( 0 )
#else
	#define VOIDRET return
#endif


#ifdef DEBUG
	#define MSG( text ) \
		_dbg_trace( __FILE__, __LINE__, "MSG", "%s", text )
#else
	#define MSG( text )
#endif


#ifdef DEBUG
	#define VARS( name, format, val ) \
		_dbg_trace( __FILE__, __LINE__, "VARS", "%s = >" format "<", name, val )
#else
	#define VARS( name, format, val )
#endif


#ifdef DEBUG
	#define PARMS( name, format, val ) \
		_dbg_trace( __FILE__, __LINE__, "PARMS", \
			"%s = >" format "<", name, val )
#else
	#define PARMS( name, format, val )
#endif

#endif



#ifndef PLIST_H
#define PLIST_H


typedef struct Plistel		plistel;
typedef struct Plist		plist;


struct Plistel
{
	char*					key;
	plist*					list;

	plistel*				prev;
	plistel*				next;

	plistel*				hashnext;
	plistel*				hashprev;
};


struct Plist
{
	int						flags;
#define PLIST_MOD_NONE		0	
#define PLIST_MOD_PTR		1	
#define PLIST_MOD_RECYCLE	2	
#define PLIST_MOD_AUTOSORT	4	
#define PLIST_MOD_EXTKEYS	8	
#define PLIST_MOD_PTRKEYS	16	
#define PLIST_MOD_UNIQUE	32	
#define PLIST_MOD_WCHAR		64	

	int						size;
	int						count;
	int						hashsize;
#define PLIST_DFT_HASHSIZE	64

	int						(*comparefn)( plist*, plistel*, plistel* );
	int						(*sortfn)( plist*, plistel*, plistel* );
	void					(*printfn)( plist* );

	plistel*				unused;

	plistel*				first;
	plistel*				last;
	plistel**				hash;
};





#define plist_for( l, e )	\
	for( (e) = plist_first( l ); (e); (e) = plist_next( e ) )

#endif 



#ifndef STACK_H
#define STACK_H

typedef struct
{
	void*	stack;
	size_t	size;
	size_t	top;
	size_t	count;
	size_t	step;
} pstack;

#endif





#ifndef _XML_H
#define _XML_H

#define XML_BUFSIZE 1024	
#define XML_NAMEM	0x80	
#define XML_TXTM	0x40	
#define XML_DUP		0x20	

typedef struct xml*		XML_T;
struct xml
{
	char*		name;		
	char **	 attr;		
	char*		txt;		
	size_t		off;		
	int			line;		
	XML_T		next;		
	XML_T		sibling;	
	XML_T		ordered;	
	XML_T		child;		
	XML_T		parent;		
	short		flags;		
};

#define xml_next( xml )		\
	( ( xml ) ? xml->next : NULL )

#define xml_next_inorder( xml ) \
	((xml) ? xml->ordered : NULL)

#define xml_name( xml )	\
	( ( xml ) ? xml->name : NULL )

#define xml_txt( xml ) \
	( ( xml ) ? xml->txt : "" )
	
#define xml_line(xml) \
	( ( xml ) ? (xml)->line : 0 )

#define xml_new_d( name ) \
	xml_set_flag( xml_new( pstrdup(name) ),XML_NAMEM )

#define xml_add_child_d( xml, name, off ) \
	xml_set_flag \
	( \
		xml_add_child( xml, pstrdup(name), off ), \
		XML_NAMEM \
	)

#define xml_set_txt_d( xml, txt ) \
	xml_set_flag \
	( \
		xml_set_txt( xml, pstrdup(txt) ), \
		XML_TXTM \
	)

#define xml_set_attr_d( xml, name, value ) \
	xml_set_attr \
	( \
		xml_set_flag( xml, XML_DUP ), \
		pstrdup( name ), \
		pstrdup( value ) \
	)
	
#define xml_add_child_f( xml, name, off ) \
	xml_set_flag \
	( \
		xml_add_child( xml, name, off ), \
		XML_NAMEM \
	)
	
#define xml_set_txt_f( xml, txt ) \
	xml_set_flag \
	( \
		xml_set_txt( xml, txt ), \
		XML_TXTM \
	)

#define xml_set_attr_f( xml, name, value ) \
	xml_set_attr \
	( \
		xml_set_flag( xml, XML_DUP ), \
		pstrdup( name ), \
		(value) \
	)

#define xml_move( xml, dest, off )	\
	xml_insert( xml_cut( xml ),dest,off )

#define xml_remove( xml ) \
	xml_free( xml_cut( xml ) )

#endif 





#define PCCL_MIN			0x0
#if UNICODE
#define PCCL_MAX			0xFFFF
#else
#define PCCL_MAX			0xFF
#endif

typedef struct
{
	wchar_t			begin;
	wchar_t			end;
} pcrange;

typedef struct
{
	int				min;
	int				max;

	plist*			ranges;

	char*			str;
} pccl;





#define PREGEX_ACCEPT_NONE		-1

#define PREGEX_ALLOC_STEP		16


#define PREGEX_STAT_NONE		0
#define PREGEX_STAT_NFA			1
#define PREGEX_STAT_DFA			2


#define PREGEX_MOD_NONE			0		
#define PREGEX_MOD_WCHAR		1		
#define PREGEX_MOD_INSENSITIVE	2		
#define PREGEX_MOD_GLOBAL		4		
#define PREGEX_MOD_STATIC		8		
#define PREGEX_MOD_NO_REF		16		
#define PREGEX_MOD_NO_ERRORS	32		
#define PREGEX_MOD_NO_ANCHORS	64		
#define PREGEX_MOD_GREEDY		128		
#define PREGEX_MOD_NONGREEDY	256		
#define PREGEX_MOD_DEBUG		1024 	


#define PREGEX_ANCHOR_NONE		0	
#define PREGEX_ANCHOR_BOL		1	
#define PREGEX_ANCHOR_EOL		2	
#define PREGEX_ANCHOR_BOW		4	
#define PREGEX_ANCHOR_EOW		8	


enum _regex_ptntype
{
	PREGEX_PTN_NULL,
	PREGEX_PTN_CHAR,
	PREGEX_PTN_SUB,
	PREGEX_PTN_ALT,
	PREGEX_PTN_KLE,
	PREGEX_PTN_POS,
	PREGEX_PTN_OPT
};


typedef struct	_regex_accept	pregex_accept;

typedef struct	_regex_nfa_st	pregex_nfa_st;
typedef struct	_regex_nfa		pregex_nfa;

typedef struct	_regex_dfa_tr	pregex_dfa_tr;
typedef struct	_regex_dfa_st	pregex_dfa_st;
typedef struct	_regex_dfa		pregex_dfa;

typedef enum 	_regex_ptntype	pregex_ptntype;
typedef struct	_regex_ptn		pregex_ptn;

typedef struct	_regex			pregex;
typedef	struct	_regex_range	pregex_range;

typedef struct 	_regex_in		pregex_in;


typedef	int 					(*pregex_fn)( pregex*, pregex_range* );
#define PREGEX_FN_NULL			( (pregex_fn)NULL )


struct _regex_accept
{
	int				accept;		
	pboolean		greedy;		
	int				anchors;	
};


struct _regex_nfa_st
{
	pccl*			ccl;		
	pregex_nfa_st*	next;		
	pregex_nfa_st*	next2;		

	int				ref;		

	pregex_accept	accept;		
};


struct _regex_nfa
{
	plist*			states;		
	int				modifiers;	

	int				ref_count;	
	int				ref_cur;	
};


struct _regex_dfa_tr
{
	pccl*			ccl;		
	unsigned int	go_to;		
};


struct _regex_dfa_st
{
	plist*			trans;		
	pregex_dfa_tr*	def_trans;	
	int*			ref;		
	int				ref_cnt;	

	pregex_accept	accept;		

	pboolean		done;		
	plist*			nfa_set;	
};


struct _regex_dfa
{
	plist*			states;		
	int				ref_count;	
};



struct _regex_ptn
{
	pregex_ptntype	type;		
	pccl*			ccl;		

	pregex_ptn*		child[ 2 ];	
	pregex_ptn*		next;		

	pregex_accept*	accept;		
};

struct _regex_range
{
	char*			begin;		
	wchar_t*		pbegin;		
	char*			end;		
	wchar_t*		pend;		
	size_t			pos;		
	size_t			len;		
	int				accept;		
	void*			user;		
};


struct _regex
{
	

	short			stat;		
	int				flags;		

	plist*			patterns;	

	union
	{
		pregex_nfa*	nfa;		
		pregex_dfa*	dfa;		
	} machine;

	

	int				age;		

	pregex_fn		match_fn;	

	
	int				last_err;	
	int				match_count;
	int				last_age;	

	char*			last_str;	
	char*			last_pos;	

	pregex_range	range;		
	pregex_range	split;		

	pregex_range*	refs;		
	int				refs_cnt;	

	char*			tmp_str;	
};






typedef enum
{
	PPSYMTYPE_NONTERM,
	PPSYMTYPE_CCL,
	PPSYMTYPE_STRING,
	PPSYMTYPE_SPECIAL
} ppsymtype;

typedef struct _ppsym	ppsym;
typedef struct _ppprod	ppprod;
typedef struct _ppgram	ppgram;

#define PPFLAG_CALLED	1
#define PPFLAG_DEFINED	2
#define PPFLAG_NULLABLE	4
#define PPFLAG_LEFTREC	8

struct _ppprod
{
	int				id;
	ppsym*			lhs;
	plist*			rhs;

	int				flags;

	char*			strval;
};

struct _ppsym
{
	int				id;
	ppsymtype		type;

	char*			name;
	pboolean		emit;

	int				flags;

	plist*			first;

	pccl*			ccl;
	plist*			productions;
};

struct _ppgram
{
	plist*			symbols;
	plist*			productions;

	ppsym*			ws;
	ppsym*			goal;
	ppsym*			eof;
};

typedef struct
{
	enum
	{
		PPMATCH_BEGIN,
		PPMATCH_END
	} 				type;

	ppsym*			sym;
	ppprod*			prod;

	char*			start;
	char*			end;

	int				line;
	int				col;
} ppmatch;







#ifndef PGVALUETYPE_H
#define PGVALUETYPE_H


typedef enum
{
	PGVALUETYPE_NULL,

	
	PGVALUETYPE_CHAR,
	PGVALUETYPE_INT,
	PGVALUETYPE_LONG,
	PGVALUETYPE_ULONG,
	PGVALUETYPE_FLOAT,
	PGVALUETYPE_DOUBLE,

	
	PGVALUETYPE_STRING,
	PGVALUETYPE_WSTRING,

	
	PGVALUETYPE_PTR
} pgvaluetype;


#define PGVALUEFLAG_CONSTANT	16	
#define PGVALUEFLAG_AUTOCONVERT	32	


typedef struct
{
	pgvaluetype	type;				
	short		flags;				

	union
	{
		char	c;
		

		int	i;
		

		long	l;
		

		ulong	ul;
		

		float	f;
		

		double	d;
		

		char*	s;
		
		

		wchar_t*	ws;
		
		

		void*	ptr;
		
	} val;
} pgvalue;


#define pg_value_set_string_d( val, str ) \
			pg_value_set_string( val, pstrdup( str ) )
#define pg_value_set_wstring_d( val, str ) \
			pg_value_set_wstring( val, pwcsdup( str ) )

#endif




typedef struct _pggrammar			pggrammar;
typedef struct _pgproduction		pgprod;
typedef struct _pgsymbol			pgsymbol;
typedef struct _pgtoken				pgtoken;

typedef struct _pgastnode			pgastnode;	
typedef struct _pgast				pgast;

typedef enum
{
	PGSYMTYPE_UNDEFINED,			
	PGSYMTYPE_TERMINAL,				
	PGSYMTYPE_NONTERMINAL			
} pgsymtype;

typedef enum
{
	PGASSOC_NONE,					
	PGASSOC_LEFT,					
	PGASSOC_RIGHT,					
	PGASSOC_NOASSOC					
} pgassoc;

typedef enum
{
	PGPARADIGM_UNDEFINED,			
	
	PGPARADIGM_LR0,					
	PGPARADIGM_SLR1,				
	PGPARADIGM_LR1,					
	PGPARADIGM_LALR1,				

	PGPARADIGM_LL1,					
	
	PGPARADIGM_EOP					
} pgparadigm;


struct _pgsymbol
{
	pggrammar*		grammar;		

	int				id;				
	pgsymtype		type;			
	char*			name;			

	pboolean		nullable;		
	pboolean		lrec;			

	plist*			first;			
	plist*			follow;			

	
	pregex_ptn*		ptn;			
	pregex_accept	accept;			

	int				prec;			
	pgassoc			assoc;			

	
	plist*			productions;	

	pboolean		emit;			
};

typedef pgsymbol	pgterm;		
typedef pgsymbol	pgnonterm;	


struct _pgproduction
{
	pggrammar*		grammar;		

	int				id;				

	pgsymbol*		lhs;			
	plist*			rhs;			

	int				prec;			
	pgassoc			assoc;			 
	pboolean		lrec;			

	plist*			select;			

	char*			strval;			
};


struct _pggrammar
{
	plist*			symbols;		
	plist*			productions;	

	int				status;			

#define PGGRAMMAR_STAT_NONE			0	
#define PGGRAMMAR_STAT_LOCKED		1	
#define PGGRAMMAR_STAT_FIRSTDONE	2	
#define PGGRAMMAR_STAT_FOLLOWDONE	4	
#define PGGRAMMAR_STAT_SELECTDONE	8	
#define PGGRAMMAR_STAT_LRECDONE		16	

	pgnonterm*		goal;			
	pgterm*			eoi;			
	pgterm*			error;			

	pregex_ptn*		whitespace;		
};


struct _pgtoken
{
	pgsymbol*		symbol;			
	int				flags;			

	pgvalue*		lexem;			

	int				row;			
	int				col;			
};


struct _pgastnode
{
	pgsymbol*		symbol;		
	pgtoken*		token;		

	pgastnode*		parent;		
	pgastnode*		child;		
	pgastnode*		prev;		
	pgastnode*		next;		
};


struct _pgast
{
	pggrammar*		grammar;	
	pgastnode*		root;		
};



typedef struct _pglexer			pglexer;


struct _pglexer
{
	pggrammar*		grammar;		
	int				flags;			

#define PG_LEXMOD_NONE			0		
#define PG_LEXMOD_SKIP_UNKNOWN	1		
#define PG_LEXMOD_UTF8			2		
#define PG_LEXMOD_WCHAR			4		

	int				states_cnt;		
	wchar_t**			states;			

	
	int				source;
#define PG_LEX_SRCTYPE_FUNC		0		
#define PG_LEX_SRCTYPE_STRING		1		
#define	PG_LEX_SRCTYPE_STREAM		2		

	union
	{
		char*			str;			
		FILE*			stream;			
		unsigned int	(*func)();		
	} src;							

	wchar_t			eof;			
	pboolean		is_eof;			

	
	int				chsize;

	wchar_t*		bufbeg;			
	wchar_t*		bufend;			
	size_t			bufsiz;			

	char*			lexem;			
	size_t			len;			

#define PLEX_BUFSTEP	1024

	int				line;			
	int				column;			

	pgtoken*		token;			
};



typedef struct _pgparser		pgparser;


struct _pgparser
{
	pgparadigm		paradigm;	

	pggrammar*		grammar;	
	pglexer*		lexer;		

	plist*			states;		

	pboolean		optimize;	
	char*			source;		
};




#ifdef __cplusplus
extern "C"
{
#endif



void _dbg_trace( char* file, int line, char* type, char* format, ... );


pboolean plist_init( plist* list, size_t size, int flags );
plist* plist_create( size_t size, int flags );
plist* plist_dup( plist* list );
pboolean plist_erase( plist* list );
pboolean plist_clear( plist* list );
plist* plist_free( plist* list );
plistel* plist_insert( plist* list, plistel* pos, char* key, void* src );
plistel* plist_push( plist* list, void* src );
void* plist_malloc( plist* list );
#if 0
pboolean plist_preallocate( plist* list, int n, int chunk );
#endif
pboolean plist_remove( plist* list, plistel* e );
pboolean plist_pop( plist* list, void* dest );
pboolean plist_take( plist* list, void* dest );
plistel* plist_get( plist* list, int n );
plistel* plist_get_by_key( plist* list, char* key );
plistel* plist_get_by_ptr( plist* list, void* ptr );
int plist_union( plist* all, plist* from );
int plist_diff( plist* left, plist* right );
pboolean plist_subsort( plist* list, plistel* from, plistel* to );
pboolean plist_sort( plist* list );
pboolean plist_set_comparefn( plist* list, int (*comparefn)( plist*, plistel*, plistel* ) );
pboolean plist_set_sortfn( plist* list, int (*sortfn)( plist*, plistel*, plistel* ) );
pboolean plist_set_printfn( plist* list, void (*printfn)( plist* ) );
void* plist_access( plistel* e );
char* plist_key( plistel* e );
plistel* plist_next( plistel* u );
plistel* plist_prev( plistel* u );
plistel* plist_hashnext( plistel* u );
plistel* plist_hashprev( plistel* u );
int plist_offset( plistel* u );
pboolean plist_swap( plistel* a, plistel* b );
plistel* plist_first( plist* l );
plistel* plist_last( plist* l );
int plist_size( plist* l );
int plist_count( plist* l );


void* pmalloc( size_t size );
void* prealloc( void* oldptr, size_t size );
void* pfree( void* ptr );
void* pmemdup( void* ptr, size_t size );


pboolean pstack_init( pstack* stack, size_t size, size_t step );
pstack* pstack_create( size_t size, size_t step );
pboolean pstack_erase( pstack* stack );
pstack* pstack_free( pstack* stack );
void* pstack_push( pstack* stack, void* item );
void* pstack_malloc( pstack* stack );
void* pstack_pop( pstack* stack );
void* pstack_access( pstack* stack, size_t offset );
void* pstack_raccess( pstack* stack, size_t offset );
void* pstack_top( pstack* stack );
void* pstack_bottom( pstack* stack );
int pstack_count( pstack* stack );


pccl* p_ccl_create( int min, int max, char* ccldef );
pboolean p_ccl_compat( pccl* l, pccl* r );
int p_ccl_size( pccl* ccl );
int p_ccl_count( pccl* ccl );
pccl* p_ccl_dup( pccl* ccl );
pboolean p_ccl_testrange( pccl* ccl, wchar_t begin, wchar_t end );
pboolean p_ccl_test( pccl* ccl, wchar_t ch );
pboolean p_ccl_instest( pccl* ccl, wchar_t ch );
pboolean p_ccl_addrange( pccl* ccl, wchar_t begin, wchar_t end );
pboolean p_ccl_add( pccl* ccl, wchar_t ch );
pboolean p_ccl_delrange( pccl* ccl, wchar_t begin, wchar_t end );
pboolean p_ccl_del( pccl* ccl, wchar_t ch );
pboolean p_ccl_negate( pccl* ccl );
pccl* p_ccl_union( pccl* ccl, pccl* add );
pccl* p_ccl_diff( pccl* ccl, pccl* rem );
int p_ccl_compare( pccl* left, pccl* right );
pccl* p_ccl_intersect( pccl* ccl, pccl* within );
pboolean p_ccl_get( wchar_t* from, wchar_t* to, pccl* ccl, int offset );
pboolean p_ccl_parse( pccl* ccl, char* ccldef, pboolean extend );
pboolean p_ccl_erase( pccl* ccl );
pccl* p_ccl_free( pccl* ccl );
char* p_ccl_to_str( pccl* ccl, pboolean escape );
void p_ccl_print( FILE* stream, pccl* ccl, int break_after );


void pregex_dfa_print( pregex_dfa* dfa );
pregex_dfa* pregex_dfa_create( void );
pboolean pregex_dfa_reset( pregex_dfa* dfa );
pregex_dfa* pregex_dfa_free( pregex_dfa* dfa );
int pregex_dfa_from_nfa( pregex_dfa* dfa, pregex_nfa* nfa );
int pregex_dfa_minimize( pregex_dfa* dfa );
int pregex_dfa_match( pregex_dfa* dfa, char* str, size_t* len, int* anchors, pregex_range** ref, int* ref_count, int flags );
int pregex_dfa_to_matrix( wchar_t*** matrix, pregex_dfa* dfa );


int pregex_qmatch( char* regex, char* str, int flags, pregex_range** results );
int pregex_qsplit( char* regex, char* str, int flags, pregex_range** results );
char* pregex_qreplace( char* regex, char* str, char* replace, int flags );


pregex_accept* pregex_accept_init( pregex_accept* accept );
pboolean pregex_check_anchors( char* all, char* str, size_t len, int anchors, int flags );


pregex_nfa_st* pregex_nfa_create_state( pregex_nfa* nfa, char* chardef, int flags );
void pregex_nfa_print( pregex_nfa* nfa );
pregex_nfa* pregex_nfa_create( void );
pboolean pregex_nfa_reset( pregex_nfa* nfa );
pregex_nfa* pregex_nfa_free( pregex_nfa* nfa );
int pregex_nfa_move( pregex_nfa* nfa, plist* hits, wchar_t from, wchar_t to );
int pregex_nfa_epsilon_closure( pregex_nfa* nfa, plist* closure, pregex_accept* accept );
int pregex_nfa_match( pregex_nfa* nfa, char* str, size_t* len, int* anchors, pregex_range** ref, int* ref_count, int flags );
int pregex_nfa_from_string( pregex_nfa* nfa, char* str, int flags, int acc );


pregex_ptn* pregex_ptn_create_char( pccl* ccl );
pregex_ptn* pregex_ptn_create_string( char* str, int flags );
pregex_ptn* pregex_ptn_create_sub( pregex_ptn* ptn );
pregex_ptn* pregex_ptn_create_alt( pregex_ptn* left, ... );
pregex_ptn* pregex_ptn_create_kle( pregex_ptn* ptn );
pregex_ptn* pregex_ptn_create_pos( pregex_ptn* ptn );
pregex_ptn* pregex_ptn_create_opt( pregex_ptn* ptn );
pregex_ptn* pregex_ptn_create_seq( pregex_ptn* first, ... );
pregex_ptn* pregex_ptn_free( pregex_ptn* ptn );
void pregex_ptn_print( pregex_ptn* ptn, int rec );
int pregex_ptn_to_regex( char** regex, pregex_ptn* ptn );
int pregex_ptn_to_nfa( pregex_nfa* nfa, pregex_ptn* pattern );
int pregex_ptn_parse( pregex_ptn** ptn, char* str, int flags );


pregex* pregex_create( void );
pregex* pregex_free( pregex* regex );
pregex* pregex_reset( pregex* regex );
int pregex_compile( pregex* regex, char* pattern, int accept );
int pregex_finalize( pregex* regex );
pregex_range* pregex_match_next( pregex* regex, char* str );
int pregex_match( pregex* regex, char* str, pregex_range** results );
pregex_range* pregex_split_next( pregex* regex, char* str );
int pregex_split( pregex* regex, char* str, pregex_range** results );
char* pregex_replace( pregex* regex, char* str, char* replacement );
pregex_range* pregex_get_range( pregex* regex );
pregex_range* pregex_get_split( pregex* regex );
pregex_range* pregex_get_ref( pregex* regex, int offset );
int pregex_get_match_count( pregex* regex );
int pregex_get_flags( pregex* regex );
pboolean pregex_set_flags( pregex* regex, int flags );
pregex_fn pregex_get_match_fn( pregex* regex );
pboolean pregex_set_match_fn( pregex* regex, pregex_fn match_fn );


char* pregex_range_to_string( pregex_range* range );


int pregex_ref_init( pregex_range** ref, int* ref_count, int ref_all, int flags );
void pregex_ref_update( pregex_range* ref, char* strp, size_t off );


char* pwcs_to_str( wchar_t* str, pboolean freestr );
wchar_t* pstr_to_wcs( char* str, pboolean freestr );
char* pdbl_to_str( double d );
wchar_t* pdbl_to_wcs( double d );


char* pstrcatchar( char* str, wchar_t chr );
char* pstrcatstr( char* dest, char* src, boolean freesrc );
char* pstrncatstr( char* str, char* append, size_t n );
char* pstrreplace( char* str, char* find, char* replace );
char* pstrdup( char* str );
char* pstrndup( char* str, size_t len );
size_t pstrlen( char* str );
int psprintf( char* res, char* fmt, ... );
int pvasprintf( char** str, char* fmt, va_list ap );
char* pasprintf( char* fmt, ... );
char* psetstr( char** str, char* val );
char* pgetstr( char* str );
char* pstrrender( char* tpl, ... );
char* pstrltrim( char* s );
char* pstrrtrim( char* s );
char* pstrtrim( char* s );
int pstrsplit( char*** tokens, char* str, char* sep, int limit );
char* pstrupr( char* s );
char* pstrlwr( char* s );
int pstrcasecmp( char* s1, char* s2 );
int pstrncasecmp( char* s1, char* s2, int n );
int pstrparsechar( wchar_t* retc, char *str, pboolean escapeseq );
#ifdef UNICODE
wchar_t* pwcsdup( wchar_t* str );
int pwcsprintf( wchar_t* res, wchar_t* fmt, ... );
int pvawcsprintf( wchar_t** str, wchar_t* fmt, va_list ap );
wchar_t* pawcsprintf( wchar_t* fmt, ... );
wchar_t* pwcscatchar( wchar_t* str, wchar_t chr );
wchar_t* pwcscatstr( wchar_t* dest, wchar_t* src, boolean freesrc );
wchar_t* pwcsncatstr( wchar_t* str, wchar_t* append, size_t num );
size_t pwcslen( wchar_t* str );
wchar_t* pwcsndup( wchar_t* str, size_t len );
#endif


pboolean u8_isutf( unsigned char c );
int u8_seqlen(char *s);
wchar_t u8_char( char* str );
char* u8_move( char* str, int count );
wchar_t u8_parse_char( char** ch );
int u8_toucs(wchar_t *dest, int sz, char *src, int srcsz);
int u8_toutf8(char *dest, int sz, wchar_t *src, int srcsz);
int u8_wc_toutf8(char *dest, wchar_t ch);
int u8_offset(char *str, int charnum);
int u8_charnum(char *s, int offset);
int u8_strlen(char *s);
wchar_t u8_nextchar(char *s, int *i);
void u8_inc(char *s, int *i);
void u8_dec(char *s, int *i);
int octal_digit(char c);
int hex_digit(char c);
int u8_read_escape_sequence(char *str, wchar_t *dest);
int u8_unescape(char *buf, int sz, char *src);
int u8_escape_wchar(char *buf, int sz, wchar_t ch);
int u8_escape(char *buf, int sz, char *src, int escape_quotes);
char *u8_strchr(char *s, wchar_t ch, int *charn);
char *u8_memchr(char *s, wchar_t ch, size_t sz, int *charn);
int u8_is_locale_utf8(char *locale);


char* pwhich( char* filename, char* directories );
char* pbasename( char* path );
pboolean pfileexists( char* filename );
int map_file( char** cont, char* filename );
int pgetopt( char* opt, char** param, int* next, int argc, char** argv, char* optstr, char* loptstr, int idx );


XML_T xml_child( XML_T xml, char* name );
XML_T xml_idx( XML_T xml, int idx );
char* xml_attr( XML_T xml, char* attr );
long xml_int_attr( XML_T xml, char* attr );
double xml_float_attr( XML_T xml, char* attr );
XML_T xml_vget( XML_T xml, va_list ap );
XML_T xml_get( XML_T xml, ... );
char ** xml_pi( XML_T xml, char* target );
char* xml_decode( char* s, char ** ent, char t );
char* xml_str2utf8( char ** s, size_t* len );
void xml_free_attr( char ** attr );
XML_T xml_parse_str( char* s, size_t len );
XML_T xml_parse_fp( FILE* fp );
XML_T xml_parse_file( char* file );
char* xml_ampencode( char* s, size_t len, char ** dst, size_t* dlen, size_t* max, short a );
char* xml_toxml( XML_T xml );
void xml_free( XML_T xml );
char* xml_error( XML_T xml );
XML_T xml_new( char* name );
XML_T xml_insert( XML_T xml, XML_T dest, size_t off );
XML_T xml_add_child( XML_T xml, char* name, size_t off );
XML_T xml_set_txt( XML_T xml, char* txt );
XML_T xml_set_attr( XML_T xml, char* name, char* value );
XML_T xml_set_int_attr( XML_T xml, char* name, long value );
XML_T xml_set_float_attr( XML_T xml, char* name, double value );
XML_T xml_set_flag( XML_T xml, short flag );
int xml_count( XML_T xml );
int xml_count_all( XML_T xml );
XML_T xml_cut( XML_T xml );


ppsym* pp_sym_create( ppgram* g, ppsymtype type, char* name, char* def );
pboolean pp_prod_append( ppprod* p, ppsym* sym );
ppprod* pp_prod_create( ppgram* g, ppsym* lhs, ... );
ppsym* pp_prod_getfromrhs( ppprod* p, int off );
char* pp_prod_to_str( ppprod* p );
ppgram* pp_gram_free( ppgram* g );
ppgram* pp_gram_create( char* def );
void pp_sym_print( ppsym* s );
void pp_gram_print( ppgram* g );


pboolean pp_ll_parse( plist* ast, ppgram* grm, char* start, char** end );


plist* pp_parser_lr_closure( ppgram* gram, pboolean optimize );
pboolean pp_lr_parse( plist* ast, ppgram* grm, char* start, char** end );


pboolean pp_sym_in_input( ppsym* sym, char* start, char** end );


pboolean pg_value_init( pgvalue* val );
pgvalue* pg_value_create( void );
pboolean pg_value_reset( pgvalue* val );
pgvalue* pg_value_free( pgvalue* val );
pboolean pg_value_set_constant( pgvalue* val, pboolean constant );
pboolean pg_value_get_constant( pgvalue* val );
pboolean pg_value_set_autoconvert( pgvalue* val, pboolean autoconvert );
pboolean pg_value_get_autoconvert( pgvalue* val );


char pg_value_to_char( pgvalue* val );
int pg_value_to_int( pgvalue* val );
long pg_value_to_long( pgvalue* val );
ulong pg_value_to_ulong( pgvalue* val );
float pg_value_to_float( pgvalue* val );
double pg_value_to_double( pgvalue* val );
char* pg_value_to_string( pgvalue* val );
wchar_t* pg_value_to_wstring( pgvalue* val );
void* pg_value_to_ptr( pgvalue* val );
pboolean pg_value_convert( pgvalue* val, pgvaluetype type );


char pg_value_get_char( pgvalue* val );
int pg_value_get_int( pgvalue* val );
long pg_value_get_long( pgvalue* val );
ulong pg_value_get_ulong( pgvalue* val );
float pg_value_get_float( pgvalue* val );
double pg_value_get_double( pgvalue* val );
char* pg_value_get_cstring( pgvalue* val );
char* pg_value_get_string( pgvalue* val );
wchar_t* pg_value_get_wcstring( pgvalue* val );
wchar_t* pg_value_get_wstring( pgvalue* val );
void* pg_value_get_ptr( pgvalue* val );


char pg_value_set_char( pgvalue* val, char c );
int pg_value_set_int( pgvalue* val, int i );
long pg_value_set_long( pgvalue* val, long l );
ulong pg_value_set_ulong( pgvalue* val, ulong ul );
float pg_value_set_float( pgvalue* val, float f );
double pg_value_set_double( pgvalue* val, double d );
char* pg_value_set_cstring( pgvalue* val, char* s );
char* pg_value_set_string( pgvalue* val, char* s );
wchar_t* pg_value_set_wcstring( pgvalue* val, wchar_t* ws );
wchar_t* pg_value_set_wstring( pgvalue* val, wchar_t* ws );
void* pg_value_set_ptr( pgvalue* val, void* ptr );


pgast* pg_ast_create( pggrammar* g );
pgast* pg_ast_free( pgast* ast );
void pg_ast_print( pgast* ast );
pgastnode* pg_ast_get_root( pgast* ast );
pboolean pg_ast_set_root( pgast* ast, pgastnode* root );


pgastnode* pg_astnode_create( pgsymbol* symbol );
pgastnode* pg_astnode_free( pgastnode* node );
void pg_astnode_print( pgastnode* node, FILE* stream );
pgsymbol* pg_astnode_get_symbol( pgastnode* node );
pboolean pg_astnode_set_symbol( pgastnode* node, pgsymbol* symbol );
pgtoken* pg_astnode_get_token( pgastnode* node );
pboolean pg_astnode_set_token( pgastnode* node, pgtoken* token );


void pg_grammar_from_bnf( void );


void PGERR( pggrammar* g, char* file, int line, char* fmt, ... );


pggrammar* pg_grammar_create( void );
pggrammar* pg_grammar_free( pggrammar* g );
pboolean pg_grammar_reset( pggrammar* grammar );
pboolean pg_grammar_lock( pggrammar* grammar );
pboolean pg_grammar_unlock( pggrammar* grammar );
void pg_grammar_print( pggrammar* g );
BOOLEAN pg_grammar_compute_first( pggrammar* g );
BOOLEAN pg_grammar_compute_follow( pggrammar* g );
BOOLEAN pg_grammar_compute_select( pggrammar* g );
BOOLEAN pg_grammar_find_lrec( pggrammar* g );
pgterm* pg_grammar_get_goal( pggrammar* g );
BOOLEAN pg_grammar_set_goal( pggrammar* g, pgnonterm* goal );
pgterm* pg_grammar_get_eoi( pggrammar* g );
BOOLEAN pg_grammar_set_eoi( pggrammar* g, pgterm* eoi );
BOOLEAN pg_grammar_parse_whitespace( pggrammar* grammar, char* str );
BOOLEAN pg_grammar_set_whitespace( pggrammar* grammar, pregex_ptn* whitespace );
pregex_ptn* pg_grammar_get_whitespace( pggrammar* grammar );
pboolean pg_grammar_locked( pggrammar* grammar );


pgnonterm* pg_nonterm_create( pggrammar* grammar, char* name );
pgnonterm* pg_nonterm_drop( pgterm* nonterminal );
pgnonterm* pg_nonterm_get( pggrammar* g, int offset );
pboolean pg_nonterm_get_emit( pgnonterm* nt );
pboolean pg_nonterm_set_emit( pgnonterm* nt, pboolean emit );


pgprod* pg_prod_create( pggrammar* g, pgnonterm* lhs, ... );
pgprod* pg_prod_drop( pgprod* p );
char* pg_prod_to_string( pgprod* p );
void pg_prod_print( pgprod* p, FILE* f );
pboolean pg_prod_append( pgprod* p, pgsymbol* sym );
pboolean pg_prod_append_with_key( pgprod* p, char* key, pgsymbol* sym );
pgprod* pg_prod_get( pggrammar* grammar, int i );
pgprod* pg_prod_get_by_lhs( pgnonterm* lhs, int i );
pgsymbol* pg_prod_get_rhs( pgprod* p, int i );
pgsymbol* pg_prod_get_rhs_by_key( pgprod* p, char* key );
int pg_prod_get_id( pgprod* p );
pggrammar* pg_prod_get_grammar( pgprod* p );
pgnonterm* pg_prod_get_lhs( pgprod* p );
int pg_prod_get_rhs_length( pgprod* p );


pgsymbol* pg_symbol_create( pggrammar* grammar, pgsymtype type, char* name );
pgsymbol* pg_symbol_free( pgsymbol* symbol );
BOOLEAN pg_symbol_reset( pgsymbol* s );
void pg_symbol_print( pgsymbol* symbol, FILE* f );
BOOLEAN pg_symbol_is_term( pgsymbol* symbol );
BOOLEAN pg_symbol_is_nonterm( pgsymbol* symbol );
pgsymbol* pg_symbol_get( pggrammar* g, int i );
pgsymbol* pg_symbol_get_by_id( pggrammar* g, int id );
int pg_symbol_get_id( pgsymbol* s );
pgsymtype pg_symbol_get_type( pgsymbol* s );
pggrammar* pg_symbol_get_grammar( pgsymbol* s );
char* pg_symbol_get_name( pgsymbol* s );


pgterm* pg_term_create( pggrammar* grammar, char* name, char* pattern );
pgterm* pg_term_drop( pgterm* terminal );
pgterm* pg_term_get( pggrammar* g, int offset );
BOOLEAN pg_term_parse_pattern( pgterm* terminal, char* pattern );
BOOLEAN pg_term_set_pattern( pgterm* terminal, pregex_ptn* ptn );
pregex_ptn* pg_term_get_pattern( pgterm* terminal );


pgtoken* pg_token_create( pgsymbol* sym, pgvalue* lexem );
pboolean pg_token_reset( pgtoken* tok );
pgtoken* pg_token_free( pgtoken* tok );
void pg_token_print( pgtoken* tok );
pboolean pg_token_set_symbol( pgtoken* tok, pgsymbol* symbol );
pgsymbol* pg_token_get_symbol( pgtoken* tok );
pboolean pg_token_set_lexem( pgtoken* tok, pgvalue* lexem );
pgvalue* pg_token_get_lexem( pgtoken* tok );


pglexer* pg_lexer_create( pgparser* parser );
pboolean pg_lexer_reset( pglexer* lex );
pglexer* pg_lexer_free( pglexer* lex );
pboolean pg_lexer_set_source( pglexer* lex, int type, void* ptr );
pgtoken* pg_lexer_fetch( pglexer* lex );


pboolean pg_parser_ll_closure( pgparser* parser );
pboolean pg_parser_ll_reset( pgparser* parser );


pboolean pg_parser_ll_parse( pgparser* parser, pgast* ast );


BOOLEAN pg_parser_lr_closure( pgparser* parser );
BOOLEAN pg_parser_lr_reset( pgparser* parser );


pboolean pg_parser_lr_parse( pgparser* parser, pgast* ast );


pgparser* pg_parser_create( pggrammar* grammar, pgparadigm paradigm );
pgparser* pg_parser_free( pgparser* parser );
BOOLEAN pg_parser_generate( pgparser* p );
BOOLEAN pg_parser_parse( pgparser* p );
pgast* pg_parser_parse_to_ast( pgparser* p );
BOOLEAN pg_parser_is_lr( pgparser* p );
BOOLEAN pg_parser_is_ll( pgparser* p );
pggrammar* pg_parser_get_grammar( pgparser* p );
pboolean pg_parser_get_optimize( pgparser* p );
pboolean pg_parser_set_optimize( pgparser* p, pboolean optimize );
char* pg_parser_get_source( pgparser* p );
pboolean pg_parser_set_source( pgparser* p, char* source );


#ifdef __cplusplus
}
#endif

#endif 

