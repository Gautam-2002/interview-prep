// Tree of Space – Locking and Unlocking N-Ary Tree

// You are given a complete, balanced N-Ary Tree and must support Q queries. There are 3 kinds of queries. Return true or false depending on whether the query was successful.

// Lock(v, id) - Lock vertex v for user - id
// Unlock(v, id) - If vertex v is locked by the same id, unlock it.
// Upgrade(v, id) - If v is unlocked and has at least one locked vertex in it's subtree and every locked vertex in the subtree of v is locked by id, unlock them and lock v instead.
// Further, here are some additional constraints

// A vertex cannot be locked if it has any locked ancestors or descendants, by any ID.
// When a vertex is upgraded, it's locked descendants are automatically unlocked.
// An upgrade operation is not possible if the vertex is already locked or has any locked ancestors
// An unlock operation is only possible if the vertex is already locked and locked by the same id
// P.S. - The question was very badly stated and not a single constraint was mentioned openly on the problem statement. A lot of these constraints had to be deduced by supplying custom input and observing the expected output. There were no constraints on the size of the variables.

#include <bits/stdc++.h>
using namespace std;

// TreeNode structure
class nary_Node
{
public:
    nary_Node *parent;
    vector<nary_Node *> connections;
    string v;
    set<nary_Node *> locked_children;
    // int acendent;
    int decendent, f;
    bool already_lock;
    nary_Node(string str, nary_Node *nary_Node)
    {
        v = str;
        parent = nary_Node;
        // acendent = 0;
        decendent = 0;
        f = 0;
        already_lock = false;
    }
    void MakeConnection(vector<string> l, nary_Node *a)
    {
        for (auto &x : l)
        {
            connections.push_back(new nary_Node(x, a));
        }
    }
};
//
nary_Node *CreateTree(nary_Node *treenode, int &m, vector<string> &s)
{
    queue<nary_Node *> vec;
    vec.push(treenode);
    int st = 1;
    while (!vec.empty())
    {
        nary_Node *r = vec.front();
        vec.pop();
        if (st >= s.size())
            continue;
        vector<string> temp;
        for (int i = st; i < st + m; i++)
        {
            temp.push_back(s[i]);
        }
        r->MakeConnection(temp, r);
        st += m;
        for (auto k : r->connections)
        {
            vec.push(k);
        }
    }
    return treenode;
};
class Tree
{
private:
    nary_Node *root;
    unordered_map<string, nary_Node *> hashing_map;

public:
    Tree(nary_Node *r) { root = r; }
    nary_Node *getRoot() { return root; }

    void Map_fill_Fun(nary_Node *r)
    {
        if (!r)
            return;
        hashing_map[r->v] = r;
        for (auto k : r->connections)
        {
            Map_fill_Fun(k);
        }
    }

    bool unlock(string v, int id)
    {

        nary_Node *t = hashing_map[v];
        // current node shoudn be locked
        if (!t->already_lock)
            return false;

        // locked by someone else
        if (t->f != id)
            return false;

        // parents ko btana hoga m unlock hua hu
        nary_Node *curr = t->parent;
        while (curr)
        {
            curr->decendent--;
            curr->locked_children.erase(t);
            curr = curr->parent;
        }

        t->already_lock = false;
        // t->f=0;
        return true;
    }

    bool upgrade(string v, int id)
    {
        nary_Node *t = hashing_map[v];
        // current node shoudn't be locked
        if (t->already_lock)
            return false;
        // no decendents should be locked
        if (t->decendent == 0)
            return false;

        // going on parents to check them
        nary_Node *temp = t->parent;

        while (temp)
        {
            if (temp->already_lock)
            {
                return false;
            }
            temp = temp->parent;
        }

        // now checking locked childrens

        for (auto i : t->locked_children)
        {
            if (i->f != id)
            {
                return false;
            }
        }

        set<nary_Node *> temp_children = t->locked_children;
        for (auto i : temp_children)
        {
            unlock(i->v, id);
        }

        lock(v, id);
        return true;
    }
    bool lock(string v, int id)
    {
        nary_Node *t = hashing_map[v];
        if (t->already_lock)
            return false;
        if (t->decendent != 0)
            return false;

        nary_Node *curr = t->parent;

        while (curr)
        {
            if (curr->already_lock)
                return false;
            curr = curr->parent;
        }

        curr = t->parent;
        while (curr)
        {
            curr->decendent++;
            curr->locked_children.insert(t);
            curr = curr->parent;
        }

        t->already_lock = true;
        t->f = id;
        return true;
    }
};

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    vector<string> s(a);
    for (int i = 0; i < a; i++)
        cin >> s[i];
    nary_Node *r = new nary_Node(s[0], NULL);
    r = CreateTree(r, b, s);
    Tree t(r);
    t.Map_fill_Fun(t.getRoot());

    int op, f;
    string palce;
    int x = 0;
    while (x < c)
    {
        cin >> op >> palce >> f;
        if (op == 3)
        {
            if (t.upgrade(palce, f))
            {
                cout << "upgraded"
                     << " " << palce << endl;
            }
            else
            {
                cout << "false 3" << endl;
            }
        }
        else if (op == 2)
        {
            if (t.unlock(palce, f))
            {
                cout << "unlocked"
                     << " " << palce << endl;
            }
            else
            {
                cout << "false 2" << endl;
            }
        }
        else if (op == 1)
        {
            if (t.lock(palce, f))
            {
                cout << "lock"
                     << " " << palce << endl;
            }
            else
            {
                cout << "false 1" << endl;
            }
        }
        x++;
    }
    return 0;
}