#include <stdio.h>
#include "mxml.h"
#include "config.h"

int main() {
	mxml_node_t *tree;

	tree = mxmlNewElement(MXML_NO_PARENT, "element");

	printf("hello world!\n");
	return 0;
}
