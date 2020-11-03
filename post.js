(function() {
  Module.ready = new Promise((resolve, reject) => {
    addOnPreMain(() => {
      const cinit = Module.cwrap('init', 'number', ['string', 'number', 'string']);
      const cvalidate = Module.cwrap('validate', 'number', ['number', 'string']);

      const init = function(xsd, filename) {
        const code = cinit(xsd, xsd.length, filename);
        postMessage({ file: filename, loaded: code === 0 });
      }
      const validate = function(xml, filename) {
        const length = lengthBytesUTF8(xml) + 1;
        const buf = Module._malloc(length);
        stringToUTF8(xml, buf, length);
        const code = cvalidate(buf, filename);
        postMessage({ file: filename, valid: code === 0, code: code });
        Module._free(buf);
      }

      resolve({ init, validate });
    });

    var origAbort = Module.abort;
    Module.abort = function(reason) {
      reject(Error(reason));
      origAbort.call(this, reason);
    }
  });
})();
