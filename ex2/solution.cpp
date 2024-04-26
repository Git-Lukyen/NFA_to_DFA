#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#import <queue>

using namespace std;

string pretty_group(const vector<int> &vec) {
    string pretty_string;

    for (int num: vec)
        pretty_string += to_string(num);

    return pretty_string;
}

int main() {
    ifstream fin("/Users/lucian/Desktop/facultate/lfa_tema2/ex2/input.txt");
    ofstream fout("/Users/lucian/Desktop/facultate/lfa_tema2/ex2/output.txt");

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
    map<int, map<char, int>> transitions;

    fin >> nr_transitions;
    for (int i = 1; i <= nr_transitions; i++) {
        int initial_state, end_state;
        char transition_letter;

        fin >> initial_state >> transition_letter >> end_state;

        transitions[initial_state][transition_letter] = end_state;
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


    vector<vector<int>> initial, next;

    {
        vector<int> initial_start, initial_end;
        for (int state: states)
            if (find(end_states.begin(), end_states.end(), state) == end_states.end())
                initial_start.push_back(state);
            else
                initial_end.push_back(state);
        initial.push_back(initial_start), initial.push_back(initial_end);
    }

    int countdown = 1000;

    while (countdown--) {
        next = vector<vector<int>>{(vector<int>{(*initial[0].begin())})};

        for (vector<int> &it_vec: initial)
            for (int elem: it_vec) {

                if (find(end_states.begin(), end_states.end(), elem) != end_states.end())
                    continue;

                bool compatible = false;

                for (vector<int> &cmp_vec: next) {
                    int cmp_elem = *cmp_vec.begin();

                    if (elem == cmp_elem || compatible) {
                        compatible = true;
                        continue;
                    }

                    bool is_equiv = true;

                    for (char letter: alphabet) {
                        is_equiv &=
                                *find_if(initial.begin(), initial.end(), [&](vector<int> temp_vec) {
                                             return find(temp_vec.begin(), temp_vec.end(), transitions[elem][letter]) !=
                                                    temp_vec.end();
                                         }
                                ) == *find_if(initial.begin(), initial.end(),
                                              [&](vector<int> temp_vec) {
                                                  return find(temp_vec.begin(), temp_vec.end(),
                                                              transitions[cmp_elem][letter]) != temp_vec.end();
                                              }
                                );
                    }

                    if (is_equiv)
                        cmp_vec.push_back(elem), compatible = true;
                }

                if (!compatible)
                    next.push_back(vector<int>{(elem)});
            }

        initial = next;
    }


    fout << "\nTOTAL STATES: ";
    for (const vector<int> &state: next)
        fout << pretty_group(state) << " ";

    for (int num: end_states)
        fout << num << " ";

    fout << "\n\nALPHABET: ";
    for (char &chr: alphabet)
        fout << chr << " ";

    fout << "\n\nFINAL TRANSITIONS:\n";
    for (const vector<int> &state: next)
        for (int num: state)
            for (char letter:alphabet)
                fout << pretty_group(state) << " " << letter << " " << transitions[num][letter] << "\n";

    fout << "\n\nSTART STATE: " << start_state;

    fout << "\n\nEND STATES: ";
    for (int num: end_states)
        fout << num << " ";
    fout << "\n";

    return 0;
}
