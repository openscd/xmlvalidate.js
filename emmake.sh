#! /bin/sh

cp xmlvalidate.c libxml2

cd libxml2

patch -N < ../Makefile.am.patch
[ -f ./configure ] || ./autogen.sh
emconfigure ./configure --with-minimum --with-schemas --disable-shared
emmake make

OBJECTS="SAX.o entities.o encoding.o error.o parserInternals.o  \
		parser.o tree.o hash.o list.o xmlIO.o xmlmemory.o uri.o  \
		valid.o xlink.o HTMLparser.o HTMLtree.o debugXML.o xpath.o  \
		xpointer.o xinclude.o nanohttp.o nanoftp.o \
		catalog.o globals.o threads.o c14n.o xmlstring.o buf.o \
		xmlregexp.o xmlschemas.o xmlschemastypes.o xmlunicode.o \
		xmlreader.o relaxng.o dict.o SAX2.o \
		xmlwriter.o legacy.o chvalid.o pattern.o xmlsave.o \
		xmlmodule.o schematron.o xzlib.o"

emcc -Os xmlvalidate.o $OBJECTS -o xmlvalidate.js -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS='["_validate", "_init"]' -s EXPORTED_RUNTIME_METHODS='["cwrap"]' -s 'ENVIRONMENT=web,worker' --pre-js ../pre.js --post-js ../post.js

mv xmlvalidate.wasm xmlvalidate.js ../dist
cd ..
