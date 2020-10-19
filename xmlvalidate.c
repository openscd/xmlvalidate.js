#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

static xmlSchemaPtr wxschemas = NULL;
static char *schemaname = "unknown.xsd";
static int options = XML_PARSE_COMPACT | XML_PARSE_BIG_LINES;

void printError(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
}

void printWarning(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stdout, msg, args);
  va_end(args);
}

void init(const char *schemas, int size, const char *filename) {
  xmlSchemaParserCtxtPtr ctxt = xmlSchemaNewMemParserCtxt(schemas, size);
  xmlSchemaSetParserErrors(ctxt, printError, printWarning, NULL);

  wxschemas = xmlSchemaParse(ctxt);

  if (wxschemas == NULL) printError(NULL, "%s failed to compile\n", filename);
  else schemaname = filename;

  xmlSchemaFreeParserCtxt(ctxt);
}

int validate(const xmlChar *document, const char *filename) {
  if (wxschemas == NULL) printWarning(NULL, "No schema compiled\n");
  xmlSchemaValidCtxtPtr vctxt = xmlSchemaNewValidCtxt(wxschemas);
  xmlSchemaSetValidErrors(vctxt, printError, printWarning, NULL);
  xmlSchemaValidateSetFilename(vctxt, schemaname);

  xmlDocPtr doc = xmlReadDoc(document, filename, NULL, options);
  int ret = xmlSchemaValidateDoc(vctxt, doc);

  if (ret < 0) fprintf(stderr, "%s validation: internal error\n", filename);

  xmlSchemaFreeValidCtxt(vctxt);
  return ret;
}
