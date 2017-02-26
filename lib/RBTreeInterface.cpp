#include "RBTreeInterface.h"

using namespace std;

/**
 * Интерфейс вставки узла в дерево: запрашивает ключ у пользователя и добавляет узел с таким ключем.
 *
 * @param [RBtree*] tree Указатель на объект дерева.
 */
void RBtreeInterface::insert(RBtree * tree) {
    int key;
    cout << endl << "> Введите ключ вставляемого узла:" << endl;
    cin >> key;
    tree->insert(key);
    cout << endl << "> Узел успешно добавлен." << endl;
}

/**
 * Интерфейс удаления узла из дерева: запрашивает ключ у пользователя и удаляет узел с таким ключем.
 *
 * @param [RBtree*] tree Указатель на объект дерева.
 */
void RBtreeInterface::remove(RBtree * tree) {
    int key;
    cout << endl << "> Введите ключ узла, который нужно удалить:" << endl;
    cin >> key;
    bool result = tree->remove(key);
    string text = result ? "Узел успешно удален." : "Узел с таким ключом не найден.";
    cout << endl << "> " << text << endl;
}

/**
 * Интерфейс вывода дерева заданной версии на консоль.
 *
 * @param [RBtree*] tree Указатель на объект дерева.
 * @param [int] version Версия выводимого дерева.
 */
void RBtreeInterface::display(RBtree * tree, int version) {
    node* root = tree->getRoot(version);
    cout << endl;
    displayWalk(root, 1);
}

/**
 * Интерфейс вывода дерева последней версии на консоль.
 *
 * @param [RBtree*] Указатель на объект дерева.
 */
void RBtreeInterface::display(RBtree * tree) {
    int version;
    int lastVersion = tree->getLastVersion();
    if (lastVersion == 0) {
        cout << endl << "> Увы, дерево пустое, выводить нечего." << endl;
        return;
    }
    cout << endl << "> Введите желаемую версию дерева (последняя версия - " << lastVersion << "):" << endl;
    cin >> version;
    if (version < 1) {
        cout << endl << "> Версия указана некорректно. Последняя версия - " << lastVersion << endl;
        return;
    }
    if (version > lastVersion) {
        cout << endl << "> Увы, указанной версии дерева не существует. Последняя версия - " << lastVersion << endl;
        return;
    }
    this->display(tree, version);
}

/**
 * Интерфейс вывода узла дерева на консоль.
 *
 * @param [node*] p Указатель на узел дерева.
 */
void RBtreeInterface::displayNode(node * p) {
    string nodeColor = p->color == 'b' ? "black" : "red";
    cout << ">";
    cout << " Узел '" << p->key << "' (" << nodeColor <<")";
    if (p->parent != NULL) {
        cout << " [ ↑" << p->parent->key << " ]";
    }
    if (p->left != NULL) {
        cout << " [ ↙" << p->left->key << " ]";
    }
    if (p->right != NULL) {
        cout << " [ ↘" << p->right->key << " ]";
    }
}

/**
 * Рекурсивный обход дерева с целью дальнейшего вывода на консоль встреченных узлов.
 *
 * @param [node*] p Указатель на узел дерева.
 * @param [int] depth Глубина рекурсии.
 */
void RBtreeInterface::displayWalk(node * p, int depth) {
    if (p != NULL) {
        for (int i = 0; i < depth - 1; i++) {
            cout << ">";
        }
        this->displayNode(p);
        cout << endl;
        if (p->left != NULL) {
            this->displayWalk(p->left, depth + 1);
        }
        if (p->right != NULL) {
            this->displayWalk(p->right, depth + 1);
        }
    }
}

/**
 * Поиск узла в дереве по ключу.
 *
 * @param [RBtree*] tree Указатель на объект дерева.
 */
void RBtreeInterface::search(RBtree * tree) {
    int key, version;
    cout << endl << "> Введите ключ узла, который нужно найти:" << endl;
    cin >> key;
    int lastVersion = tree->getLastVersion();
    cout << endl << "> Введите желаемую версию дерева (последняя версия - " << lastVersion << "):" << endl;
    cin >> version;
    if (version < 1) {
        cout << endl << "> Версия указана некорректно. Последняя версия - " << lastVersion << endl;
        return;
    }
    if (version > lastVersion) {
        cout << endl << "> Увы, указанной версии дерева не существует. Последняя версия - " << lastVersion << endl;
        return;
    }
    node *p = tree->search(key, version);
    if (p == NULL) {
        cout << endl << "> Сожалеем, такой узел не был найден." << endl;
    } else {
        cout << endl;
        this->displayNode(p);
        cout << endl;
    }
}