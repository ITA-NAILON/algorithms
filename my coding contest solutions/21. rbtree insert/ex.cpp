#include<fstream>
#include<iostream>

using namespace std;

enum Rotations {
    RIGHT,
    LEFT
};

enum Colors {
    RED,
    BLACK
};

template<class H>
class Node {

    private:
        H           value;
        Node<H>*    parent;
        Node<H>*    right;
        Node<H>*    left;
        Colors      color;

    public:
        Node(H value, Node<H>* parent = NULL, Node<H>* right = NULL, Node<H>* left = NULL) {
            this->value     = value;
            this->parent    = parent;
            this->right     = right;
            this->left      = left;
            this->color     = Colors::RED;
        }

        H getKey() {return value;}
        void setKey(H value) {this->value = value;}

        Node<H>* getParent() {return parent;}
        void setParent(Node<H>* parent) {this->parent = parent;}

        Node<H>* getRight() {return right;}
        void setRight(Node<H>* right) {this->right = right;}

        Node<H>* getLeft() {return left;}
        void setLeft(Node<H>* left) {this->left = left;}

        Colors getColor() {return color;}
        void setColor(Colors color) {this->color = color;}

        string getColorString() {return color == Colors::BLACK ? "B" : "R";}
};

template<class H>
class BSTree {

    private:
        Node<H>* root;

        // Trova il minimo (si trova nel sottoalbero sinistro)
		Node<H>* minimum(Node<H>* node) {
            Node<H>* min = node;

            while (min->getLeft() != NULL)
                min = min->getLeft();
            
            return min;
		}

        // Trova il successore di qualsiasi nodo
        Node<H>* successor(Node<H>* node) {

            // Caso 1: Il nodo ha il sottoalbero destra 
            if (node->getRight() != NULL)
                return minimum(node->getRight());

            // Caso 2: Il nodo non ha il sottoalbero di destra (es: è una foglia)
            // In questo caso il successore va cercato nei vari parent
            Node<H>* parent = node->getParent();

            while (parent != NULL && node == parent->getRight()) {
                node = parent;
                parent = parent->getParent();
            }

            return parent;
        }

        // Sostituisce il nodo x con il nodo y aggiornando il puntatore ai figli del padre di x
        void replace(Node<H>* x, Node<H>* y) {
            Node<H>* parent = x->getParent();

            // N.B: La radice non ha mai il padre
            parent != NULL ? 
                (x == parent->getRight() ? parent->setRight(y) : parent->setLeft(y))
                : (void)(root = y);
        }

        void rotation(Rotations way, Node<H>* node) {
            Node<H>* child = way == RIGHT ? node->getLeft() : node->getRight();
            Node<H>* parent = node->getParent();
            
            // Figlio non esiste: impossibile ruotare
            if (child == NULL) 
                return;
            
            // Cambio il figlio del nodo da ruotare con il figlio del figlio
            // e assegno al figlio il suo padre
            way == RIGHT ? (node->setLeft(child->getRight()), child->setRight(node))
               : (node->setRight(child->getLeft()), child->setLeft(node));

            // Sistemiamo i padri
            replace(node, child);
            child->setParent(parent);
            node->setParent(child);
            
            way == RIGHT ? (node->getLeft() != NULL ? node->getLeft()->setParent(node) : (void)NULL) 
                : (node->getRight() != NULL ? node->getRight()->setParent(node) : (void)NULL);
        }

    public:

        BSTree() {
            root = NULL;
        }

        Node<H>* getRoot() {
            return root;
        }
        
        Node<H>* insert(H key) {
            Node<H>* node = new Node<H>(key);

            // Caso 1: la radice non esiste
            if (root == NULL) {
                root = node;
                return node;
            }

            // Caso 2: Scansione per trovare la nuova posizione del nodo
            Node<H>* next = root;
            Node<H>* prev = NULL;

            while (next != NULL) {
                prev = next;
                next = key > next->getKey() ? next->getRight() : next->getLeft();
            }

            // Inserimento del nodo
            node->setParent(prev);
            key > prev->getKey() ? prev->setRight(node) : prev->setLeft(node);
            return node;
        }

        Node<H>* search(H key) {
            Node<H>* node = root;

            while (node != NULL && key != node->getKey()) 
                node = key > node->getKey() ? node->getRight() : node->getLeft();
            
            return node;
        }

