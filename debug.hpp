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

/// \file debug.hpp
/// Provides the debug wrapper class for the Lua C debug state.

#if !defined(LUTOK_DEBUG_HPP)
#define LUTOK_DEBUG_HPP

#include <string>
#include <memory>

namespace lutok {


class state;


/// A model for the Lua debug state.
///
/// This extremely-simple class provides a mechanism to hide the internals of
/// the C native lua_Debug type, exposing its internal fields using friendlier
/// C++ types.
///
/// This class also acts as a complement to the state class by exposing any
/// state-related functions as methods of this function.  For example, while it
/// might seem that get_info() belongs in state, we expose it from here because
/// its result is really mutating a debug object, not the state object.
class debug {
    struct impl;

    /// Pointer to the shared internal implementation.
    std::shared_ptr< impl > _pimpl;

public:
    debug();
    ~debug();

    void get_info(state&, const std::string&) const;
    void get_stack(state&, int) const;

    int event() const;
    std::string name() const;
    std::string name_what() const;
    std::string what() const;
    std::string source() const;
    int current_line() const;
    int n_ups() const;
    int line_defined() const;
    int last_line_defined() const;
    std::string short_src() const;
};


}  // namespace lutok

#endif  // !defined(LUTOK_DEBUG_HPP)
