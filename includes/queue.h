struct Node{
    int val;
    Node* next = nullptr;
    Node* prev = nullptr;
    Node(int val){
        this->val = val;
    }
};

class Queue{
public:
    Queue(int maxSize);
    ~Queue();
    void push(long long val);
    void pop();
    int front();
    int back();
    bool isEmpty();
    bool isFull();
    void clear();
private:
    Node* _head = nullptr;
    Node* _tail = nullptr;
    int _size = 0;
    int _maxSize;
};