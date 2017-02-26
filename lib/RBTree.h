#include <vector>

#ifndef DS_A_RBTREE_H
#define DS_A_RBTREE_H

#endif

using namespace std;

struct fatNode;

/**
 * Структура 'узел' дерева.
 *
 * @property [int] key Ключ узла.
 * @property [char] parent Указатель на родителя.
 * @property [node*] color Цвет узла: 'r' (red) или 'b' (black).
 * @property [node*] left Ссылка на левого ребенка узла.
 * @property [node*] right Ссылка на правого ребенка узла.
 */
struct node {
    int key;
    char color;
    node *parent;
    node *left;
    node *right;
    int version;
    fatNode *container;
};

/**
 * Структура 'толстый узел' дерева ('fat nodes').
 * Хранит в себе множество индексированных структур 'node' (индекс = версия узла).
 *
 * @property [vector<node*>] nodes Вектор с указателями на узлы дерева.
 */
struct fatNode {
    vector<node *> nodes;
};

/**
 * Класс 'красно-черное дерево'.
 *
 * @property [node*] root Указатель на корневой узел дерева.
 */
class RBtree {
private:
    node *root = NULL;
    fatNode* createFatNode(node *);
    void appendToFatNode(fatNode *, node *);
    node *branchChainSuccessors(node *);
public:
    node* getRoot();
    node* getRoot(int);
    void insert(int);
    void insertFix(node *);
    void leftRotate(node *);
    void rightRotate(node *);
    bool remove(int);
    void removeFix(node *);
    node* successor(node *);
    node* search(int, int);
    node* search(int);
    int getLastVersion();
};