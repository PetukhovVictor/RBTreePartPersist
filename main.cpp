#include <iostream>
#include "lib/RBTree.cpp"
#include "lib/RBTreeInterface.cpp"

using namespace std;

int main() {
    int commandNumber;
    bool exitFlag = false;
    RBtree * tree = new RBtree;
    RBtreeInterface treeInterface;
    do {
        cout << "----------------------------" << endl;
        cout << "|      RED-BLACK TREE      |" << endl;
        cout << "----------------------------" << endl;
        cout << "| 1 - Вставка узла         |" << endl;
        cout << "| 2 - Удаление узла        |" << endl;
        cout << "| 3 - Поиска узла по ключу |" << endl;
        cout << "| 4 - Вывод дерева         |" << endl;
        cout << "| 5 - Завершение программы |" << endl;
        cout << "----------------------------" << endl << endl;
        cin >> commandNumber;
        switch (commandNumber) {
            case 1:
                treeInterface.insert(tree);
                break;
            case 2:
                treeInterface.remove(tree);
                break;
            case 3:
                treeInterface.search(tree);
                break;
            case 4:
                treeInterface.display(tree);
                break;
            case 5:
                exitFlag = true;
                break;
            default:
                cout << endl << "> Некорректная команда. Попробуем снова?" << endl;
        }
        cout << endl;

    } while(!exitFlag);
    return 1;
}