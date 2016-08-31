#!/bin/sh

if [ $# -lt 2 ] ; then
    echo "Usage: ./res_gen.sh RES_FILE OUT_DIR [-c]";
    echo "RES_FILE : Resource file to encode."
    echo "OUT_DIR  : Output directory, where encoded file will be saved."
    echo "-c       : true Compress with gzip resource file."
    exit 1
fi

XXD=xxd
command -v $XXD >/dev/null 2>&1 || { echo "$XXD command not found."; exit 1; }

BASE=$(dirname $0)
HEADER_EXT=h
SOURCE_EXT=cpp

RES_FILE=$1
if [ ! -f $RES_FILE ] ; then
    echo "File $RES_FILE doesn't exist!";
    exit 1
fi
RES_NAME=$(basename $RES_FILE)
RES_DIR=$(pwd -P)/$(dirname $RES_FILE)

RES_NAME_PROG=$(echo $RES_NAME | tr '.' '_')
STRUCT_NAME=res_$RES_NAME_PROG
STRUCT_NAME_TPD=res_${RES_NAME_PROG}_t

OUT_DIR=$2
if [ ! -d $OUT_DIR ]; then
    echo "Creating directory $OUT_DIR";
    mkdir -p $OUT_DIR
fi

if [ "$3" = "-c" ]; then
    echo "Compressing file with gzip.";
    gzip -c --best $RES_FILE > $RES_FILE.gz
    RES_FILE=$RES_FILE.gz
fi

HEADER_NAME=$RES_NAME_PROG.h
HEADER_FILE=$OUT_DIR/$HEADER_NAME

SOURCE_NAME=$RES_NAME_PROG.cpp
SOURCE_FILE=$OUT_DIR/$SOURCE_NAME

HEADER_GUARD_PREF="WEB_RES_GENERATED"
HEADER_GUARD_DEF=$HEADER_GUARD_PREF\_$(echo $HEADER_NAME | tr '.' '_' | tr [:lower:] [:upper:])
echo "#ifndef $HEADER_GUARD_DEF"                                                    >  $HEADER_FILE
echo "#define $HEADER_GUARD_DEF"                                                    >> $HEADER_FILE
echo ""                                                                             >> $HEADER_FILE
echo "#include <cstddef>"                                                           >> $HEADER_FILE
echo ""                                                                             >> $HEADER_FILE
echo "typedef struct $STRUCT_NAME"                                                  >> $HEADER_FILE
echo "{"                                                                            >> $HEADER_FILE
echo -n "    static constexpr "                                                     >> $HEADER_FILE
$XXD -i $RES_FILE | sed 's/[a-z_0-9]*_len/size/' | sed 's/[a-z_0-9]*\[\]/data\[\]/' >> $HEADER_FILE
echo "} ${STRUCT_NAME_TPD};"                                                        >> $HEADER_FILE
echo ""                                                                             >> $HEADER_FILE
echo "#endif // $HEADER_GUARD_DEF"                                                  >> $HEADER_FILE

echo " * $HEADER_FILE generated"

sed -i -e 's/  0x/        0x/'                            $HEADER_FILE
sed -i -e 's/};/    };/'                                  $HEADER_FILE
sed -i -e 's/unsigned int/\n    static constexpr size_t/' $HEADER_FILE

echo "#include \"$HEADER_NAME\""                           >  $SOURCE_FILE
echo ""                                                    >> $SOURCE_FILE
echo "constexpr unsigned char ${STRUCT_NAME_TPD}::data[];" >> $SOURCE_FILE

echo " * $SOURCE_FILE generated"

if [ "$3" = "-c" ]; then
    rm $RES_FILE
fi

# echo -n "extern constexpr " > $HEADER_DIR/$HEADER_NAME
# cd $RES_DIR
# xxd -i $RES_NAME >> $HEADER_DIR/$HEADER_NAME
# cd $BASE
# sed -i -e 's/^unsigned int/const unsigned int/' $HEADER_DIR/$HEADER_NAME


# xxd -i web_def_pages/400.html | grep len | sed 's/[a-z_0-9]*_len/size/'
