#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
#define CHUNK_SIZE 1024

struct Node {
    std::map<char, long long> to;
    long long link;
    long long len;
    long long pos;
    char sym;
    explicit Node(char symb)
    {
        link = -1;
        len = 0;
        pos = -1;
        sym = symb;
    }
};
void add(
        std::vector<Node>& suffix_automaton,
        long long& last,
        char c,
        long long alphabet_size)
{
    suffix_automaton.emplace_back(c);
    long long curr = int(suffix_automaton.size()) - 1;
    suffix_automaton[curr].len = suffix_automaton[last].len + 1;
    suffix_automaton[curr].pos = suffix_automaton[curr].len - 1;
    long long prev = last;
    while (prev != -1 &&
           suffix_automaton[prev].to.find(c) == suffix_automaton[prev].to.end()) {
        suffix_automaton[prev].to[c] = curr;
        prev = suffix_automaton[prev].link;
    }
    if (prev == -1) {
        suffix_automaton[curr].link = 0;
        last = curr;
        return;
    }
    long long node_to_clone = suffix_automaton[prev].to[c];
    if (suffix_automaton[node_to_clone].len == suffix_automaton[prev].len + 1) {
        suffix_automaton[curr].link = node_to_clone;
        last = curr;
        return;
    }
    suffix_automaton.emplace_back(' ');
    long long clone = int(suffix_automaton.size()) - 1;
    suffix_automaton[clone].len = suffix_automaton[prev].len + 1;
    while (prev != -1 &&
           suffix_automaton[prev].to.find(c) != suffix_automaton[prev].to.end() &&
           suffix_automaton[prev].to[c] == node_to_clone) {
        suffix_automaton[prev].to[c] = clone;
        prev = suffix_automaton[prev].link;
    }
    suffix_automaton[clone].to = suffix_automaton[node_to_clone].to;
    suffix_automaton[clone].link = suffix_automaton[node_to_clone].link;
    suffix_automaton[node_to_clone].link = clone;
    suffix_automaton[curr].link = clone;
    suffix_automaton[clone].pos = suffix_automaton[node_to_clone].pos;
    suffix_automaton[clone].sym = suffix_automaton[node_to_clone].sym;
    last = curr;
}
long long check(std::vector<Node>& suffix_automaton, std::string s)
{
    long long pos = 0;
    for (char& i : s) {
        if (suffix_automaton[pos].to.find(i) ==
            suffix_automaton[pos].to.end()) {
            return -1;
        } else
            pos = suffix_automaton[pos].to[i];
    }
    return suffix_automaton[pos].pos;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cout.tie(0);
    std::cin.tie(0);
    std::vector<Node> suffix_automaton;
    int alphabet_size = 26;
    suffix_automaton.emplace_back(' ');
    long long last = 0;
    std::string path;
    std::cin >> path;
    int N;
    std::cin >> N;
    std::ifstream file;
    file.open(path);
    std::string to_find;
    char buffer[CHUNK_SIZE];
    while (!file.eof()) {
        file.read(buffer, CHUNK_SIZE);
        for (long long i = 0; i < strlen(buffer); ++i) {
            add(suffix_automaton, last, buffer[i], alphabet_size);
        }
    }
    for (int i = 0; i < N; ++i) {
        std::cin >> to_find;
        long long ans = check(suffix_automaton, to_find);
        if (ans == -1) {
            std::cout << "Not in a file" << '\n';
        } else {
            std::cout << "The first occurrence of the string in the "
                      << ans + 1 - to_find.size() << "th place" << '\n';
        }
        fflush(stdout);
    }
    file.close();
    return 0;
}
