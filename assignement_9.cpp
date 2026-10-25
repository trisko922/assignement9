
#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
using namespace std;

// PART 1: MIN-HEAP
class MinHeap {
    vector<int> data;

    void heapifyUp(size_t i) {
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (data[parent] <= data[i]) break;
            swap(data[parent], data[i]);
            i = parent;
        }
    }

    void heapifyDown(size_t i) {
        while (true) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            size_t smallest = i;

            if (left < data.size() &&
                data[left] < data[smallest])
                smallest = left;

            if (right < data.size() &&
                data[right] < data[smallest])
                smallest = right;

            if (smallest == i) break;

            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    void insert(int value) {
        data.push_back(value);
        heapifyUp(data.size() - 1);
    }

    int peekMin() const {
        if (data.empty())
            throw runtime_error("Heap is empty");
        return data.front();
    }

    int removeMin() {
        if (data.empty())
            throw runtime_error("Heap is empty");

        int result = data.front();
        data.front() = data.back();
        data.pop_back();

        if (!data.empty())
            heapifyDown(0);

        return result;
    }

    void print() const {
        cout << "[ ";
        for (int value : data)
            cout << value << " ";
        cout << "]\n";
    }
};

// PART 2: TREAP
class Treap {
    struct Node {
        int key, priority;
        Node *left, *right;

        Node(int k, int p)
            : key(k), priority(p),
              left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    // Max-heap ordering by priority
    static Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    static Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    static Node* insertNode(
        Node* node, int key, int priority
    ) {
        if (!node)
            return new Node(key, priority);

        if (key < node->key) {
            node->left =
                insertNode(node->left, key, priority);

            if (node->left->priority > node->priority)
                node = rotateRight(node);
        }
        else if (key > node->key) {
            node->right =
                insertNode(node->right, key, priority);

            if (node->right->priority > node->priority)
                node = rotateLeft(node);
        }
        // Duplicate keys are ignored.

        return node;
    }

    static bool searchNode(const Node* node, int key) {
        if (!node) return false;
        if (key == node->key) return true;

        if (key < node->key)
            return searchNode(node->left, key);

        return searchNode(node->right, key);
    }

    static void printNode(const Node* node, int depth) {
        if (!node) return;

        printNode(node->right, depth + 1);

        for (int i = 0; i < depth; ++i)
            cout << "    ";

        cout << "(" << node->key
             << ", " << node->priority << ")\n";

        printNode(node->left, depth + 1);
    }

    static void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    Treap() = default;
    ~Treap() { clear(root); }

    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;

    void insert(int key, int priority) {
        root = insertNode(root, key, priority);
    }

    bool search(int key) const {
        return searchNode(root, key);
    }

    void print() const {
        printNode(root, 0);
    }
};

int main() {
    cout << "PART 1: MIN-HEAP\n";

    MinHeap heap;

    vector<int> values = {
        42, 17, 33, 5, 28,
        61, 9, 14, 50, 2
    };

    for (int value : values)
        heap.insert(value);

    cout << "After inserting 10 integers: ";
    heap.print();

    cout << "peekMin(): " << heap.peekMin() << '\n';

    cout << "removeMin(): " << heap.removeMin() << '\n';

    cout << "After removal: ";
    heap.print();

    heap.insert(1);

    cout << "After inserting 1: ";
    heap.print();

    cout << "removeMin(): " << heap.removeMin() << '\n';

    cout << "Final heap: ";
    heap.print();

    cout << "\nPART 2: TREAP\n";

    Treap treap;

    vector<pair<int, int>> pairs = {
        {50, 30}, {25, 40},
        {75, 20}, {10, 15},
        {35, 60}, {60, 50},
        {90, 10}, {5, 5},
        {30, 45}, {80, 35}
    };

    for (auto item : pairs)
        treap.insert(item.first, item.second);

    cout << "Final treap "
         << "(right subtree printed above each node):\n";

    treap.print();

    cout << "Search 35: "
         << (treap.search(35) ? "Found" : "Not found")
         << '\n';

    cout << "Search 99: "
         << (treap.search(99) ? "Found" : "Not found")
         << '\n';

    return 0;
}
