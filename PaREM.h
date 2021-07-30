#include <iostream>
#include "DFA.h"

#define PB push_back

using namespace std;

typedef vector<bool> vb;
typedef vector<int> vi;
typedef vector< pair<int, int> > vpii;

enum Mode {
    RegexMatch,
    AutomatonMatch
};

bool dfs(vector<vi>* I, vector<bool>* hasF, int p, DFA& dfa, Mode mode);
bool dfs_helper(DFA& dfa, vector<vi>* I, vector<bool>* hasF, int idx, int jdx, int p, Mode mode);
bool PaREM(DFA& dfa, const string& T, int p, Mode mode);

bool PaREM(DFA& dfa, const string& T, int p, Mode mode) {

    int start_position;
    const int chunk_size = T.length()/p;

    vector <vi> I [p];
    vector <bool> hasF [p];

    #pragma omp parallel private(start_position)
    {

        int i = omp_get_thread_num();
        start_position = i*(chunk_size);
        
        vi R;
        int j = 0;

        if (i != 0) {
            vb S(dfa.Q.size(), false);
            vb L(dfa.Q.size(), false);
            for (auto& q : dfa.Q) {
                if (dfa.belongs_to_Q(q, T[start_position])) 
                    S[j] = true;
                j++;
            }
            for (auto& q : dfa.Q) {
                if (dfa.belongs_to_Q(q, T[start_position-1]))
                    L[dfa.Tt[q][string(1, T[start_position-1])]] = true;
            }
            for (j = 0; j < dfa.Q.size(); j++){
                if (S[j] && L[j]) R.PB(j);
            }
        }

        else R.PB(dfa.q0);

        int k;

        for (auto r : R) {
            bool flag = 0, foundQ = false;
            vi Rr;
            k = r;
            Rr.PB(r);
            int end_position;
            if (i < p-1) 
                end_position = start_position+chunk_size;
            else end_position = T.length();
            for (j = start_position; j < end_position; ++j) {
                if (!dfa.belongs_to_Q(k, T[j])) { flag = 1; break; }

                if (dfa.belongs_to_F(k)) foundQ = true;

                k = dfa.Tt[k][string(1, T[j])];
                Rr.PB(k);
            }
            if (flag) continue;
            I[i].PB(Rr);
            hasF[i].PB(foundQ);
        }
        
    }
    
    //print_I(I, p);

    return dfs(I, hasF, p, dfa, mode);
    
}

void print_I(vector<vi>* I, int p){
    for (int f = 0; f < p; f++) {
        cout << "p" << f << ": " << endl;
        for (auto v : I[f]) {
            for (auto e : v) 
                cout << e << " ";
            cout << endl;
        }
    }
}

bool dfs(vector<vi>* I, vector<bool>* hasF, int p, DFA& dfa, Mode mode) {
    bool si = false;
    for (auto v : I[0]) {
        int j = 0;
        for (auto e : I[1]) {
            if (v.back() == e.front()) {
                //cout << v.back() << " hizo match con " << e.front() << "\n";
                si = dfs_helper(dfa, I, hasF, 1, j, p, mode);
                if (si) return true;
            }
            j++;
            //cout << endl;
        }
    }
    return false;
}

bool dfs_helper(DFA& dfa, vector<vi>* I, vector<bool>* hasF, int idx, int jdx, int p, Mode mode) {
    // idx = p
    // jdx = lista del proceso actual
    int j = 0;
    if (idx != p-1) {
        if (mode == RegexMatch && hasF[idx][jdx]) return true;
        bool si = false;
        int v = I[idx][j].back();
        for (auto e : I[idx+1]) {
            if (v == e.front()) {
                //cout << v << " hizo match con " << e.front() << "\n";
                si = dfs_helper(dfa, I, hasF, idx+1, j, p, mode);
                if (si) return true;
            }
            j++;
        }
        return false;
    } else {
        int v = I[idx][j].back();
        //cout << "llegué al estado final " << v << "\n";
        if (dfa.belongs_to_F(v)) {
            //cout << "pertenece\n";
            return true;
        } else {
            //cout << "no pertenece\n";
            return false;
        }
    }
}
