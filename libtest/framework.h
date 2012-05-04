/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Data Differential YATL (i.e. libtest)  library
 *
 *  Copyright (C) 2012 Data Differential, http://datadifferential.com/
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

#pragma once

#include <libtest/signal.h>

/**
  Framework is the structure which is passed to the test implementation to be filled.
  This must be implemented in order for the test framework to load the tests. We call
  get_world() in order to fill this structure.
*/

#include <vector>

class Framework {
public:
  collection_st *collections;

  /* These methods are called outside of any collection call. */
  test_callback_create_fn *_create;
  test_callback_destroy_fn *_destroy;

public:
  test_return_t create();

  /**
    If an error occurs during the test, this is called.
  */
  test_callback_error_fn *_on_error;

  void set_on_error(test_callback_error_fn *arg)
  {
    _on_error= arg;
  }

  test_return_t on_error(const enum test_return_t, void *);

  void set_socket()
  {
    _servers.set_socket();
  }

  void set_sasl(const std::string& username_arg, const std::string& password_arg)
  {
    _servers.set_sasl(username_arg, password_arg);
  }

  libtest::server_startup_st& servers()
  {
    return _servers;
  }
  
  /**
    Runner represents the callers for the tests. If not implemented we will use
    a set of default implementations.
  */
  libtest::Runner *_runner;

  void set_runner(libtest::Runner *arg)
  {
    _runner= arg;
  }

  libtest::Runner *runner();

  void exec();

  libtest::Collection& collection();

  Framework(libtest::SignalThread&, const std::string&);

  virtual ~Framework();

  Framework(libtest::SignalThread&,
            const std::string&,
            const std::string&);

  bool match(const char* arg);

  void *creators_ptr()
  {
    return _creators_ptr;
  }

  libtest::SignalThread& signal()
  {
    return _signal;
  }

  uint32_t sum_total();
  uint32_t sum_success();
  uint32_t sum_skipped();
  uint32_t sum_failed();

  size_t size() 
  {
    return _collection.size();
  }

  uint32_t total() const
  {
    return _total;
  }

  uint32_t success() const
  {
    return _success;
  }

  uint32_t skipped() const
  {
    return _skipped;
  }

  uint32_t failed() const
  {
    return _failed;
  }

private:
  Framework& operator=(const Framework&);

  uint32_t _total;
  uint32_t _success;
  uint32_t _skipped;
  uint32_t _failed;

  libtest::server_startup_st _servers;
  bool _socket;
  void *_creators_ptr;
  unsigned long int _servers_to_run;
  std::vector<libtest::Collection*> _collection;
  libtest::SignalThread& _signal;
  std::string _only_run;
  std::string _wildcard;
};
