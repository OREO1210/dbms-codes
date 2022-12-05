#include <bits/stdc++.h>
using namespace std;
int cap;
class bucket{
    public:
    int local_depth;
    set<int>b;
    int capacity;

    bucket(int ld, int sz){
        this->local_depth = ld;
        this->capacity = sz;
    }
    int insert(int num)
    {
        if ((this->b).find(num) == (this->b).end())
        {
            if ((this->capacity) == (this->b).size())
                return 2;

            (this->b).insert(num);
            return 0;
        }
        else
            return 1;
    }
   
    void erase(int x)
    {
        this->b.erase(find(this->b.begin(), this->b.end(), x));
    }
};
class directory{
    public:
    int global_depth;
    int bucket_capacity;
    vector<bucket*> buckets;

    directory(int depth,int bucket_cap){
        this->global_depth = depth;
        this->bucket_capacity = bucket_cap;
        for(int i=0;i<1 <<depth;i++){
           this->buckets.push_back(new bucket(depth,bucket_cap));
        }
    }   
    void split(int hash){
        int d= buckets[hash]->local_depth;
        if(d==global_depth){
            for (int i = 0; i < (1 << (this->global_depth)); i++)
            {
                (this->buckets).push_back(buckets[i]);
            }
            this->global_depth += 1;
        }
        int mod1=hash;
        int mod2= (1<<d)^hash;
        d++;
        bucket* b1= new bucket(d, this->bucket_capacity);
        bucket *b2 = new bucket(d, this->bucket_capacity);
        bucket *old = buckets[hash];

        for(int i=0;i< 1<<global_depth;i++){
            if(i%(1<<d)==mod1){ buckets[i]=b1;}
            else if(i%(1<<d) ==mod2){ buckets[i]=b2;}
        }
        for(auto e: old->b){
            this->insert(e,1);
        }
        delete old;
    }
    void insert(int x, bool flag)
    {
        int Hash = x & ((1 << global_depth) - 1);
        int status = this->buckets[Hash]->insert(x);
        if(status==1){cout<<"\nElement Already present!\n"; return;}
        if(status==2)
        {
            split(Hash);
            insert(x, false);
        }
    }
    void display()
    {
        cout << "Global depth: " << global_depth << endl;
        for (int i = 0; i < (1 << global_depth); i++)
        {
            cout << "Local depth: " << buckets[i]->local_depth << "\n";
            for (auto e : buckets[i]->b)
            {
                cout << e << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    int erase(int val)
    {
        int hash = val & ((1<<global_depth)-1);
        bucket *bk = buckets[hash]; 
        if(bk->b.find(val) == bk->b.end())
            {cout<<"\nElement not found!\n";
            return 0;}
        bk->b.erase(val);
        mergeif(hash);
        cout<<"\nElement Deleted!\n";
        return 1;
    }
    
    void mergeif(int hash)
    {
        bucket *bk = buckets[hash];
        int ld = bk->local_depth;
        if (ld == 1)
            return;
        int bucketNo2 =(( (1<<ld) -1)&(hash))^(1<<(ld-1));

        bucket *b2 = buckets[bucketNo2];
        if ((int)(bk->b).size() + (int)(b2->b).size() <= bucket_capacity && b2->local_depth == ld)
        {
            for (auto val : b2->b)
            {
                (bk->b).insert(val);
            }
            for (int i = 0; i < (1 << (global_depth - ld)); i++)
            {
                int j = ((i << ld) + bucketNo2);
                if (buckets[j] == b2)
                    buckets[j] = bk;
            }

            bk->local_depth--;
            delete b2;
            half_if();
            int xx= hash&((1<< (bk->local_depth)) -1);
            mergeif(xx);
        }
    }
    void half_if()
    {
        int count = 0;
        for (int i = 0; i < (1 << global_depth); i++)
        {
            if (buckets[i]->local_depth < global_depth)
            {
                count++;
            }
        }
        if (count == (1 << (global_depth)))
        {
            global_depth--;
            buckets.resize(1 << global_depth);
        }
    }
};

int main(){
    int gd;
    cout<<"Enter capacity of bucket:  \n";
    cin>>cap;
    cout<<"Enter Global Depth: \n";
    cin>>gd;
    directory d(gd,cap);
    int choice;
    int num;
    while(1){
        cout<<string(50,'-')<<"\n";
        cout<<"Insert choice for operation: \n";
        cout<<"1. Insertion \n";
        cout<<"2. Deletion \n";
        cout<<"3. Display \n";
        cout<<"4. Exit \n";
        cout << string(50, '-') << "\n";
        cout<<"Enter your choice: \n";
        cin>>choice;
        switch(choice){
            case 1:
                cout << "Enter number to be inserted :\n";
                cin>>num;
                d.insert(num,false);
                break;
            case 2:
                cout<<"Enter number to be deleted: \n";
                cin>>num;
                d.erase(num);
                break;
            case 3:
                d.display();
                break;
            case 4:
                exit(0);
            default:
                cout<<"Invalid choice! \n";
        }
    }
    return 0;
}