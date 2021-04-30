#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
#include <random>

struct Node{ // структура лист дерева
    int num; // ключ листа
    Node *left; // указатель на левого потомка
    Node *right; // указатель на правого потомка
    int left_depth; // глубина левой ветви
    int right_depth; // глубина правой ветви
public:
    Node(int n, Node* l, Node* r, int ld, int rd){ // конструктор листа
        num = n;
        left = l;
        right = r;
        left_depth = ld;
        right_depth = rd;
    }
    Node* right_rotation(){ // правый поворот
        Node *A = left;
        left = A->right;
        A->right = this;
        left_depth = A->right_depth;
        A->right_depth = 1 + max(left_depth, right_depth);
        return A;
    }
    Node* left_rotation(){ // левый поворот
        Node *A = right;
        right = A->left;
        A->left = this;
        right_depth = A->left_depth;
        A->left_depth = 1 + max(left_depth, right_depth);
        return A;
    }
    Node* triple_right_rotation(){ // последовательные правый и левый повороты
        right = right->right_rotation();
        return left_rotation();
    }
    Node* triple_left_rotation(){ // последовательные левый и правый повороты
        left = left->left_rotation();
        return right_rotation();
    }
    void print(){ // вывод на экран всех элементов поддерева, считая данный, в порядке возрастания
        if (left != NULL) left->print();
        cout << num << ' ';
        if (right != NULL) right->print();
    }
    ~Node(){}  // деструктор "листа"
};

struct Nodelist{ // структура список из листов
    Node *node; // указатель на лист для данного элемента списка
    Nodelist *next; // указатель на следующий элемент списка
    Nodelist *prev; // указатель на предыдущий элемент списка
    unsigned len; // длина списка
public:
    Nodelist(Node* nd,Nodelist* nxt, Nodelist* prv, unsigned ln){ // конструктор списка
        node = nd;
        next = nxt;
        prev = prv;
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
        auto new_nodelist = new Nodelist{nd, NULL, current, 0};
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
        cout << endl;
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
        head = new Nodelist{NULL, NULL, NULL, 0};
    }
    void insert(int key) { // добавляет новый лист с ключом key, если его нет в дереве; иначе не добавляет
        Node *new_node = new Node(key, NULL, NULL, 0, 0);
        if (root == NULL) {
            root = new_node;
            head->push(new_node);
            return;
        }
        Node *present = root;
        auto way_head = new Nodelist{root, NULL, NULL, 1};
        while (true) {
            if (new_node->num == present->num) {
                delete new_node;
                way_head->erase();
                return;
            } else if (new_node->num > present->num) {
                if (present->right == NULL) {
                    present->right = new_node;
                    head->push(new_node);
                    way_head->push(new_node);
                    break;
                } else {
                    present = present->right;
                    way_head->push(present);
                }
            } else if (new_node->num < present->num) {
                if (present->left == NULL) {
                    present->left = new_node;
                    head->push(new_node);
                    way_head->push(new_node);
                    break;
                } else {
                    present = present->left;
                    way_head->push(present);
                }
            }
        }
        auto current = way_head;
        while (current->next != NULL) {
            current = current->next;
        }
        auto pre_current = current;
        current = current->prev;
        while (current != NULL) {
            if (current->node->right == pre_current->node) {
                current->node->right_depth = 1 + max(pre_current->node->left_depth, pre_current->node->right_depth);
            } else {
                current->node->left_depth = 1 + max(pre_current->node->left_depth, pre_current->node->right_depth);
            }
            if (pre_current->node->right_depth - pre_current->node->left_depth >= 2) {
                if (pre_current->next->node->right_depth >= pre_current->next->node->left_depth) {
                    if (current->node->left == pre_current->node) {
                        current->node->left = pre_current->node->left_rotation();
                    } else {
                        current->node->right = pre_current->node->left_rotation();
                    }
                } else {
                    if (current->node->left == pre_current->node) {
                        current->node->left = pre_current->node->triple_right_rotation();
                    } else {
                        current->node->right = pre_current->node->triple_right_rotation();
                    }
                }
            } else if (pre_current->node->left_depth - pre_current->node->right_depth >= 2) {
                if (pre_current->next->node->left_depth >= pre_current->next->node->right_depth) {
                    if (current->node->left == pre_current->node) {
                        current->node->left = pre_current->node->right_rotation();
                    } else {
                        current->node->right = pre_current->node->right_rotation();
                    }
                } else {
                    if (current->node->left == pre_current->node) {
                        current->node->left = pre_current->node->triple_left_rotation();
                    } else {
                        current->node->right = pre_current->node->triple_left_rotation();
                    }
                }
            }
            pre_current = current;
            current = current->prev;
        }
        if (pre_current->node->right_depth - pre_current->node->left_depth >= 2) {
            if (pre_current->next->node->right_depth >= pre_current->next->node->left_depth) {
                root = pre_current->node->left_rotation();
            } else {
                root = pre_current->node->triple_right_rotation();
            }
        }
        if (pre_current->node->left_depth - pre_current->node->right_depth >= 2) {
            if (pre_current->next->node->left_depth >= pre_current->next->node->right_depth) {
                root = pre_current->node->right_rotation();
            } else {
                root = pre_current->node->triple_left_rotation();
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
    ~Tree(){ // деструктор дерева
        head->erase();
    }
};


int main() {
    //std::default_random_engine generator;
    for (int j = 0; j < 15; j++){
        for (int i = 8; i <= 11; i++){
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
            tree.insert(1);//здесь добавляется этот элемент, чтобы дерево не удалилось сверху, как только поиски закончились
            delete[] arr;
        }
        cout << endl;
    }
    return 0;
}