        BSTree* remove(H key) {
            Node<H>* node   = search(key);
            Node<H>* parent = node->getParent();

            // Caso 1: Non ci sono figli
            if (node->getLeft() == NULL && node->getRight() == NULL) {
                replace(node, NULL);
                delete node;
            }

            // Caso 2: Ha un figlio
            else if (node->getRight() != NULL && node->getLeft() == NULL || node->getRight() == NULL && node->getLeft() != NULL) {
                Node<H>* child = node->getRight() != NULL ? node->getRight() : node->getLeft();
                replace(node, child);
                child->setParent(parent);
            }

            // Caso 3: Entrambi i figli
            else if (node->getRight() != NULL && node->getLeft() != NULL) {
                H key = successor(node)->getKey();
                remove(key);
                node->setKey(key);
            }

            return this;
        }

        void rotate(Rotations way, H key) {
            Node<H>* node = search(key);
            rotation(way, node);
        }
};

template<class H>
class RBTree {
    private:
        BSTree<H> btree;

        // Ricolora il nodo
        void recolor(Node<H>* node) {
            Colors color = node->getColor() == Colors::RED ? Colors::BLACK : Colors::RED;
            node->setColor(color);
        }

        void fixup(Node<H>* node) {
            Node<H>* parent = node->getParent();

            // Caso 1: se il nodo inserito è la radice si colora di nero
            if (parent == NULL) {
                node->setColor(Colors::BLACK);
                return;
            }

            // Caso base: se il padre è nero non si fa nulla e si inserisce il nodo rosso
            if (parent->getColor() == Colors::BLACK) 
                return;

            Node<H>* grandparent    = parent->getParent();
            Node<H>* uncle          = grandparent->getRight() == parent ? grandparent->getLeft() : grandparent->getRight();

            // Caso 2: zio rosso, si ricolorano padre, nonno e zio
            if (uncle != NULL && uncle->getColor() == Colors::RED) {
                recolor(grandparent);
                recolor(parent);
                recolor(uncle);
                fixup(grandparent);
                return;
            }
        
            // Direzione di futura rotazione per distingere il caso della linea e del triangolo
            Rotations grandpway = grandparent->getLeft() == parent ? Rotations::RIGHT : Rotations::LEFT;
            Rotations parentway = parent->getLeft() == node ? Rotations::RIGHT : Rotations::LEFT;
            
            // Caso 3 (triangolo): zio nero, si ruota il padre nella direzione opposta di node
            if (parentway != grandpway) {
                btree.rotate(parentway, parent->getKey());
                parent = node;
            }

            // Caso 4 (linea): si ruota il nonno di node nella direzione opposta di node e si ricolorano il padre e il nonno 
            btree.rotate(grandpway, grandparent->getKey());
            recolor(parent);
            recolor(grandparent);
        }

        void inorder(Node<H>* node, ofstream &out) {
            if (node != NULL) {
                inorder(node->getLeft(), out);
                out << "(" << node->getKey() << " " << node->getColorString() << ")" << " ";
                inorder(node->getRight(), out);
            }
        }

        void preorder(Node<H>* node, ofstream &out) {
            if (node != NULL) {
                out << "(" << node->getKey() << " " << node->getColorString() << ")" << " ";
                preorder(node->getLeft(), out);
                preorder(node->getRight(), out);
            }
        }

        void postorder(Node<H>* node, ofstream &out) {
            if (node != NULL) {
                postorder(node->getLeft(), out);
                postorder(node->getRight(), out);
                out << "(" << node->getKey() << " " << node->getColorString() << ")" << " ";
            }
        }
    
    public:
        RBTree() {
            btree = BSTree<H>();
        }

        void insert(H key) {
            Node<H>* node = btree.insert(key);
            fixup(node);
        }

        void orderby(ofstream &out, string type) {
            if (type == "inorder")
                inorder(btree.getRoot(), out);
            
            else if (type == "preorder")
                preorder(btree.getRoot(), out);
            
            else if (type == "postorder")
                postorder(btree.getRoot(), out);
            
            out << endl;
        }
};


template<class H>
void solve(ifstream &in, ofstream &out, string visit, int n) {
    RBTree<H> tree = RBTree<H>();

    for (int i=0; i < n; i++) {
        H value; 
        in >> value;
        tree.insert(value);
    }

    tree.orderby(out, visit);
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    for (int i=0; i<100; i++) {
        string type, visit;
        int    n;
        
        in >> type >> n >> visit;

        if (type == "int")
            solve<int>(in, out, visit, n);
        
        else if (type == "double")
            solve<double>(in, out, visit, n);
    }
}
