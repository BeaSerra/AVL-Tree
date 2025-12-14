//Ana Beatriz da Silva Serra - 202304940034
//Gabriel Ribeiro de Alcantra - 202004940012

#include <iostream>
#include <algorithm>
#include <stack>
using namespace std;

class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
    };

    Node* root;

    Node* createNode(int key) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    int height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    void updateHeight(Node* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    int balanceFactor(Node* node) {
        return height(node->right) - height(node->left);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insert(Node* node, int key) {
        if (node == nullptr)
            return createNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        updateHeight(node);

        int balance = balanceFactor(node);

        // LL
        if (balance < -1 && key < node->left->key)
            return rotateRight(node);

        // RR
        if (balance > 1 && key > node->right->key)
            return rotateLeft(node);

        // LR
        if (balance < -1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL
        if (balance > 1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* search(Node* node, int key) {
        if (node == nullptr || node->key == key)
            return node;

        if (key < node->key)
            return search(node->left, key);

        return search(node->right, key);
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr)
            return node;

        if (key < node->key)
            node->left = remove(node->left, key);
        else if (key > node->key)
            node->right = remove(node->right, key);
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }

                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }

        if (node == nullptr)
            return node;

        updateHeight(node);

        int balance = balanceFactor(node);

        // LL
        if (balance < -1 && balanceFactor(node->left) <= 0)
            return rotateRight(node);

        // LR
        if (balance < -1 && balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RR
        if (balance > 1 && balanceFactor(node->right) >= 0)
            return rotateLeft(node);

        // RL
        if (balance > 1 && balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void printTree(Node* root) {
        if (root == nullptr) {
            cout << "Arvore vazia\n";
            return;
        }

        stack<Node*> globalStack;
        globalStack.push(root);

        int nEspacos = 32;
        bool linhaVazia = false;

        cout << endl;

        while (!linhaVazia) {
            stack<Node*> localStack;
            linhaVazia = true;

            for (int i = 0; i < nEspacos; i++)
                cout << ' ';

            while (!globalStack.empty()) {
                Node* temp = globalStack.top();
                globalStack.pop();

                if (temp != nullptr) {
                    cout << temp->key;
                    localStack.push(temp->left);
                    localStack.push(temp->right);

                    if (temp->left != nullptr || temp->right != nullptr)
                        linhaVazia = false;
                } else {
                    cout << "--";
                    localStack.push(nullptr);
                    localStack.push(nullptr);
                }

                for (int i = 0; i < nEspacos * 2 - 2; i++)
                    cout << ' ';
            }

            cout << endl;
            nEspacos /= 2;

            while (!localStack.empty()) {
                globalStack.push(localStack.top());
                localStack.pop();
            }
        }

        cout << endl;
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    bool search(int key) {
        return search(root, key) != nullptr;
    }

    void printVisual() {
        printTree(root);
    }
};

int main() {
    AVLTree tree;
    int n, value;

    cout << "Quantos valores deseja inserir? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Digite o valor " << i + 1 << ": ";
        cin >> value;
        tree.insert(value);
    }

    cout << "\nImpressao da arvore AVL:\n";
    tree.printVisual();

    return 0;
}
