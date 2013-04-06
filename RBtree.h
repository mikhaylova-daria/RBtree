#ifndef RBTREE_H
#define RBTREE_H

template <typename T >
class RBtree
{
    struct Node
    {
        bool color; //true - черный, false - красный
        T key;
        Node * left;
        Node * right;
        Node * parent;
        Node(){}
        Node (T _key): key(_key) {;}
        Node (Node * _root): left(_root), right(_root), color(true) {;}     //для NIL
        Node (Node const & _nd):left(_nd.left), right(_nd.right), key(_nd.key), parent(_nd.parent), color(true){;} // для копирования
        bool is_NIL() {
            if (this -> left == root);
        }
        bool is_NIL() {
            if (this -> left == root ) {
                return true;
            } else {
                return false;
            }
        }
    };
    Node NIL;
    Node * root;


public:
    RBtree() {;}
    ~RBtree() {;}
    void LEFT_ROTATE (Node *);
    void RIGHT_ROTATE (Node *);
    void RB_INSERT (Node *);
    void RB_INSERT_FIXUP (Node *);
    void RB_DELETE (Node *);
    void RB_DELETE_FIXUP (Node *);

    void LEFT_ROTATE (Node * x) {
       Node* y = x->right;
       x->right = y->left;
       if(!y->left->is_null()) {
           y->left->parent = x;
       }
       y->parent = x->parent;
       if (x->parent->is_NIL()) {
           root = y;
       } else {
           if (x == x->parent->left) {
               x->parent->left = y;
           } else {
               x->parent->right = y;
           }
       }
       y->left = x;
       x->parent = y;
       return;
    }


    void RIGHT_ROTATE (Node * x) {
       Node* y = x->left;
       x->left = y->right;
       if(!y->right->is_null()) {
           y->right->parent = x;
       }
       y->parent = x->parent;
       if (x->parent->is_NIL()) {
           root = y;
       } else {
           if (x == x->parent->right) {
               x->parent->right = y;
           } else {
               x->parent->left = y;
           }
       }
       y->right = x;
       x->parent = y;
       return;
    }


    void RB_INSERT(Node * z) {
        Node y (NIL);
        Node x (*root);
        while (! x.is_NIL()){
            y = x;
            if (z->key < x.key) {
                x = x.left;
            } else {
                x = x.right;
            }
        }
        z->parent = y;
        if (y.is_NIL()) {
            root = z;
        } else {
            if (z->key < y.key) {
                y.left = z;
            } else {
                y.right = z;
            }
        }
        z->left = NIL;
        z->right = NIL;
        z->color = false;
        RB_INSERT_FIXUP(z);
        return;
    }



    void RB_INSERT_FIXUP (Node * z) {
        Node y;
        while (((z->parent) -> color) == false) {
            if (z->parent == (((z->parent)->parent) ->left)) { //если родитель z является левым потомком
                y = *(((z->parent)->parent) -> right);
                if (y.color == false){                  // случай, когда дядя z красный
                    z->parent->color = true;
                    y.color  = true;
                    z->parent->parent->color = false;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent ->right) { //если z правый потомок
                       z = z->parent;
                       LEFT_ROTATE(z);
                    }
                    z -> p->color = true;
                    z->parent->parent->color = false;
                    RIGHT_ROTATE(z->parent->parent);
                }
            } else {
                if (z->parent == (((z->parent)->parent) ->right)) { //если родитель z является левым потомком
                    y = *(((z->parent)->parent) -> left);
                    if (y.color == false){                  // случай, когда дядя z красный
                        z->parent->color = true;
                        y.color  = true;
                        z->parent->parent->color = false;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent -> left) { //если z правый потомок
                           z = z->parent;
                           RIGHT_ROTATE(z); //??
                        }
                        z -> p->color = true;
                        z->parent->parent->color = false;
                        LEFT_ROTATE(z->parent->parent); //???
                    }
                }
            }
         }
        return;
    }
};


#endif // RBTREE_H
