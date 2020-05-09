#include <bits/stdc++.h>

using namespace std;

int main()
{
    int value;
    priority_queue<int,vector<int>,greater<int> >pq;
    pq.push(3);
    pq.push(1);
    pq.push(2);

    while(!pq.empty())
    {
        value = pq.top();
        pq.pop();
        cout<<value<< " ";
    }
    return 0;
}
