importScripts('xmlvalidate.js');

Module.ready.then(mod => {
  postMessage('ready');
  onmessage = e => {
    if (e.data.name.toLowerCase().endsWith('.xsd')) {
      mod.init(e.data.content, e.data.name);
    } else mod.validate(e.data.content, e.data.name)
  };
})
