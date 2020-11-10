(function() {
  Module.ready = new Promise(function (resolve, reject) {
    addOnPreMain(() => {
      var init = Module.cwrap('init', 'number', ['string', 'number', 'string']);
      var validate = Module.cwrap('validate', 'number', ['number', 'string']);

      var jsInit = function(xsd, filename) {
        var code = init(xsd, xsd.length, filename);
        postMessage({ file: filename, loaded: code === 0 });
      }
      var jsValidate = function(xml, filename) {
        var length = lengthBytesUTF8(xml) + 1;
        var buf = Module._malloc(length);
        stringToUTF8(xml, buf, length);
        var code = validate(buf, filename);
        postMessage({ file: filename, valid: code === 0, code: code });
        Module._free(buf);
      }

      resolve({ init: jsInit, validate: jsValidate });
    });

    var origAbort = Module.abort;
    Module.abort = function(reason) {
      reject(Error(reason));
      origAbort.call(this, reason);
    }
  });
})();
