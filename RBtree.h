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
       Node* y = x->right; //определили, с кем меняем (у был ниже х - у "поднимается")
       x->right = y->left; // х отказывается от сына х и вместо усыновил внука, левого потомка y
       if(!y->left->is_null()) { //если внук "реальный", то он признаёт в х родителя
           y->left->parent = x;
       }
       y->parent = x->parent; //у уверен, что он сын отца х
                               //но один из них лишний для этого родителя
       if (x->parent->is_NIL()) {  //если родитель был нереальным, зн х был старшем в семье,
           root = y;        //тогда семья признаёт у старшим
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


    void RIGHT_ROTATE (Node * x) {
       Node* y = x->left; //у - снизу справа, определили будущего бунтовщика
       x->left = y->right;  // внезапно и безпричинно х отказывается от сына: теперь х считает своим левым потомком не у, а у.right
                                //выкинули y и всех его левых последователей из родословного древа
       if(!y->right->is_null()) {
           y->right->parent = x; // если правый потомок был правоспособным, он признаёт в х синьора, подчиняется ему, предавая отца (у)
       }
       y->parent = x->parent; // у больше не считает своего отца (х) синьором, теперь он считает себя вассалом синьора икса (своего деда)
       if (x->parent->is_NIL()) {   //если тот выжил из ума (формально его не существует и всем заправлял х
           root = y;        // то у удаётся перехватить бразды правления, теперь он глава семьи
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


    void RB_INSERT(Node * z) { //вставка z (по указателю !!!)
        Node y (NIL);     //двойник универсальный (далее шут)
        Node x (*root);   //двойник корня (далее экс - режисёр )
        while (! x.is_NIL()){ //до тех пор пока экс - режисёр не выживет из ума
            y = x; //шут копирует экс-режиссёра в его текущем состоянии
            if (z->key < x.key) { //если способности (ключ) вновь прибывшего актёра меньше способностей (ключа) экс - режисёр
                x = x.left; //отправляем экс - режисёра на понижение (теперь он пародирует своего левого потомка)
            } else {
                x = x.right; //иначе тоже на понижение, но пародировать правого потомка
            }
        }
        z->parent = y;  //новый актёр выбирает учителем шута, который был и остаётся не в себе, если экс - режисёр изначально был сумасшедшим,
                        //либо находится в состоянии, в котором был экс - режисёр перед тем, как свихнуться (все его подопечные были уже в психушке -  дети - NIL)
        if (y.is_NIL()) {//еслли экс - режисёр всё-таки изначально был чокнутым
            root = z; //то новичёк становится режиссёром (без сравнения способностей)
        } else {
            if (z->key < y.key) { //иначе, сравнив способности актёра со своими,
                y.left = z;       //шут по-справедливости определяет его в главный (правый) состав, или второй (левый)
            } else {
                y.right = z;
            }
        }
        z->left = NIL;          //актёр понимает, что на грани сумасшедшего
        z->right = NIL;         //все его потомки - не в себе
        z->color = false;       // актёру присваивается статус незарекомендовавшего себя (красного)
        RB_INSERT_FIXUP(z);     // в театре начинается "ревизия"
        return;
    }



    void RB_INSERT_FIXUP (Node * z) {
        Node* y;
        while (((z->parent) -> color) == false) { // пока не исправим нарушение свойства RBtree, из-за того, что отец красного z - сам красный
            if (z->parent == (((z->parent)->parent) ->left)) { // определили дядю z
                y = ((z->parent)->parent) -> right;
                if (y.color == false){                  // если он тоже красный, делаем их с отцом чёрными, а деда красным
                    z->parent->color = true;
                    y.color  = true;
                    z->parent->parent->color = false;
                    z = z->parent->parent;      // теперь проблемы с дедом - исправим в след итерации
                } else {    //если дядя чёрный
                    if (z == z->parent -> right) { //если z(Б) - правый потомок, он "свергает" своего отца (А) левым поворотом,
                       z = z->parent; //переименовался в родителя
                       LEFT_ROTATE(z); //теперь старый z родитель нового (теперь А под Б)
                    }
                    z -> p -> color = true; // если были в if: затем родитель z ( Б теперь чёрный) становится чёрным ()
                    z->parent->parent->color = false; // С, старого деда Б - его нынешенего отца сделали красным
                    RIGHT_ROTATE(z->parent->parent); // Б сверг С правым поворотом
                }
            } else {
                if (z->parent == (((z->parent)->parent) ->right)) { //нашли дядю
                    y = ((z->parent)->parent) -> left;
                    if (y.color == false){                  // случай, когда дядя z красный
                        z->parent->color = true;
                        y.color = true;
                        z->parent->parent->color = false;
                        z = z->parent->parent;
                    } else {            //если дядя чёрный
                        if (z == z->parent -> left) { //если z (В)  - левый потомок, он свергает своего отца (А) Правым поворотом
                           z = z->parent;
                           RIGHT_ROTATE(z);
                        }
                        z -> p->color = true;
                        z->parent->parent->color = false;
                        LEFT_ROTATE(z->parent->parent); //дед свергается правым потомком
                    }
                }
            }
         }
        return;
    }
};


#endif // RBTREE_H
