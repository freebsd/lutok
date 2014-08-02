#! /bin/sh

set -e -x

sudo apt-get update -qq
sudo apt-get install -y liblua5.2-0 liblua5.2-dev pkg-config

wget --no-check-certificate \
    https://github.com/jmmv/atf/releases/download/atf-0.20/atf-0.20.tar.gz
tar -xzvf atf-0.20.tar.gz
( cd atf-0.20 && ./configure --prefix=/usr && make && sudo make install )

# TODO(jmmv): Should also install Kyua to be able to run the tests... but
# without a binary package available, the circular dependencies become hard
# to handle.
