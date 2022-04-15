#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Node {
    private:
        int     frequency;
        char    data; 
        Node*   left;
        Node*   right;

    public:
        Node(Node* left, Node* right, int frequency, char data) {
            this->left      = left;
            this->right     = right;
            this->frequency = frequency;
            this->data      = data;
        }

        Node* getRight() {
            return right;
        } 

        Node* getLeft() {
            return left;
        }

        int getFrequency() {
            return frequency;
        }

        char getData() {
            return data;
        }       
};

class MinHeap {

    private:
        Node** heap;
        int size;
        int heapsize;

        int parent(int i) {
            return i/2;
        }

        int right(int i) {
            return (i*2)+1;
        }

        int left(int i) {
            return i*2;
        }

        void heapify(int i) {
            int r   = right(i);
            int l   = left(i);
            int m   = i;

            if (l <= heapsize && heap[l]->getFrequency() < heap[m]->getFrequency())
                m = l;
            
            if (r <= heapsize && heap[r]->getFrequency() < heap[m]->getFrequency())
                m = r;

            if (i != m) {
                swap(heap[m], heap[i]);
                heapify(m);
            }
        }

    public:
        MinHeap(int size) {
            this->size         = size + 1;
            this->heapsize     = 0;
            this->heap         = new Node * [this->size]{NULL};
        }

        ~MinHeap() {
            delete[] heap;
        }

        MinHeap* enqueue(Node* x) {
            if (heapsize == size)
                return NULL;
            
            // Inserimento
            heap[++heapsize] = x;

            // Controllo
            int i = heapsize;

            while (i > 1 && heap[i]->getFrequency() < heap[parent(i)]->getFrequency()) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }

            return this;
        }

        Node* extract() {
            Node* node = heap[1];
            heap[1] = heap[heapsize--];
            heapify(1);
            return node;
        }

        void print() {
            for (int i=1; i <= heapsize; i++) {
                string data;
                data = heap[i]->getData() == (int)NULL ? "INTERNAL" : to_string(heap[i]->getData());
                cout << "(" << "D: " << data << " " << "F: " << heap[i]->getFrequency() << ") ";
            }

            cout << endl;
        }

        void buildHuffmanTree() {
            while (heapsize > 1) {
                Node* x = extract();
                Node* y = extract();
                Node* internal = new Node(x, y, x->getFrequency() + y->getFrequency(), (int)NULL);
                enqueue(internal);
            }
        }

        void printPostorderHuffmanCode(Node* node, string code) {
            string lcode = code;
            string rcode = code;

            if (node->getRight() != NULL) {
                rcode += "1";
                printPostorderHuffmanCode(node->getRight(), rcode);
            }

            if (node->getLeft() != NULL) {
                lcode += "0";
                printPostorderHuffmanCode(node->getLeft(), lcode);
            }

            if (node->getData() != (int)NULL)
                cout << "D: " << node->getData() << " " << "C: " << code << endl;
        }
    

        void postorderCode() {
            printPostorderHuffmanCode(heap[1], "");
        }
};


int main() {
    MinHeap h = MinHeap(10);
    h.enqueue(new Node(NULL, NULL, 5, 'a'));
    h.enqueue(new Node(NULL, NULL, 9, 'b'));
    h.enqueue(new Node(NULL, NULL, 12, 'c'));
    h.enqueue(new Node(NULL, NULL, 13, 'd'));
    h.enqueue(new Node(NULL, NULL, 16, 'e'));
    h.enqueue(new Node(NULL, NULL, 45, 'f'));

    h.print();
    h.buildHuffmanTree();
    h.print();
    h.postorderCode();

}
