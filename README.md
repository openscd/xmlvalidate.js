# xmlvalidate.js
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fopenscd%2Fxmlvalidate.js.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fopenscd%2Fxmlvalidate.js?ref=badge_shield)


Validates `XML` documents against a W3C XML Schema (`XSD`) from the comfort of your browser.

## Building

> On POSIX

```sh
git submodule update --init --recursive
./emmake.sh
```

> On other OSs

This is what `emmake.sh` does for you on POSIX:

1. *copy* `xmlvalidate.c` into the `./libxml2` directory
2. *change working directory* to `./libxml2`
3. *patch* `Makefile.am` to include `xmlvalidate.c` in its `libxml2_la_SOURCES`
4. `emconfigure ./autogen.sh --with-minimum --with-schemas --disable-shared`

   If your system does not provide a Bourne shell, consider using
   `autoreconf --install` in place of `./autogen.sh` and crossing your fingers.
5. `emmake make`
6. *compile* the new object file `xmlvalidate.o` to `xmlvalidate.js`:
   
   ```sh
   OBJECTS="SAX.o entities.o encoding.o error.o parserInternals.o  \
       parser.o tree.o hash.o list.o xmlIO.o xmlmemory.o uri.o  \
       valid.o xlink.o HTMLparser.o HTMLtree.o debugXML.o xpath.o  \
       xpointer.o xinclude.o nanohttp.o nanoftp.o \
       catalog.o globals.o threads.o c14n.o xmlstring.o buf.o \
       xmlregexp.o xmlschemas.o xmlschemastypes.o xmlunicode.o \
       xmlreader.o relaxng.o dict.o SAX2.o \
       xmlwriter.o legacy.o chvalid.o pattern.o xmlsave.o \
       xmlmodule.o schematron.o xzlib.o"

   emcc -Os xmlvalidate.o $OBJECTS -o xmlvalidate.js \
   -s ALLOW_MEMORY_GROWTH=1 \
   -s EXPORTED_FUNCTIONS='["_validate", "_init"]' \
   -s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
   -s 'ENVIRONMENT=worker' \
   --pre-js ../pre.js --post-js ../post.js
   ```
7. *move* the resulting `xmlvalidate.wasm` and `xlvalidate.js` to `../dist/`
8. *change working directory* to `..`

Replicate these steps manually in order to compile `xmlvalidate.js` on a
non-POSIX-compliant operating system.

## Usage

Instantiate a WebWorker for validating against your schemas like so:

```js
if (window.Worker) {
  const worker = new Worker('worker.js');
  worker.postMessage({ content: xsd_content, name: "filename.xsd" });
  // a filename ending in ".xsd" tells the worker to load the schema
  worker.onmessage = e => {
    if (e.data.file === "filename.xsd" && e.data.loaded) {
      // our schema has been parsed and loaded
      worker.postMessage({ content: xml_content, name: "filename.xml" });
      // a filename not ending in ".xsd" tells the worker to validate
    } else if (e.data.file === "filename.xml") { // respond to errors
      console.log(e.data);
    }
  };
}
```

## Dependency
[Emscripten](https://emscripten.org/)

## License
Apache 2.0

&copy; 2020 OMICRON electronics GmbH


[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fopenscd%2Fxmlvalidate.js.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fopenscd%2Fxmlvalidate.js?ref=badge_large)