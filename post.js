(function() {
  Module.ready = new Promise((resolve, reject) => {
    addOnPreMain(() => {
      const cinit = Module.cwrap('init', 'number', ['string', 'number', 'string']);
      const cvalidate = Module.cwrap('validate', 'number', ['string', 'string']);

      const init = function(xsd, filename) {
        const code = cinit(xsd, xsd.length, filename);
        postMessage({ file: filename, loaded: code === 0 });
      }
      const validate = function(xml, filename) {
        const code = cvalidate(xml, filename);
        postMessage({ file: filename, valid: code === 0, code: code });
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
