#ifndef RBTREE_H
#define RBTREE_H

template <typename T >
class RBtree
{
    struct Node;
    Node NIL;
    Node * root;
    struct Node
    {
        bool colour; //true - черный, false - красный
        T key;
        Node * left;
        Node * right;
        Node * parent;
        Node(){;}
        Node (T _key): key(_key) {;}
        Node (Node * _root): colour(true),left(_root), right(_root) {;}     //для NIL
        // Node (Node const & _nd):left(_nd.left), right(_nd.right), key(_nd.key), parent(_nd.parent), color(_nd.color){;} // для копирования
        ~Node(){;}

        bool is_NIL(RBtree * tree){
            if ((this->left == tree->root) && (this->right == tree->root)) {
                return true;
            } else {
                return false;
            }
        }
    };

public:
    RBtree() {
        NIL.left = &NIL;
        NIL.right = &NIL;
        NIL.colour = true;
        root = &NIL;
    }
    ~RBtree() {;}   //!!!!!!!!!!!!!!!!!!!!!!придумать хорошее удаление Node-ов
private:
    void LEFT_ROTATE (Node *);
    void RIGHT_ROTATE (Node *);
    void RB_INSERT (Node *);
    void RB_INSERT_FIXUP (Node *);
    void * RB_DELETE (Node *);
    void RB_DELETE_FIXUP (Node *);
    void TreeWalk(Node*);
    void * TreeSuccessor(Node *);
    void * TreePredecessor(Node *);
    void * TreeMinimum (Node * x);
    void * TreeMaximum (Node *x);
    void * TreeSearch (Node * x , T key);
public:
    void insert(T key);
    void deleting (T key);
    bool has_key (T key);
    void print ();
};

template <typename T>
    void RBtree<T>::insert(T _key){
        Node * key = new Node(_key);
        this->RB_INSERT(key);
        return;
    }

template <typename T>
    void RBtree<T>::deleting (T _key){
        Node * key = (Node*) (this->TreeSearch(root, _key));
        if (key ->is_NIL(this)) return;
        this->RB_DELETE(key);
        delete (key);
        return;
    }

template <typename T>
    bool RBtree<T>::has_key (T _key){
        Node * key = (Node*)(this->TreeSearch(root, _key));
        return (!(key->is_NIL()));
    }

template <typename T>
    void RBtree<T>::print() {
        TreeWalk(root);
        return;
    }

template <typename T >
    void RBtree<T>::LEFT_ROTATE (RBtree<T>::Node * x) {
       Node* y = x->right; //определили, с кем меняем (у был ниже х - у "поднимается")
       x->right = y->left; // х отказывается от сына х и вместо усыновил внука, левого потомка y
       if(!y->left->is_NIL(this)) { //если внук "реальный", то он признаёт в х родителя
           y->left->parent = x;
       }
       y->parent = x->parent; //у уверен, что он сын отца х
                               //но один из них лишний для этого родителя
       if (x->parent->is_NIL(this)) {  //если родитель был нереальным, зн х был старшем в семье,
           root = y;        //тогда семья признаёт у старшим
           Node newNil(root);
           NIL = newNil;
       } else {
           if (x == x->parent->left) { // если х не был корнем, то у становится на место х (слева или справа от родителя)
               x->parent->left = y;
           } else {
               x->parent->right = y;
           }
       }
       y->left = x;     //левым потомком у стал х  // у считает х подчиненным (сыном), он теперь имеет ту же власть, что раньше имел над ним х
       x->parent = y; //х признаёт в у своего отца
       return;
    }

template <typename T >
    void RBtree<T>::RIGHT_ROTATE (RBtree<T>::Node * x) {
       Node* y = x->left; //у - снизу справа, определили будущего бунтовщика
       x->left = y->right;  // внезапно и безпричинно х отказывается от сына: теперь х считает своим левым потомком не у, а у.right
                                //выкинули y и всех его левых последователей из родословного древа
       if(!y->right->is_NIL(this)) {
           y->right->parent = x; // если правый потомок был правоспособным, он признаёт в х синьора, подчиняется ему, предавая отца (у)
       }
       y->parent = x->parent; // у больше не считает своего отца (х) синьором, теперь он считает себя вассалом синьора икса (своего деда)
       if (x->parent->is_NIL(this)) {   //если тот выжил из ума (формально его не существует и всем заправлял х
           root = y;        // то у удаётся перехватить бразды правления, теперь он глава семьи
           Node newNil(root);
           NIL = newNil;
       } else {
           if (x == x->parent->right) { // если дед все же в здравом уме и твёрдой памяти, он понимает, что х не на своём месте
               x->parent->right = y;            //и ставит у на место х
           } else {
               x->parent->left = y;
           }
       }
       y->right = x; //у считает х своим вассалом
       x->parent = y; //х подчиняется его воли
       return; //конец
    }

