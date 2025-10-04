#pragma once

#include <stddef.h>
#include "mbedtls/threading.h"

void * mbedtls_platform_calloc( size_t nmemb, size_t size );
void mbedtls_platform_free( void * ptr );

int mbedtls_platform_send( void * ctx,
                           const unsigned char * buf,
                           size_t len );

int mbedtls_platform_recv( void * ctx,
                           unsigned char * buf,
                           size_t len );

void mbedtls_platform_mutex_init( mbedtls_threading_mutex_t * pMutex );
void mbedtls_platform_mutex_free( mbedtls_threading_mutex_t * pMutex );
int mbedtls_platform_mutex_lock( mbedtls_threading_mutex_t * pMutex );
int mbedtls_platform_mutex_unlock( mbedtls_threading_mutex_t * pMutex );
