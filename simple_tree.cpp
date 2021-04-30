#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
using namespace std;


struct Node{ // структура лист дерева
    int num; // ключ листа
    Node *left; // указатель на левого потомка
    Node *right; // указатель на правого потомка
public:
    Node(int n, Node* l, Node* r){ // конструктор листа
        num = n;
        left = l;
        right = r;
    }
    void print(){ // вывод на экран всех элементов поддерева, считая данный, в порядке возрастания
        if (left != NULL) left->print();
        cout << num << ' ';
        if (right != NULL) right->print();
    }
    ~Node(){} // деструктор "листа"
};

struct Nodelist{ //структура список из листов
    Node *node; // указатель на лист для данного элемента списка
    Nodelist *next; // указатель на следующий элемент списка
    unsigned len; // указатель на предыдущий элемент списка
public:
    Nodelist(Node* nd,Nodelist* nxt,unsigned ln){ // конструктор списка
        node = nd;
        next = nxt;
        len = ln;
    }
    void push(Node* nd){ // добавление нового элемента с заданным листом в конец списка
        if (node == NULL) {
            node = nd;
            len = 1;
            return;
        }
        Nodelist *current = this;
        while (current->next != NULL){
            current = current->next;
        }
        auto new_nodelist = new Nodelist{nd, NULL, 0};
        current->next = new_nodelist;
        len += 1;
    }

    unsigned get_len(){ // возвращает длину списка
        return len;
    }
    void print(){ // выводит список на экран
        Nodelist *current = this;
        while (current != NULL){
            cout << current->node->num << ' ';
            current = current->next;
        }
    }
    void erase(){ // удаление списка
        Nodelist *current = this;
        while (current != NULL){
            Nodelist *tmp = current->next;
            delete current;
            current = tmp;
        }
    }
};

class Tree{ // класс дерева
private:
    Node* root; // указатель на корневой элемент
    Nodelist* head; // указатель на список из листов дерева
public:
    Tree(){ // конструктор дерева
        root = NULL;
        head = new Nodelist{NULL, NULL, 0};
    }
    void insert(int key){ // добавляет новый лист с ключом key, если его нет в дереве; иначе не добавляет
        Node *new_node = new Node(key, NULL, NULL);
        if (root == NULL){
            root = new_node;
            head->push(new_node);
            return;
        }
        Node* current = root;
        while(true){
            if (new_node->num == current->num){
                delete new_node;
                return;
            }
            else if (new_node->num > current->num){
                if (current->right == NULL){
                    current->right = new_node;
                    head->push(new_node);
                    return;
                }
                current = current->right;
            }
            else if (new_node->num < current->num){
                if (current->left == NULL){
                    current->left = new_node;
                    head->push(new_node);
                    return;
                }
                current = current->left;
            }
        }
    }
    bool find(int key){ // возвращает true, если есть лист с ключом key, возращает false иначе
        if (root == NULL){
            return false;
        }
        Node *current = root;
        while (true){
            if (current->num == key){
                return true;
            }
            else if (current->num > key){
                if (current->left == NULL){
                    return false;
                }
                current = current->left;
            }
            else if (current->num < key){
                if (current->right == NULL){
                    return false;
                }
                current = current->right;
            }
        }
    }
    unsigned size(){ // возвращает количество листов в дереве
        return head->get_len();
    }
    void list(){ // если дерево не пусто, выводит значения листов дерева в порядке возрастания
        if (root != NULL) root->print();
    }
    void delete_tree(){ // деструктор дерева
        head->erase();
    }
};

int main() {
    //std::default_random_engine generator;
    for (int j = 0; j < 15; j++){
        for (int i = 6; i <= 11; i++){
            int limiter = int(sqrt(pow(10, i)));
            std::uniform_int_distribution<int> distribution(1,limiter);
            auto tree = Tree();
            int *arr = new int[limiter];
            for (int counter = 0; counter < limiter; counter++) {
                //arr[counter] = 2*distribution(generator);
                arr[counter] = 2*counter;
                tree.insert(arr[counter]);
            }
            auto start = std::chrono::steady_clock::now();
            for (int counter = 0; counter < limiter; counter++) {
                tree.find(2*counter+1);
            }
            auto stop = std::chrono::steady_clock::now();
            auto duration = (stop - start);
            cout << limiter << ' ' << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << ' ';
            tree.delete_tree();
            delete[] arr;
        }
        cout << endl;
    }
    return 0;
}
