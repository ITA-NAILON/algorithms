#include <iostream>
#include <fstream>
using namespace std;

template<class H>
class Heap {

    private:
        H* heap;
        int size;
        int heapsize;
        int heapifycalls;

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
            if (heapsize >= 1)
                heapifycalls++;

            int r   = right(i);
            int l   = left(i);
            int max = i;

            if (l <= heapsize && heap[l] > heap[max])
                max = l;
            
            if (r <= heapsize && heap[r] > heap[max])
                max = r;

            if (i != max) {
                swap(heap[max], heap[i]);
                heapify(max);
            }
        }

    public:
        Heap(int _size) {
            heapifycalls = 0;
            size         = _size;
            heapsize     = 0;
            heap         = new H[size];
        }

        ~Heap() {
            delete[] heap;
        }

        Heap* enqueue(H x) {
            if (heapsize == size)
                return NULL;
            
            // Inserimento
            heap[++heapsize] = x;

            // Controllo
            int i = heapsize;

            while (i > 1 && heap[i] > heap[parent(i)]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }

            return this;
        }

        Heap* extract() {
            heap[1] = heap[heapsize--];
            heapify(1);
        }

        void fprint(ofstream &out) {
            out << heapifycalls << " ";

            for (int i=1; i<=heapsize; i++)
                out << heap[i] << " ";

            out << endl;
        }
};

template<class H>
H stringto(string x) {
    if (typeid(H) == typeid(int) || typeid(H) == typeid(bool))
        return stoi(x);

    else if (typeid(H) == typeid(double))
        return stod(x);
        
    else if (typeid(H) == typeid(char))
        return (char)x[0];
}

template<class H>
void solve(ifstream &in, ofstream &out, int n) {
    Heap<H> heap = Heap<H>(n + 1);

    for (int i=0; i<n; i++) {
        string tmp; 
        in >> tmp;

        if (tmp == "extract")
            heap.extract();
        else
            heap.enqueue(stringto<H>(tmp.substr(2, tmp.length())));
    }

    heap.fprint(out);
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    for (int i=0; i<100; i++) {
        string type;
        int    n;

        in >> type >> n;

        if (type == "int")
            solve<int>(in, out, n);
        
        else if (type == "double")
            solve<double>(in, out, n);

        else if (type == "bool")
            solve<bool>(in, out, n);

        else if (type == "char")
            solve<char>(in, out, n);
    }
}
