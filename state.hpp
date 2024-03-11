// Copyright 2011 Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
// * Neither the name of Google Inc. nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/// \file state.hpp
/// Provides the state wrapper class for the Lua C state.

#if !defined(LUTOK_STATE_HPP)
#define LUTOK_STATE_HPP

#include <string>

#if defined(_LIBCPP_VERSION) || __cplusplus >= 201103L
#include <memory>
#else
#include <tr1/memory>
#endif

namespace lutok {


class debug;
class state;


/// The type of a C++ function that can be bound into Lua.
///
/// Functions of this type are free to raise exceptions.  These will not
/// propagate into the Lua C API.  However, any such exceptions will be reported
/// as a Lua error and their type will be lost.
typedef int (*cxx_function)(state&);


/// Stack index constant pointing to the registry table.
extern const int registry_index;


/// A RAII model for the Lua state.
///
/// This class holds the state of the Lua interpreter during its existence and
/// provides wrappers around several Lua library functions that operate on such
/// state.
///
/// These wrapper functions differ from the C versions in that they use the
/// implicit state hold by the class, they use C++ types where appropriate and
/// they use exceptions to report errors.
///
/// The wrappers intend to be as lightweight as possible but, in some
/// situations, they are pretty complex because they need to do extra work to
/// capture the errors reported by the Lua C API.  We prefer having fine-grained
/// error control rather than efficiency, so this is OK.
class state {
    struct impl;

    /// Pointer to the shared internal implementation.
#if defined(_LIBCPP_VERSION) || __cplusplus >= 201103L
    std::shared_ptr< impl > _pimpl;
#else
    std::tr1::shared_ptr< impl > _pimpl;
#endif

    void* new_userdata_voidp(const size_t) const;
    void* to_userdata_voidp(const int) const;

    friend class state_c_gate;
    explicit state(void*);
    void* raw_state() const;

public:
    state();
    ~state();

    void close() const;
    void get_global(const std::string&);
    void get_global_table() const;
    bool get_metafield(const int, const std::string&) const;
    bool get_metatable(const int) const;
    void get_table(const int);
    int get_top() const;
    void insert(const int) const;
    bool is_boolean(const int) const;
    bool is_function(const int) const;
    bool is_nil(const int) const;
    bool is_number(const int) const;
    bool is_string(const int) const;
    bool is_table(const int) const;
    bool is_userdata(const int) const;
    void load_file(const std::string&);
    void load_string(const std::string&);
    void new_table() const;
    template< typename Type > Type* new_userdata();
    bool next(const int);
    void open_all() const;
    void open_base();
    void open_string() const;
    void open_table() const;
    void pcall(const int, const int, const int);
    void pop(const int) const;
    void push_boolean(const bool) const;
    void push_cxx_closure(cxx_function, const int) const;
    void push_cxx_function(cxx_function) const;
    void push_integer(const int) const;
    void push_nil() const;
    void push_string(const std::string&) const;
    void push_value(const int) const;
    void raw_get(const int) const;
    void raw_set(const int) const;
    void set_global(const std::string&);
    void set_metatable(const int) const;
    void set_table(const int);
    bool to_boolean(const int) const;
    long to_integer(const int) const;
    template< typename Type > Type* to_userdata(const int);
    std::string to_string(const int) const;
    static int upvalue_index(const int);
};


}  // namespace lutok

#endif  // !defined(LUTOK_STATE_HPP)
