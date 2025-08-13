/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2022, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_string.с                                                                               */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

#ifndef AK_HAVE_STRING_H
/* ----------------------------------------------------------------------------------------------- */
/*                        Реализация функций, определяемых в string.h                              */
/* ----------------------------------------------------------------------------------------------- */
 void *memset( void *s, int c, size_t n )
{
     size_t i;
     ak_int8 *sp = (ak_int8 *)s;

     if( sp != NULL ) {
         for( i = 0; i < n; i++ ) sp[i] = (ak_int8)c;
     }

 return s;
}

/* ----------------------------------------------------------------------------------------------- */
 void *memcpy( void *dest, const void *src, size_t n )
{
     size_t i;
     ak_uint8 *dp = (ak_uint8 *)dest;
     ak_uint8 *sp = (ak_uint8 *)src;

     if( dp != NULL ) {
         for( i = 0; i < n; i++ ) dp[i] = sp[i];
     }

 return dest;
}

/* ----------------------------------------------------------------------------------------------- */
 size_t strlen( const char *s )
{
     size_t count = 0;
     char *sptr = (char *)s;

     if( sptr == NULL ) return 0;
     for( ; *sptr != '\0'; ++count, ++sptr ) {
         if( count >= AK_DEFAULT_STATIC_BUFFER_SIZE ) break;
     };

  return count;
}

/* ----------------------------------------------------------------------------------------------- */
/* Copyright (c) 1989 The Regents of the University of California.                                 */
 int strncmp( const char *s1, const char *s2, size_t n )
{
    if( n == 0 ) return (0);
    do {
        if( *s1 != *s2++ )
            return( *(unsigned char *)s1 - *(unsigned char *)--s2 );
        if( *s1++ == 0 )
            break;
    } while( --n != 0 );
    return( 0 );
}
#endif

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                    ak_string.с  */
/* ----------------------------------------------------------------------------------------------- */
