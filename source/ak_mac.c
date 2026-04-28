/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_mac.c                                                                                  */
/*  - содержит реализацию алгоритмов итерационного сжатия                                          */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt-internal.h>

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_create( ak_mac mctx, const size_t size, ak_pointer ictx,
                            ak_function_clean *clean, ak_function_update *update,
                                                                  ak_function_finalize *finalize )
{
  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                             "using null pointer to mac context" );
  if( !size ) return ak_error_message( ak_error_zero_length, __func__,
                                                    "using zero length of input data block size" );
  if( size > ak_mac_max_buffer_size ) return ak_error_message( ak_error_wrong_length,
                                     __func__, "using very huge length of input data block size" );
  if( ictx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                        "using null pointer to internal context" );
  memset( mctx->data, 0, sizeof( mctx->data ));
  mctx->length = 0;
  mctx->bsize = size;
  mctx->ctx = ictx;
  mctx->clean = clean;
  mctx->update = update;
  mctx->finalize = finalize;

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_destroy( ak_mac mctx )
{
  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                             "using null pointer to mac context" );
  memset( mctx->data, 0, sizeof( mctx->data ));
  mctx->length = 0;
  mctx->bsize = 0;
  mctx->ctx = NULL;
  mctx->clean = NULL;
  mctx->update = NULL;
  mctx->finalize = NULL;

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_clean( ak_mac mctx )
{
  int error = ak_error_ok;
  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                  "using a null pointer to internal mac context" );
  if( mctx->clean == NULL ) return ak_error_message( ak_error_undefined_function, __func__ ,
                                                             "using an undefined clean function" );
  memset( mctx->data, 0, ak_mac_max_buffer_size );
  mctx->length = 0;
  if(( error = mctx->clean( mctx->ctx )) != ak_error_ok )
    ak_error_message( error, __func__, "incorrect cleaning of parent context" );

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_update( ak_mac mctx, const ak_pointer in, const size_t size )
{
  ak_uint8 *ptrin = (ak_uint8 *) in;
  size_t quot = 0, offset = 0, newsize = size;

  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                  "using a null pointer to internal mac context" );
  if( mctx->update == NULL ) return ak_error_message( ak_error_undefined_function, __func__ ,
                                                            "using an undefined update function" );
 /* в начале проверяем, есть ли данные во временном буфере */
  if( mctx->length != 0 ) {
   /* если новых данных мало, то добавляем во временный буффер и выходим */
    if(( mctx->length + newsize ) < mctx->bsize ) {
       memcpy( mctx->data + mctx->length, ptrin, newsize );
       mctx->length += newsize;
       return ak_error_ok;
    }
   /* дополняем буффер до длины, кратной bsize */
    offset = mctx->bsize - mctx->length;
    memcpy( mctx->data + mctx->length, ptrin, offset );

   /* обновляем значение контекста функции и очищаем временный буффер */
    mctx->update( mctx->ctx, mctx->data, mctx->bsize );
    memset( mctx->data, 0, mctx->bsize );
    mctx->length = 0;
    ptrin += offset;
    newsize -= offset;
  }

 /* теперь обрабатываем входные данные с пустым временным буффером */
  if( newsize != 0 ) {
    quot = newsize/mctx->bsize;
    offset = quot*mctx->bsize;
   /* обрабатываем часть, кратную величине bsize */
    if( quot > 0 ) mctx->update( mctx->ctx, ptrin, offset );
   /* хвост оставляем на следующий раз */
    if( offset < newsize ) {
      mctx->length = newsize - offset;
      memcpy( mctx->data, ptrin + offset, mctx->length );
    }
  }

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_finalize( ak_mac mctx,
                    const ak_pointer in, const size_t size, ak_pointer out, const size_t out_size )
{
  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                   "using a null pointer to internal mac context" );
  if( mctx->finalize == NULL ) return ak_error_message( ak_error_undefined_function, __func__ ,
                                                           "using an undefined finalize function" );
 /* начинаем с того, что обрабатываем все переданные данные */
  if( ak_mac_update( mctx, in, size ) != ak_error_ok )
    return ak_error_message( ak_error_get_value(), __func__ , "incorrect updating input data" );
 /* потом обрабатываем хвост, оставшийся во временном буффере, и выходим */
 return mctx->finalize( mctx->ctx, mctx->data, mctx->length, out, out_size );
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_mac_ptr( ak_mac mctx,
                    const ak_pointer in, const size_t size, ak_pointer out, const size_t out_size )
{
  int error = ak_error_ok;
  if( mctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                             "using null pointer to mac context" );
  if(( error = ak_mac_clean( mctx )) != ak_error_ok )
    return ak_error_message( error, __func__, "incorrect cleaning of mac context" );

  if(( error = ak_mac_finalize( mctx, in, size, out, out_size )) != ak_error_ok )
    return ak_error_message( error, __func__, "incorrect updating mac context" );

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                       ak_mac.c  */
/* ----------------------------------------------------------------------------------------------- */