template <typename T >
    void RBtree<T>::RB_INSERT(RBtree<T>::Node * z) { //вставка z (по указателю !!!)
        Node* y = &NIL;//двойник универсальный (далее шут)
        Node* x  = root;   //двойник корня (далее экс - режисёр )
        while (! x->is_NIL(this)){ //до тех пор пока экс - режисёр не выживет из ума
            y = x; //шут копирует экс-режиссёра в его текущем состоянии
            if (z->key < x->key) { //если способности (ключ) вновь прибывшего актёра меньше способностей (ключа) экс - режисёр
                x = x->left; //отправляем экс - режисёра на понижение (теперь он пародирует своего левого потомка)
            } else {
                x = x->right; //иначе тоже на понижение, но пародировать правого потомка
            }
        }
        z->parent = y;  //новый актёр выбирает учителем шута, который был и остаётся не в себе, если экс - режисёр изначально был сумасшедшим,
                        //либо находится в состоянии, в котором был экс - режисёр перед тем, как свихнуться (все его подопечные были уже в психушке -  дети - NIL)
        if (y->is_NIL(this)) {//еслли экс - режисёр всё-таки изначально был чокнутым
            root = z; //то новичёк становится режиссёром (без сравнения способностей)
            Node newNil(root);
            NIL = newNil;
        } else {
            if (z->key < y->key) { //иначе, сравнив способности актёра со своими,
                y->left = z;       //шут по-справедливости определяет его в главный (правый) состав, или второй (левый)
            } else {
                y->right = z;
            }
        }
        z->left = &NIL;          //актёр понимает, что на грани сумасшедшего
        z->right = &NIL;         //все его потомки - не в себе
        z->colour = false;       // актёру присваивается статус незарекомендовавшего себя (красного)
        RB_INSERT_FIXUP(z);     // в театре начинается "ревизия"
        return;
    }


template <typename T >
    void RBtree<T>::RB_INSERT_FIXUP (RBtree<T>::Node * z) {
        Node* y;
        while (((z->parent) -> colour) == false) { // пока не исправим нарушение свойства RBtree, из-за того, что отец красного z - сам красный
            if (z->parent == (((z->parent)->parent) ->left)) { // определили дядю z
                y = ((z->parent)->parent) -> right;
                if (y->colour == false){                  // если он тоже красный, делаем их с отцом чёрными, а деда красным
                    z->parent->colour = true;
                    y->colour  = true;
                    z->parent->parent->colour = false;
                    z = z->parent->parent;      // теперь проблемы с дедом - исправим в след итерации
                } else {    //если дядя чёрный
                    if (z == z->parent -> right) { //если z(Б) - правый потомок, он "свергает" своего отца (А) левым поворотом,
                       z = z->parent; //переименовался в родителя
                       LEFT_ROTATE(z); //теперь старый z родитель нового (теперь А под Б)
                    }
                    z -> parent -> colour = true; // если были в if: затем родитель z ( Б теперь чёрный) становится чёрным ()
                    z->parent->parent->colour = false; // С, старого деда Б - его нынешенего отца сделали красным
                    RIGHT_ROTATE(z->parent->parent); // Б сверг С правым поворотом
                }
            } else {
                if (z->parent == (((z->parent)->parent) ->right)) { //нашли дядю
                    y = ((z->parent)->parent) -> left;
                    if (y->colour == false){                  // случай, когда дядя z красный
                        z->parent->colour = true;
                        y->colour = true;
                        z->parent->parent->colour = false;
                        z = z->parent->parent;
                    } else {            //если дядя чёрный
                        if (z == z->parent -> left) { //если z (В)  - левый потомок, он свергает своего отца (А) Правым поворотом
                           z = z->parent;
                           RIGHT_ROTATE(z);
                        }
                        z -> parent->colour = true;
                        z->parent->parent->colour = false;
                        LEFT_ROTATE(z->parent->parent); //дед свергается правым потомком
                    }
                }
            }
         }
        root->colour = true;
        return;
    }


