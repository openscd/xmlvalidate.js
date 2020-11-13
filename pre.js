Module['print'] = function(text) {
  try {
    postMessage(JSON.parse(text.replace('\\','\\\\')));
  } catch (e) {
    console.error(e);
    postMessage(text);
  }
};
Module['printErr'] = function(text) {
  postMessage(text);
};
