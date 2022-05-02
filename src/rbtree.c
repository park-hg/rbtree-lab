#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>


node_t *new_node_t(key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->color = RBTREE_RED;
  return z;
}


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}


// rotate x's subtree to the left.
// time complexity: O(1)
void left_rotate(rbtree *t, node_t *x) {
  // perform left-rotation only when x has a right child.
  if (x->right != NULL) 
  {
    node_t *y = x->right;
    
    x->right = y->left;
    if (y->left != NULL) {
      y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NULL) {
      t->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;

  }

}

// rotate x's subtree to the right.
// time complexity: O(1)
void right_rotate(rbtree *t, node_t *x) {
  // perfome right-rotation only when x has a left child.
  if (x->left != NULL)
  {
    node_t *y = x->left;

    x->left = y->right;
    if (y->right != NULL) {
      y->right->parent = x;
    }
    
    y->parent = x->parent;

    if (x->parent == NULL) {
      t->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    y->right = x;
    x->parent = y;

  }

}


// fix up rbtree when it has red-red violations.
// Case 2 
// time complexity: O(logn)
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent != NULL && z->parent->parent != NULL && z->parent->color == RBTREE_RED) {

    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      // Case 1. z's uncle is RBTREE_RED.
      if ((y != NULL) && (y->color == RBTREE_RED)) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } 
      // z's uncle is RBTREE_BLACK.
      else {
        // Case 2.
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        // Case 3.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } 
    
    else {
      // same as {then} clause of Case 1,2,3 with "right" and "left" exchanged.
      node_t *y = z->parent->parent->left;
      // Case 4. z's uncle is RBTREE_RED.
      if ((y != NULL) && (y->color == RBTREE_RED)) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } 
      // z's uncle is RBTREE_BLACK.
      else {
        // Case 5.
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // Case 6.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // same as Binary Search Tree insertion except rbtree_insert_fixup.
  node_t *x = t->root;
  node_t *y = NULL;
  node_t *z = new_node_t(key);
  
  while (x != NULL) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;

  if (y == NULL) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }
  
  z->left = NULL;
  z->right = NULL;
  z->color = RBTREE_RED;

  // fix up the tree when it has red-red violation.
  rbtree_insert_fixup(t, z);

  return z;
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // find a node that matches the key.
  node_t *curr = t->root;
  while (curr != NULL) {
    if (curr->key > key) {
      curr = curr->left;
    } else if (curr->key < key) {
      curr = curr->right;
    } else {
      return curr;
    }
  }
  // return NULL when the node couldn't be found.
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement min
  node_t *prev = NULL;
  node_t *curr = t->root;
  while (curr != NULL) {
    prev = curr;
    curr = curr->left;
  }
  return prev;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement max
  node_t *prev = NULL;
  node_t *curr = t->root;
  while (curr != NULL) {
    prev = curr;
    curr = curr->right;
  }
  return prev;
}


// fix up the tree when it has red-red violation.
// pass the parent of x as an instance to the function.
void rbtree_erase_fixup(rbtree *t, node_t *x, node_t *px) {
  while ((x != t->root) && (x == NULL || x->color == RBTREE_BLACK)) {
    if (x == px->left) {
      node_t *w = px->right;
      // Case 1. w is RBTREE_RED. w has two non-NULL children.
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        px->color = RBTREE_RED;
        left_rotate(t, px);
        w = px->right;
      }
      // Case 2. w is RBTREE_BLACK. children of w are also RBTREE_BLACK.
      if (((w->left == NULL) || (w->left->color == RBTREE_BLACK)) && ((w->right == NULL) || (w->right->color == RBTREE_BLACK))) {
        w->color = RBTREE_RED;
        x = px;
        px = px->parent;
      } 
      else {
        // Case 3. w is RBTREE_BLACK. left[w] is  RBTREE_RED.
        // Case 3 comes to Case 4.
        if (w->right == NULL || w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = px->right;
        }
        // Case 4. w is RBTREE_BLACK. right[w] is  RBTREE_RED.
        w->color = px->color;
        px->color = RBTREE_BLACK;
        if (w->right != NULL) {
          w->right->color = RBTREE_BLACK;
        }
        left_rotate(t, px);
        x = t->root;
      }
    }

    else {
      node_t *w = px->left;
      // Case 5. w is RBTREE_RED. w has two non-NULL children.
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        px->color = RBTREE_RED;
        right_rotate(t, px);
        w = px->left;
      }
      // Case 6. w is RBTREE_BLACK. children of w are also RBTREE_BLACK.
      if (((w->left == NULL) || (w->left->color == RBTREE_BLACK)) && ((w->right == NULL) || (w->right->color == RBTREE_BLACK))) {
        w->color = RBTREE_RED;
        x = px;
        px = px->parent;
      } 
      else {
        // Case 7. w is RBTREE_BLACK. left[w] is  RBTREE_RED.
        // Case 7 comes to Case 8.
        if (w->left == NULL || w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = px->left;
        }
        // Case 8. w is RBTREE_BLACK. right[w] is  RBTREE_RED.
        w->color = px->color;
        px->color = RBTREE_BLACK;
        if (w->left != NULL) {
          w->left->color = RBTREE_BLACK;
        }
        right_rotate(t, px);
        x = t->root;
      }
    }
  }
  if (x != NULL){
    x->color = RBTREE_BLACK;
  }
}


// find the maximum node which is smaller than x.
node_t *tree_predecessor(rbtree *t, node_t *x) {
  node_t *prev = NULL;
  node_t *curr = x->left;
  while (curr != NULL) {
    prev = curr;
    curr = curr->right;
  }
  return prev;
}


// same as Binary Search Tree deletion except rbtree_erase_fixup.
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = NULL;
  if ((z->left == NULL) || (z->right == NULL)) {
    y = z;
  } else {
    y = tree_predecessor(t, z);
  }

  node_t *x = NULL;
  if (y->left != NULL) {
    x = y->left;
  } else {
    x = y->right;
  }

  if (x != NULL) {
    x->parent = y->parent;
  }

  if (y->parent == NULL) {
    t->root = x;
  } else {
    if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }
  }

  if (y != z) {
    z->key = y->key;
  }

  // fix up the tree when has a red-red violation.
  if (y->color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x, y->parent);
  }
  free(y);
  return 0;
}


// inorder traversal to convert a BST to an array.
int inorder(node_t *node, key_t *arr, int i) {

  if (node != NULL){
    i = inorder(node->left, arr, i);
    arr[i] = node->key;
    i++;
    i = inorder(node->right, arr, i);
  }
  return i;
}


// call inorder.
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorder(t->root, arr, 0);
  return 0;
}
