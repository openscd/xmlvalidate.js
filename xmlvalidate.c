#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

static xmlSchemaPtr wxschemas = NULL;
static int options = XML_PARSE_COMPACT | XML_PARSE_BIG_LINES;

char *removeFinalNewline(char *str) {
  if (str == NULL) return str;
  int length = strlen(str);
  if (str[length-1] == '\n') str[length-1]  = '\0';
  return str;
}

void printError(void *userData, xmlErrorPtr error) {
  xmlNode *node = error->node;
  char *tag = "";
  if (node) tag = node->name;
  char *file = error->file;
  if (!file) file = "";
  char *part = error->str1;
  if (!part) part = "";
  int line = error->line;
  if (!line) line = xmlGetLineNo(node);
  int column = error->int2;
  if (!column) column = -1;

  fprintf(stdout,
      "{\"code\":%d,\"message\":\"%s\",\"level\":%d,\"node\":\"%s\","
      "\"file\":\"%s\",\"line\":%d,\"column\":%d,\"part\":\"%s\"}\n",
      error->code, removeFinalNewline(error->message), error->level, tag,
      file, line, column, part);
}

int init(const char *schemas, int size, const char *filename) {
  xmlSchemaParserCtxtPtr ctxt = xmlSchemaNewMemParserCtxt(schemas, size);
  xmlSchemaSetParserStructuredErrors(ctxt, printError, NULL);
  xmlSetStructuredErrorFunc(ctxt, printError);

  wxschemas = xmlSchemaParse(ctxt);

  if (wxschemas == NULL) fprintf(stderr, "%s failed to compile\n", filename);

  xmlSchemaFreeParserCtxt(ctxt);
  return (wxschemas == NULL) ? -1 : 0;
}

int validate(const xmlChar *document, const char *filename) {
  if (wxschemas == NULL) {
    fprintf(stderr, "No schema compiled\n");
    return -1;
  }
  xmlSchemaValidCtxtPtr vctxt = xmlSchemaNewValidCtxt(wxschemas);
  xmlSchemaSetValidStructuredErrors(vctxt, printError, NULL);
  xmlSetStructuredErrorFunc(vctxt, printError);
  xmlSchemaValidateSetFilename(vctxt, filename);

  xmlDocPtr doc = xmlReadDoc(document, filename, NULL, options);
  int ret = xmlSchemaValidateDoc(vctxt, doc);

  xmlSchemaFreeValidCtxt(vctxt);
  return ret;
}
