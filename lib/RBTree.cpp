#include "RBTree.h"

using namespace std;

/**
 * Создание 'толстого узла' по переданному узлу.
 *
 * @param [node*] element Узел, на основе которого будет создан 'толстый узел'.
 *
 * @return fatNode* Указатель на созданный 'толстый узел'.
 */
fatNode* RBtree::createFatNode(node *element) {
    fatNode *fatNodeElement = new fatNode;
    this->appendToFatNode(fatNodeElement, element);
    return fatNodeElement;
}

/**
 * Добавление переданного узла к переданному 'толстому узлу' (с двойной линковкой).
 *
 * @param [fatNode*] fatNodeElement 'Толстый узел'.
 * @param [node*] element Узел, добавляемый в 'толстый узел'.
 */
void RBtree::appendToFatNode(fatNode *fatNodeElement, node *element) {
    fatNodeElement->nodes.push_back(element);
    element->container = fatNodeElement;
    element->version = fatNodeElement->nodes.size();
}

/**
 * Отпочковывание всей цепочки наследников заданного узла с целью создания новой версии дерева.
 * Отпочковывание подразумевает клонирование ('node cloning') очередного узла и добавление клона в 'толстый узел' ('fat node').
 *
 * @param [node*] element Узел, наследников которого необходимо отпочковать (склонировать).
 *
 * @return node* Новый (отпочкованный) родитель переданного узла.
 */
node* RBtree::branchChainSuccessors(node *element) {
    node * parentNode = element->parent;
    if (parentNode == NULL) {
        return NULL;
    }
    node * newParentNode = new node;
    *(newParentNode) = *parentNode;
    this->appendToFatNode(parentNode->container, newParentNode);
    if (newParentNode->parent != NULL) {
        node *grandNode = this->branchChainSuccessors(newParentNode);
        newParentNode->parent = grandNode;
        if (grandNode != NULL && grandNode->left == parentNode) {
            grandNode->left = newParentNode;
        } else if (grandNode != NULL) {
            grandNode->right = newParentNode;
        }
    } else {
        this->root = newParentNode;
    }
    return newParentNode;
}

/**
 * Вставка узла с заданным ключом в дерево.
 *
 * @param [int] key Ключ вставляемого узла.
 */
void RBtree::insert(int key) {
    node *wayNode;
    node *parentNodeCandidate = NULL;
    node *element = new node;
    element->key = key;
    element->left = NULL;
    element->right = NULL;
    element->color = 'r';
    this->createFatNode(element);
    if (this->root == NULL) {
        this->root = element;
        element->parent = NULL;
    } else {
        wayNode = this->root;
        while (wayNode != NULL) {
            parentNodeCandidate = wayNode;
            if (wayNode->key < element->key) {
                wayNode = wayNode->right;
            } else {
                wayNode = wayNode->left;
            }
        }
        element->parent = parentNodeCandidate;
        element->parent = this->branchChainSuccessors(element);
        if (element->parent->key < element->key) {
            element->parent->right = element;
        } else {
            element->parent->left = element;
        }
    }
    this->insertFix(element);
}

/**
 * Исправление последствий вставки узла: повороты и смена окраски при необходимости.
 *
 * @param [node*] newNode Вставленный узел.
 */
