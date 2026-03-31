#include "vector.hpp"
#include <iostream>
#include <string>

using namespace std;
using sjtu::vector;

int main() {
    // This is a placeholder main that will read commands from stdin
    // and perform corresponding vector operations

    int n;
    cin >> n;

    vector<int> v;

    for (int i = 0; i < n; ++i) {
        string cmd;
        cin >> cmd;

        if (cmd == "push_back") {
            int val;
            cin >> val;
            v.push_back(val);
        } else if (cmd == "pop_back") {
            v.pop_back();
        } else if (cmd == "size") {
            cout << v.size() << endl;
        } else if (cmd == "at") {
            int idx;
            cin >> idx;
            cout << v.at(idx) << endl;
        } else if (cmd == "clear") {
            v.clear();
        }
    }

    return 0;
}