template <typename T>
        void * RBtree<T>::RB_DELETE (RBtree<T>::Node * z) {
        Node * y;
        Node * x;
        if (z->left->is_NIL(this) || z->right->is_NIL(this)) {
            y = z;
        } else {
            y = (Node*)TreeSuccessor(z);
        }
        if (!(y->left->is_NIL(this))) {
            x = y->left;
        } else {
            x = y->right;
        }
        x->parent = y->parent;
        if (y->parent->is_NIL(this)) {
            root = x;
        } else {
            if (y == y ->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
        }
        if (y != z) {
            z->key = y->key;
        }
        if (y->colour == true) {
            RB_DELETE_FIXUP(x);
        }
        return (void*)y;
    }


template <typename T >
    void RBtree<T>::RB_DELETE_FIXUP (RBtree<T>::Node * x) {
        Node * w;
        while ((x != root) && (x->colour == true));
        if (x == x->parent->left) {
            w = x->parent ->right;
            if (w->colour == false) {
                w->colour = true;
                x->parent -> colour= false;
                LEFT_ROTATE(x->parent);
                w = x->parent->right;
            }
            if ((w->left->colour == true) && (w->right->colour == true)) {
                w->colour = false;
                x = x->parent;
            } else {
                if (w->right->colour == true) {
                    w->left->colour = true;
                    w->colour = false;
                    RIGHT_ROTATE(w);
                    w = x->parent->right;
                }
                w->colour = x->parent->colour;
                x->parent->colour = true;
                w->right->colour = true;
                LEFT_ROTATE(x->parent);
                x = root;
           }
        } else {
            w = x->parent ->left;
            if (w->colour == false) {
                w->colour = true;
                x->parent->colour = false;
                RIGHT_ROTATE(x->parent);
                w = x->parent->left;
            }
            if ((w->right->colour == true) && (w->left->colour == true)) {
                w->colour = false;
                x = x->parent;
            } else {
                if (w->left->colour == true) {
                    w->right->colour = true;
                    w->colour = false;
                    LEFT_ROTATE(w);
                    w = x->parent->left;
                }
                w->colour = x->parent->colour;
                x->parent->colour = true;
                w->left->colour = true;
                RIGHT_ROTATE(x->parent);
                x = root;
            }
        }
        x->colour = true;
    }


template <typename T >
    void* RBtree<T>::TreeSuccessor(Node * x) {
        if (!(x->right->is_NIL(this))) {
            return TreeMinimum(x->right);
        }
        Node * y = x->parent;
        while (!(y->is_NIL(this)) && x == y->right) {
            x = y;
            y = y->parent;
        }
        return (void*)y;
    }

template <typename T >
   void * RBtree<T>::TreePredecessor(RBtree<T>::Node * x) {
        if (!(x->left->is_NIL(this))) {
            return TreeMaximum(x->left);
        }
        Node * y = x->parent;
        while (!(y->is_NIL(this)) && x == y->left) {
            x = y;
            y = y->parent;
        }
        return (void*)y;
    }

template <typename T >
    void* RBtree<T>::TreeMinimum (RBtree<T>::Node * x) {
        while (!(x->left->is_NIL(this))) {
            x = x->left;
        }
        return (void *)x;
    }

template <typename T >
    void * RBtree<T>::TreeMaximum (RBtree<T>::Node *x) {
        while (!(x->right->is_NIL(this))) {
               x = x->right;
        }
        return (void *)x;
    }

template <typename T >
    void * RBtree<T>::TreeSearch (RBtree<T>::Node * x, T key){
        while  (!(x->is_NIL(this)) && key != (x->key)) {
            if (key < x->key) {
                   x = x->left;
            } else {
                x = x->right;
            }
        }
        return (void *)x;
    }


template <typename T >
     void RBtree<T>::TreeWalk(Node * x){
         if (!(x->is_NIL(this))) {
             TreeWalk(x->left);
             std::cout<<x->key<<' ';
             TreeWalk(x->right);
         }
     }






#endif // RBTREE_H
