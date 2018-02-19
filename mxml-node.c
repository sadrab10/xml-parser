#include "mxml.h"
#include "config.h"

static void		mxml_free(mxml_node_t *node);
static mxml_node_t	*mxml_new(mxml_node_t *parent, mxml_type_t type);

/*
 * 'mxmlRemove()' - Remove a node from its parent.
 *
 * This function does not free memory used by the node - use @link mxmlDelete@
 * for that.  This function does nothing if the node has no parent.
 */


void
mxmlRemove(mxml_node_t *node)		/* I - Node to remove */
{
#ifdef DEBUG
  fprintf(stderr, "mxmlRemove(node=%p)\n", node);
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!node || !node->parent)
    return;

 /*
  * Remove from parent...
  */

#if DEBUG > 1
  fprintf(stderr, "    BEFORE: node->parent=%p\n", node->parent);
  if (node->parent)
  {
    fprintf(stderr, "    BEFORE: node->parent->child=%p\n", node->parent->child);
    fprintf(stderr, "    BEFORE: node->parent->last_child=%p\n", node->parent->last_child);
  }
  fprintf(stderr, "    BEFORE: node->child=%p\n", node->child);
  fprintf(stderr, "    BEFORE: node->last_child=%p\n", node->last_child);
  fprintf(stderr, "    BEFORE: node->prev=%p\n", node->prev);
  fprintf(stderr, "    BEFORE: node->next=%p\n", node->next);
#endif /* DEBUG > 1 */

  if (node->prev)
    node->prev->next = node->next;
  else
    node->parent->child = node->next;

  if (node->next)
    node->next->prev = node->prev;
  else
    node->parent->last_child = node->prev;

  node->parent = NULL;
  node->prev   = NULL;
  node->next   = NULL;

#if DEBUG > 1
  fprintf(stderr, "    AFTER: node->parent=%p\n", node->parent);
  if (node->parent)
  {
    fprintf(stderr, "    AFTER: node->parent->child=%p\n", node->parent->child);
    fprintf(stderr, "    AFTER: node->parent->last_child=%p\n", node->parent->last_child);
  }
  fprintf(stderr, "    AFTER: node->child=%p\n", node->child);
  fprintf(stderr, "    AFTER: node->last_child=%p\n", node->last_child);
  fprintf(stderr, "    AFTER: node->prev=%p\n", node->prev);
  fprintf(stderr, "    AFTER: node->next=%p\n", node->next);
#endif /* DEBUG > 1 */
}



/*
 * 'mxmlAdd()' - Add a node to a tree.
 *
 * Adds the specified node to the parent.  If the child argument is not
 * @code NULL@, puts the new node before or after the specified child depending
 * on the value of the where argument.  If the child argument is @code NULL@,
 * puts the new node at the beginning of the child list (@code MXML_ADD_BEFORE@)
 * or at the end of the child list (@code MXML_ADD_AFTER@).  The constant
 * @code MXML_ADD_TO_PARENT@ can be used to specify a @code NULL@ child pointer.
 */
void
mxmlAdd(mxml_node_t *parent,		/* I - Parent node */
        int         where,		/* I - Where to add, @code MXML_ADD_BEFORE@ or @code MXML_ADD_AFTER@ */
        mxml_node_t *child,		/* I - Child node for where or @code MXML_ADD_TO_PARENT@ */
	mxml_node_t *node)		/* I - Node to add */
{
#ifdef DEBUG
  fprintf(stderr, "mxmlAdd(parent=%p, where=%d, child=%p, node=%p)\n", parent,
          where, child, node);
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!parent || !node)
    return;

#if DEBUG > 1
  fprintf(stderr, "    BEFORE: node->parent=%p\n", node->parent);
  if (parent)
  {
    fprintf(stderr, "    BEFORE: parent->child=%p\n", parent->child);
    fprintf(stderr, "    BEFORE: parent->last_child=%p\n", parent->last_child);
    fprintf(stderr, "    BEFORE: parent->prev=%p\n", parent->prev);
    fprintf(stderr, "    BEFORE: parent->next=%p\n", parent->next);
  }