void RBtree::insertFix(node *newNode) {
    node *uncleNode;
    if (this->root == newNode) {
        newNode->color = 'b';
        return;
    }
    while (newNode->parent != NULL && newNode->parent->color == 'r') {
        node *grandNode = newNode->parent->parent;
        if (grandNode->left == newNode->parent) {
            if (grandNode->right != NULL) {
                uncleNode = grandNode->right;
                if (uncleNode->color == 'r') {
                    newNode->parent->color = 'b';
                    uncleNode->color = 'b';
                    grandNode->color = 'r';
                    newNode = grandNode;
                }
            } else {
                if (newNode->parent->right == newNode) {
                    newNode = newNode->parent;
                    leftRotate(newNode);
                }
                newNode->parent->color = 'b';
                grandNode->color = 'r';
                rightRotate(grandNode);
            }
        } else {
            if (grandNode->left != NULL) {
                uncleNode = grandNode->left;
                if (uncleNode->color == 'r') {
                    newNode->parent->color = 'b';
                    uncleNode->color = 'b';
                    grandNode->color = 'r';
                    newNode = grandNode;
                }
            } else {
                if (newNode->parent->left == newNode) {
                    newNode = newNode->parent;
                    rightRotate(newNode);
                }
                newNode->parent->color = 'b';
                grandNode->color = 'r';
                leftRotate(grandNode);
            }
        }
        this->root->color = 'b';
    }
}

/**
 * Удаление узла из дерева.
 *
 * @param [int] key Ключ удаляемого узла.
 *
 * @return boolean Флаг, указывающий на успешность произведенного удаления.
 */
bool RBtree::remove(int key) {
    if (this->root == NULL) {
        return false;
    }
    node *wayNode;
    wayNode = this->root;
    node *y = NULL;
    node *q = NULL;
    bool found = false;
    while (wayNode != NULL && !found) {
        if (wayNode->key == key) {
            found = true;
        }
        if (!found) {
            if (wayNode->key < key) {
                wayNode = wayNode->right;
            } else {
                wayNode = wayNode->left;
            }
        }
    }
    if (!found) {
        return false;
    } else {
        if (wayNode->left == NULL || wayNode->right == NULL) {
            y = wayNode;
        } else {
            y = successor(wayNode);
        }
        if(y->left != NULL) {
            q = y->left;
        } else {
            if (y->right != NULL) {
                q = y->right;
            } else {
                q = NULL;
            }
        }
        if (q != NULL) {
            q->parent = y->parent;
        }
        if (y->parent == NULL) {
            this->root = q;
        } else {
            if (y == y->parent->left) {
                y->parent->left = q;
            } else {
                y->parent->right = q;
            }
        }
        if (y != wayNode) {
            wayNode->color = y->color;
            wayNode->key = y->key;
        }
        if (y->color == 'b') {
            this->removeFix(q);
        }
        y->container->nodes.erase(y->container->nodes.begin() + y->version - 1);
        if (y->container->nodes.size() == 0) {
            delete y->container;
        }
        return true;
    }
}

/**
 * Исправление последствий удаления узла: повороты и смена окраски при необходимости.
 *
 * @param [node*] p Вставленный узел.
 */
