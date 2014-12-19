#!/bin/sh

BASE=$(dirname $0)
TARGET=$(dirname $1)

if [ $# -lt 2 ] ; then
    echo "Usage: ./res_gen.sh RES_FILE HEADER_FILE";
    exit 1
fi

RES_FILE=$1
if [ ! -f $RES_FILE ] ; then
    echo "File $RES_FILE doesn't exist!";
    exit 1
fi
RES_NAME=$(basename $RES_FILE)
RES_DIR=$(pwd -P)/$(dirname $RES_FILE)
HEADER_FILE=$2
HEADER_NAME=$(basename $HEADER_FILE)
HEADER_DIR=$(pwd -P)/$(dirname $HEADER_FILE)

echo -n "extern constexpr " > $HEADER_DIR/$HEADER_NAME
cd $RES_DIR
xxd -i $RES_NAME >> $HEADER_DIR/$HEADER_NAME
cd $BASE
sed -i -e 's/^unsigned int/const unsigned int/' $HEADER_DIR/$HEADER_NAME
