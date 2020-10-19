# xmlvalidate.js

Validates `XML` documents against a W3C XML Schema (`XSD`).

## Building

> On Linux

```sh
git submodule update --init --recursive
./emmake.sh
```

> On different OSes

We'll add a description how to bulid soon.
In the meantime, read `emmake.sh` and replicate what it does. Sorry.


## Usage

```js
createModule().then(mod => {
  init = mod.cwrap('init', null, ['string', 'number', 'string']);
  cvalidate = mod.cwrap('validate', 'number', ['string', 'string']);

  reset = () => {
    mod['warn'] = '';
    mod['error'] = '';
  }

  validate = (content, name) => {
    code = cvalidate(content, name);
    warnings = mod['warn'].split('\n').filter(s => s.length > 0);
    errors = mod['error'].split('\n').filter(s => s.length > 0);
    valid = code === 0;
    reset();
    return {valid, warnings, errors, code};
  }

  init(XSD_content, XSD_content.length, "XSD_name.xsd");

  result = validate(XML_content, "XML_name.xml")
})
```

I'll put all this housekeeping stuff in a `post.js` file so you don't have to.
Promise.

## Dependency
Emscripten

## License
Apache 2.0
