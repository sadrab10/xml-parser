/*
 * 'mxmlLoadFile()' - Load a file into an XML node tree.
 *
 * The nodes in the specified file are added to the specified top node.
 * If no top node is provided, the XML file MUST be well-formed with a
 * single parent node like <?xml> for the entire file. The callback
 * function returns the value type that should be used for child nodes.
 * The constants @code MXML_INTEGER_CALLBACK@, @code MXML_OPAQUE_CALLBACK@,
 * @code MXML_REAL_CALLBACK@, and @code MXML_TEXT_CALLBACK@ are defined for
 * loading child (data) nodes of the specified type.
 */

mxml_node_t *				/* O - First node or @code NULL@ if the file could not be read. */
mxmlLoadFile(mxml_node_t    *top,	/* I - Top node */
             FILE           *fp,	/* I - File to read from */
             mxml_load_cb_t cb)		/* I - Callback function or constant */
{
 /*
  * Read the XML data...
  */

  return (mxml_load_data(top, fp, cb, mxml_file_getc, MXML_NO_CALLBACK, NULL));
}

