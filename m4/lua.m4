dnl Copyright 2011 Google Inc.
dnl All rights reserved.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions are
dnl met:
dnl
dnl * Redistributions of source code must retain the above copyright
dnl   notice, this list of conditions and the following disclaimer.
dnl * Redistributions in binary form must reproduce the above copyright
dnl   notice, this list of conditions and the following disclaimer in the
dnl   documentation and/or other materials provided with the distribution.
dnl * Neither the name of Google Inc. nor the names of its contributors
dnl   may be used to endorse or promote products derived from this software
dnl   without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
dnl A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
dnl DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
dnl THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
dnl (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
dnl OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

dnl
dnl KYUA_LUA
dnl
dnl Helper macro to detect Lua in a variety of systems.
dnl
AC_DEFUN([KYUA_LUA], [
    lua_found=no
    for lua_release in ${LUA_VERSION:-5.4 5.3}; do
        PKG_CHECK_MODULES([LUA], [lua-${lua_release} >= ${lua_release}],
            [lua_found="lua-${lua_release}"],[
        PKG_CHECK_MODULES([LUA], [lua${lua_release} >= ${lua_release}],
            [lua_found="lua${lua_release}"],[
        PKG_CHECK_MODULES([LUA], [lua >= ${lua_release}],
            [lua_found="lua"],[])
        ])])
        if test "${lua_found}" != no; then
            break
        fi
    done

    AS_IF([test "${lua_found}" = no],[],[
        AC_SUBST([LUA_CFLAGS], [$(${PKG_CONFIG} --cflags ${lua_found})])
        AC_SUBST([LUA_LIBS], [$(${PKG_CONFIG} --libs ${lua_found})])
        ])

    AS_IF([test "${lua_found}" = no],
        [AC_MSG_ERROR([lua (5.3 or newer) is required])],
        [
AC_MSG_NOTICE([using LUA_CFLAGS = ${LUA_CFLAGS}])
AC_MSG_NOTICE([using LUA_LIBS = ${LUA_LIBS}])
    ])
])
