#!/bin/bash

CWD=$PWD

if [ -z "${PREFIX}" ]; then
  PREFIX=/usr/local
fi

mkdir -p $PREFIX

## make !
{
  sh autogen.sh
  ./configure --prefix=$PREFIX
  make
  make install
}
