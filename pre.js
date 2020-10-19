Module['warn'] = '';
Module['print'] = function(text) {
  Module['warn'] += text + '\n';
};
Module['error'] = '';
Module['printErr'] = function(text) {
  Module['error'] += text + '\n';
};