void RBtree::removeFix(node *p) {
    node *s;
    while (p != this->root && p->color == 'b') {
        if (p->parent->left == p) {
            s = p->parent->right;
            if (s->color == 'r') {
                s->color = 'b';
                p->parent->color = 'r';
                leftRotate(p->parent);
                s = p->parent->right;
            }
            if (s->right->color == 'b' && s->left->color == 'b') {
                s->color = 'r';
                p = p->parent;
            } else {
                if (s->right->color == 'b') {
                    s->left->color = 'b';
                    s->color = 'r';
                    rightRotate(s);
                    s = p->parent->right;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->right->color = 'b';
                leftRotate(p->parent);
                p = this->root;
            }
        } else {
            s = p->parent->left;
            if(s->color == 'r') {
                s->color = 'b';
                p->parent->color = 'r';
                rightRotate(p->parent);
                s = p->parent->left;
            }
            if (s->left->color == 'b' && s->right->color == 'b') {
                s->color = 'r';
                p = p->parent;
            } else {
                if (s->left->color=='b') {
                    s->right->color = 'b';
                    s->color = 'r';
                    leftRotate(s);
                    s = p->parent->left;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->left->color = 'b';
                rightRotate(p->parent);
                p = this->root;
            }
        }
        p->color = 'b';
        this->root->color = 'b';
    }
}

/**
 * Осуществление левого поворота.
 *
 * @param [node*] p Узел, вокруг которого производится поворот.
 */
void RBtree::leftRotate(node *p) {
    if (p->right == NULL) {
        return;
    } else {
        node *y = p->right;
        if (y->left != NULL) {
            p->right = y->left;
            y->left->parent = p;
        } else {
            p->right = NULL;
        }
        if (p->parent != NULL) {
            y->parent = p->parent;
        }
        if (p->parent == NULL) {
            this->root = y;
            this->root->parent = NULL;
        } else {
            if (p == p->parent->left) {
                p->parent->left = y;
            } else {
                p->parent->right = y;
            }
        }
        y->left=p;
        p->parent=y;
    }
}

/**
 * Осуществление правого поворота.
 *
 * @param [node*] p Узел, вокруг которого производится поворот.
 */
void RBtree::rightRotate(node *p) {
    if (p->left == NULL) {
        return;
    } else {
        node *y = p->left;
        if (y->right != NULL) {
            p->left = y->right;
            y->right->parent = p;
        } else {
            p->left = NULL;
        }
        if (p->parent != NULL) {
            y->parent = p->parent;
        }
        if (p->parent == NULL) {
            this->root = y;
            this->root->parent = NULL;
        } else {
            if (p == p->parent->left) {
                p->parent->left = y;
            } else {
                p->parent->right = y;
            }
        }
        y->right = p;
        p->parent = y;
    }
}

/**
 * Операция 'successor' для заданного узла.
 *
 * @param [node*] p Узел, для которого выполняется операция 'successor'.
 *
 * @return node* Преемник (successor) заданного узла.
 */
node* RBtree::successor(node *p) {
    node *y = NULL;
    if (p->left != NULL) {
        y = p->left;
        while (y->right != NULL) {
            y = y->right;
        }
    } else {
        y = p->right;
        while (y->left != NULL) {
            y = y->left;
        }
    }
    return y;
}

/**
 * Осуществление поиска узла по ключу в дереве заданной версии.
 *
 * @param [int] key Ключ, по которому необходимо осуществить поиск.
 * @param [int] version Версия, в которой производить поиск.
 *
 * @return node* Найденный узел (или NULL в случае, если узел не был найден).
 */
node* RBtree::search(int key, int version) {
    if (this->root == NULL) {
        return NULL;
    }
    if (version > this->root->container->nodes.size()) {
        return NULL;
    }
    node *p = this->root->container->nodes[version - 1];
    bool found = false;
    while (p != NULL && !found) {
        if (p->key == key)
            found = true;
        if (!found) {
            if (p->key < key) {
                p = p->right;
            } else {
                p = p->left;
            }
        }
    }
    return found ? p : NULL;
}

/**
 * Осуществление поиска узла по ключу в дереве последней версии.
 *
 * @param [int] key Ключ, по которому необходимо осуществить поиск.
 *
 * @return node* Найденный узел (или NULL в случае, если узел не был найден).
 */
node* RBtree::search(int key) {
    int lastVersion = this->getLastVersion();
    return this->search(key, lastVersion);
}

/**
 * Получение корня дерева последней версии.
 *
 * @return node* Указатель на узел, являющийся коренем дерева.
 */
node* RBtree::getRoot() {
    return this->root;
}

/**
 * Получение корня дерева заданной версии.
 *
 * @param [int] version Версия дерева, корень которого нужно получить.
 *
 * @return node* Указатель на узел, являющийся коренем дерева.
 */
node* RBtree::getRoot(int version) {
    if (this->root == NULL) {
        return NULL;
    }
    vector<node *> rootNodes = this->root->container->nodes;
    if (version > rootNodes.size()) {
        return NULL;
    }
    return rootNodes[version - 1];
}

/**
 * Получение последней версии дерева.
 *
 * @return int Номер версии.
 */
int RBtree::getLastVersion() {
    if (this->root == NULL) {
        return 0;
    }
    return this->root->container->nodes.size();
}