#endif /* DEBUG > 1 */

 /*
  * Remove the node from any existing parent...
  */

  if (node->parent)
    mxmlRemove(node);

 /*
  * Reset pointers...
  */

  node->parent = parent;

  switch (where)
  {
    case MXML_ADD_BEFORE :
        if (!child || child == parent->child || child->parent != parent)
	{
	 /*
	  * Insert as first node under parent...
	  */

	  node->next = parent->child;

	  if (parent->child)
	    parent->child->prev = node;
	  else
	    parent->last_child = node;

	  parent->child = node;
	}
	else
	{
	 /*
	  * Insert node before this child...
	  */

	  node->next = child;
	  node->prev = child->prev;

	  if (child->prev)
	    child->prev->next = node;
	  else
	    parent->child = node;

	  child->prev = node;
	}
        break;

    case MXML_ADD_AFTER :
        if (!child || child == parent->last_child || child->parent != parent)
	{
	 /*
	  * Insert as last node under parent...
	  */

	  node->parent = parent;
	  node->prev   = parent->last_child;

	  if (parent->last_child)
	    parent->last_child->next = node;
	  else
	    parent->child = node;

	  parent->last_child = node;
        }
	else
	{
	 /*
	  * Insert node after this child...
	  */

	  node->prev = child;
	  node->next = child->next;

	  if (child->next)
	    child->next->prev = node;
	  else
	    parent->last_child = node;

	  child->next = node;
	}
        break;
  }

#if DEBUG > 1
  fprintf(stderr, "    AFTER: node->parent=%p\n", node->parent);
  if (parent)
  {
    fprintf(stderr, "    AFTER: parent->child=%p\n", parent->child);
    fprintf(stderr, "    AFTER: parent->last_child=%p\n", parent->last_child);
    fprintf(stderr, "    AFTER: parent->prev=%p\n", parent->prev);
    fprintf(stderr, "    AFTER: parent->next=%p\n", parent->next);
  }
#endif /* DEBUG > 1 */
}


/*
 * 'mxmlNewElement()' - Create a new element node.
 *
 * The new element node is added to the end of the specified parent's child
 * list. The constant @code MXML_NO_PARENT@ can be used to specify that the new
 * element node has no parent.
 */

mxml_node_t *				/* O - New node */
mxmlNewElement(mxml_node_t *parent,	/* I - Parent node or @code MXML_NO_PARENT@ */
               const char  *name)	/* I - Name of element */
{
  mxml_node_t	*node;			/* New node */


#ifdef DEBUG
  fprintf(stderr, "mxmlNewElement(parent=%p, name=\"%s\")\n", parent,
          name ? name : "(null)");
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!name)
    return (NULL);

 /*
  * Create the node and set the element name...
  */

  if ((node = mxml_new(parent, MXML_ELEMENT)) != NULL)
    /*node->value.element.name = strdup(name);*/

  return (node);
}




/*
 * 'mxml_new()' - Create a new node.
 */

static mxml_node_t *			/* O - New node */
mxml_new(mxml_node_t *parent,		/* I - Parent node */
         mxml_type_t type)		/* I - Node type */
{
  mxml_node_t	*node;			/* New node */


#if DEBUG > 1
  fprintf(stderr, "mxml_new(parent=%p, type=%d)\n", parent, type);
#endif /* DEBUG > 1 */

 /*
  * Allocate memory for the node...
  */

  if ((node = calloc(1, sizeof(mxml_node_t))) == NULL)
  {
#if DEBUG > 1
    fputs("    returning NULL\n", stderr);
#endif /* DEBUG > 1 */

    return (NULL);
  }

#if DEBUG > 1
  fprintf(stderr, "    returning %p\n", node);
#endif /* DEBUG > 1 */

 /*
  * Set the node type...
  */

  node->type      = type;
  node->ref_count = 1;

 /*
  * Add to the parent if present...
  */

  if (parent)
    mxmlAdd(parent, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, node);

 /*
  * Return the new node...
  */

  return (node);
}
