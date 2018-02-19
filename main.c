#include <stdio.h>
#include "mxml.h"
#include "config.h"

int main() {
	FILE *fp;
	mxml_node_t *tree;

	fp = fopen("test.xml", "r");
	
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);

	fclose(fp);

	//tree = mxmlNewElement(MXML_NO_PARENT, "element");

	printf("hello world!\n");
	return 0;
}
