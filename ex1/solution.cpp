#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#import <queue>

using namespace std;

string pretty_state(const set<int> &nums) {
    string pretty_state;

    for (int number: nums)
        pretty_state += to_string(number);

    return pretty_state;
}

int main() {
    ifstream fin("/Users/lucian/Desktop/facultate/lfa_tema2/ex1/input.txt");
    ofstream fout("/Users/lucian/Desktop/facultate/lfa_tema2/ex1/output.txt");

    int nr_states;
    fin >> nr_states;

    vector<int> states;
    vector<char> alphabet;

    for (int i = 1; i <= nr_states; i++) {
        int state;
        fin >> state;

        states.push_back(state);
    }

    int alphabet_length;
    fin >> alphabet_length;

    for (int i = 1; i <= alphabet_length; i++) {
        char letter;
        fin >> letter;

        alphabet.push_back(letter);
    }

    int nr_transitions;
    map<set<int>, map<char, set<int>>> transitions;

    queue<set<int>> new_states;

    fin >> nr_transitions;
    for (int i = 1; i <= nr_transitions; i++) {
        int initial_state, end_state;
        char transition_letter;

        fin >> initial_state >> transition_letter >> end_state;

        transitions[set<int>({initial_state})][transition_letter].emplace(end_state);
    }

    int start_state;
    fin >> start_state;

    int nr_end_states;
    fin >> nr_end_states;

    vector<int> end_states;
    for (int i = 1; i <= nr_end_states; i++) {
        int end_state;
        fin >> end_state;

        end_states.push_back(end_state);
    }

    for (int state: states)
        new_states.push(set<int>{(state)});

    while (!new_states.empty()) {
        set<int> new_state = new_states.front();
        new_states.pop();

        for (const auto &entry: transitions[new_state])
            if (transitions[entry.second].empty())
                new_states.push(entry.second);

        if (transitions[new_state].empty())
            for (char &letter: alphabet) {
                set<int> union_transition;

                for (int sub_state: new_state)
                    union_transition.insert(transitions[set<int>{(sub_state)}][letter].begin(),
                                            transitions[set<int>{(sub_state)}][letter].end());

                transitions[new_state][letter] = union_transition;
                new_states.push(new_state);
            }

    }

    for (const auto &entry: transitions) {
        for (char &letter: alphabet)
            if (transitions[entry.first][letter].empty())
                transitions[entry.first][letter] = set<int>{(-1)};
    }

    fout << "\nTOTAL STATES: ";
    for (const auto &entry: transitions)
        fout << pretty_state(entry.first) << " ";

    fout << "\n\nALPHABET: ";
    for (char &chr: alphabet)
        fout << chr << " ";

    fout << "\n\nFINAL TRANSITIONS:\n";
    for (const auto &entry: transitions)
        for (char &letter: alphabet)
            fout << pretty_state(entry.first) << " " << letter << " " << pretty_state(transitions[entry.first][letter])
                 << "\n";

    fout << "\n\nSTART STATE: " << start_state;

    fout << "\n\nEND STATES: ";
    for (const auto& entry: transitions) {
        set<int> diff_state = entry.first;
        diff_state.insert(end_states.begin(), end_states.end());

        if (diff_state.size() < entry.first.size() + end_states.size())
            fout << pretty_state(diff_state) << " ";
    }
    fout << "\n";

    return 0;
}
