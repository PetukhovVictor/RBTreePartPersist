#ifndef DS_A_RBTREEINTERFACE_H
#define DS_A_RBTREEINTERFACE_H

#endif

/**
 * Класс консольный интерфейс красно-черного дерева.
 */
class RBtreeInterface {
private:
    void displayNode(node *);
public:
    void insert(RBtree *);
    void remove(RBtree *);
    void displayWalk(node *, int);
    void display(RBtree *, int);
    void display(RBtree *);
    void search(RBtree *);
};