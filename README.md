# xmlvalidate.js

Validates `XML` documents against a W3C XML Schema (`XSD`).

## Building

> On POSIX

```sh
git submodule update --init --recursive
./emmake.sh
```

> On different OSes

We'll add a description how to bulid soon.
In the meantime, read `emmake.sh` and replicate what it does. Sorry.


## Usage

Instantiate a WebWorker for validating against your schemas like so:

```js
if (window.Worker) {
  const worker = new Worker('worker.js');
  worker.onmessage = e => {
    if (e.data === 'ready') { // libxml2 is loaded, we can load schemas now
      worker.postMessage({ content: xsd_content, name: "filename.xsd" });
      // a filename ending in ".xsd" tells the worker to load the schema
    } else if (e.data.file === "filename.xsd" && e.data.loaded) {
      worker.postMessage({ content: xml_content, name: "filename.xml" });
      // a filename not ending in ".xsd" tells the worker to validate
    } else if (e.data.file === "filename.xml") { // respond to errors
      console.log(e.data);
    }
  };
}
```

## Dependency
Emscripten

## License
Apache 2.0
