#include <bits/stdc++.h>
using namespace std;

int order = 3;                         // order of the B plus tree
int data_len = order - 1;              // length of the data block
int min_data_len = data_len / 2;       // minimum data length of block
int internal_node_len = 2 * order - 1; // length of internal node

struct Leaf_Node // struct to define the leaves of the B plus tree
{
    vector<unsigned long long int> leaves;
    struct Node *next; // we need next pointer as they form a linked list
};

struct Internal_Node // struct to define the internal Nodes of B plus tree
{
    vector<unsigned long long int> data;
};

struct Node // struct that defines node of a B plus tree
{
    Leaf_Node *leaf_node;         // corressponding  leaf node
    Internal_Node *internal_node; // corressponding  internal node
    int row_len;                  // length of the block
    bool is_leaf;                 // to specify the node is a leaf or not
    Node *predecessor;            // parent of the node
};

void print(Node *head) //  utility function to print a tree
{
    queue<pair<int, Node *>> q;
    q.push({0, head});
    int prev_level = 0; // to keep track of the level wise printing
    while (q.size())
    {
        int cur_node_level = q.front().first; // current level
        Node *cur_node = q.front().second;    // current node traversing
        q.pop();

        if (cur_node_level != prev_level)
        {
            cout << "\n";
        }
        if (cur_node->is_leaf)
        {

            // to print leaves of the node
            cout << "||";
            auto &leaves = cur_node->leaf_node->leaves;
            string s = ""; // string to store output to be printed
            for (auto e : leaves)
            {
                s += "," + to_string(e); // concatenating the values
            }

            cout << s << " "; // printing the output string for the leaf level
        }
        else
        {                                               // to print internal nodes
            auto &data = cur_node->internal_node->data; // vector stores the internal node data
            string ss = "";                             // string to store output to be printed
            for (int i = 1; i < (int)data.size(); i += 2)
            {
                ss += "," + to_string(data[i]); // concatenating the values
            }
            for (int i = 0; i < (int)data.size(); i += 2)
            {
                q.push({cur_node_level + 1, (Node *)data[i]}); // pushing the data of the current node to be traversed in the next level interation
            }
            cout << ss << " "; // printing the output string for the leaf level
        }
        prev_level = cur_node_level; // updating the prev level
    }
    cout << "\n \n";
}

Node *find(Node *root, int value) // function  to find a value in a block
{
    if (root->is_leaf)
    {
        return root; // returns pointer when the value is found at leaf level
    }
    auto &data = root->internal_node->data; // vector stores the internal node data
    int size_of_data = data.size();         // size of the data
    int i;
    for (i = 1; i < size_of_data; i += 2)
    {
        if (data[i] >= value)
        {
            break; // break the loop when data value is greater than the req value
        }
    }
    return find((Node *)data[i - 1], value); // recursively calling
}

// function to insert in internal nodes
void insert_internal(Node *head, int key, Node *l, Node *r, Node *&root)
{
    if (head == NULL)
    {
        // initialzing  the head as it as null already
        head = new Node();
        head->internal_node = new Internal_Node();
        head->predecessor = 0;
        head->is_leaf = 0;
        head->internal_node->data.push_back((unsigned long long int)l);
        root = head;
    }
    auto &data = head->internal_node->data; // vector stores the internal node data
    l->predecessor = head;                  // corressponding parent of left pointer if head
    r->predecessor = head;                  // corressponding parent of right pointer if head

    int size_of_data = data.size(); // size of the data

    // shifting the data elements to the right
    data.resize(size_of_data + 2);
    for (int i = size_of_data + 1; i >= 2; --i)
    {
        data[i] = data[i - 2]; // right shifting
    }
    size_of_data = data.size(); // updating the size of the vector after right shifting

    int i; // Now  for inserting the value
    for (i = 3; i < size_of_data; i += 2)
    {
        if (data[i] < key)
        {
            swap(data[i], data[i - 2]);
            swap(data[i + 1], data[i - 1]);
        }
        else
            // finding correct position and breaking
            break;
    }
    i -= 2;
    // inserting
    data[i] = key;
    data[i + 1] = (unsigned long long int)r;

    if (size_of_data > internal_node_len)
    {
        int mid = size_of_data / 2;
        if (mid % 2 == 0)
        {
            --mid;
        }

        // spiliting the internal node
        Node *right_half = new Node();
        right_half->internal_node = new Internal_Node();
        right_half->is_leaf = 0;
        vector<unsigned long long int> rightnums(data.begin() + mid + 1,
                                                 data.end()); // taking corressponding right numbers
        vector<unsigned long long int> leftnums(data.begin(),
                                                data.begin() + mid); // taking corressponding left numbers
        right_half->internal_node->data = rightnums;                 // reassigning the values
        data = leftnums;                                             // reassigning the values
        int right_half_size = right_half->internal_node->data.size();
        for (int i = 0; i < right_half_size; i += 2)
        {
            ((Node *)(right_half->internal_node->data[i]))->predecessor = right_half; // assigning the right_half pointer
        }
        int midkey = data[mid];                                             // mid key stores the mid value key
        insert_internal(head->predecessor, midkey, head, right_half, root); // recursively the function calling for another half
    }
}

void insert(Node *head, int value, Node *&root) // utility function to insert
{
    head = find(head, value);               // searches and stores the pointer
    auto &leaves = head->leaf_node->leaves; // vector stoers leaves data
    leaves.push_back(value);                // inserting new data
    sort(leaves.begin(), leaves.end());     // sorting leaves
    int sz = leaves.size();                 // size of leaves vector

    if (sz > data_len) // overflow condition
    {
        int mid = (sz - 1) / 2; // taking mid index

        // for splitting the leaf node

        int half_size = (data_len + 2) / 2;

        vector<unsigned long long int> left_data(leaves.begin(),
                                                 leaves.begin() + mid + 1);                // corressponding left values
        vector<unsigned long long int> right_data(leaves.begin() + mid + 1, leaves.end()); // corressponding right values
        leaves = left_data;                                                                // reassigning the values

        Node *leaf = new Node();              // creating new node that is a leaf
        leaf->leaf_node = new Leaf_Node();    // creating corresponding leaf node
        leaf->is_leaf = 1;                    // boolean is true as it is a leaf
        leaf->leaf_node->leaves = right_data; // reassigning the right values

        Node *right_half = leaf;
        int midkey = leaves[mid]; // taking mid index key
        right_half->leaf_node->next = head->leaf_node->next;
        head->leaf_node->next = right_half;
        insert_internal(head->predecessor, midkey, head, right_half, root); // calling function to process the data in internal nodes
    }
}

int main()
{
    cout << "Enter Order: ";
    cin >> order; // taking order of the Bplus tree
    internal_node_len = 2 * order - 1;
    data_len = order - 1;
    min_data_len = data_len / 2;
    // Intiliasing the structure
    Node *root = new Node();
    root->row_len = data_len;
    root->is_leaf = 1;
    root->predecessor = NULL;
    root->leaf_node = new Leaf_Node();
    int choice, num;

    // menu
    while (1)
    {
        cout << string(50, '-') << "\n";
        cout << "Insert choice for operation: \n";
        cout << "1. Insertion\n";
        cout << "2. Print Tree\n";
        cout << "3. Exit\n";
        cout << string(50, '-') << "\n";
        cout << "Enter your choice: \n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter number to be inserted: \n";
            cin >> num;
            insert(root, num, root);
            print(root);
            break;

        case 2:
            print(root);
            break;
        case 3:
            exit(0);
        default:
            cout << "Invalid choice! \n";
        }
    }
    return 0;
}