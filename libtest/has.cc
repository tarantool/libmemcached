/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Data Differential YATL (i.e. libtest)  library
 *
 *  Copyright (C) 2012-2013 Data Differential, http://datadifferential.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *      * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *      * Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following disclaimer
 *  in the documentation and/or other materials provided with the
 *  distribution.
 *
 *      * The names of its contributors may not be used to endorse or
 *  promote products derived from this software without specific prior
 *  written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "libtest/yatlcon.h"
#include <libtest/common.h>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#ifndef __INTEL_COMPILER
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif

namespace libtest {

bool has_libmemcached_sasl(void)
{
  return false;
}

bool has_libmemcached(void)
{
#if defined(HAVE_LIBMEMCACHED) && HAVE_LIBMEMCACHED
  if (HAVE_LIBMEMCACHED)
  {
    return true;
  }
#endif

  return false;
}

static char memcached_binary_path[FILENAME_MAX];

static void initialize_memcached_binary_path()
{
  memcached_binary_path[0]= 0;

#if defined(MEMCACHED_BINARY) && defined(HAVE_MEMCACHED_BINARY) && HAVE_MEMCACHED_BINARY
  if (HAVE_MEMCACHED_BINARY)
  {
    std::stringstream arg_buffer;

    char *getenv_ptr;
    if (bool((getenv_ptr= getenv("PWD"))) and strcmp(MEMCACHED_BINARY, "memcached/memcached") == 0)
    {
      arg_buffer << getenv_ptr;
      arg_buffer << "/";
    }
    arg_buffer << MEMCACHED_BINARY;

    if (access(arg_buffer.str().c_str(), X_OK) == 0)
    {
      strncpy(memcached_binary_path, arg_buffer.str().c_str(), FILENAME_MAX);
    }
  }
#endif
}

static pthread_once_t memcached_binary_once= PTHREAD_ONCE_INIT;
static void initialize_memcached_binary(void)
{
  int ret;
  if ((ret= pthread_once(&memcached_binary_once, initialize_memcached_binary_path)) != 0)
  {
    FATAL(strerror(ret));
  }
}

bool has_memcached()
{
  initialize_memcached_binary();

  if (memcached_binary_path[0] and (strlen(memcached_binary_path) > 0))
  {
    return true;
  }

  return false;
}

const char* memcached_binary()
{
  initialize_memcached_binary();

  if (memcached_binary_path[0])
  {
    return memcached_binary_path;
  }

  return NULL;
}

} // namespace libtest
