//zereg manar 3 eme anne ing network G:02
//ziani sara 3 eme anne ing securite G:02
#include <stdio.h>
#include <stdlib.h>
#define MAX_states 10
#define MAX_trans 10
#define EPS -1
typedef struct {
    int to;
    int symbol; // a,b,c or epsilon
} transition;

typedef struct {
    transition trans[MAX_trans];
    int count;
} state;

state NFA[MAX_states];
int nbstates = 4;
int initialstate = 0;

int finalstates[MAX_states];
int nbfinalstates = 1;

/* ---------- Read NFA ---------- */
void readNFA() {
    NFA[0].trans[0] = (transition){1, EPS};
    NFA[0].count = 1;

    NFA[1].trans[0] = (transition){1, 'a'};
    NFA[1].trans[1] = (transition){1, 'b'};
    NFA[1].trans[2] = (transition){2, EPS};
    NFA[1].count = 3;

    NFA[2].trans[0] = (transition){2, 'c'};
    NFA[2].trans[1] = (transition){3, EPS};
    NFA[2].count = 2;

    NFA[3].count = 0;

    finalstates[0] = 3;   // original final state
}

/* ---------- ε-closure ---------- */
void computeEclosure(int state, int eclosure[], int *size) {
    int stack[MAX_states], top = 0;
    int visited[MAX_states] = {0};

    stack[top++] = state;
    visited[state] = 1;

    while (top > 0) {
        int s = stack[--top];
        eclosure[(*size)++] = s;

        for (int i = 0; i < NFA[s].count; i++) {
            if (NFA[s].trans[i].symbol == EPS) {
                int next = NFA[s].trans[i].to;
                if (visited[next] == 0) {
                    visited[next] = 1;
                    stack[top++] = next;
                }
            }
        }
    }
}

/* ---------- Display ε-closures ---------- */
void removeEpsilon() {
    int eclosure[MAX_states];
    int size;

    printf("\nremove epsilon transition:\n");
    for (int s = 0; s < nbstates; s++) {
        size = 0;
        computeEclosure(s, eclosure, &size);

        printf("E_closure(%d)={", s);
        for (int i = 0; i < size; i++) {
            printf("%d", eclosure[i]);
        }
        printf("}\n");
    }
}
/* ---------- Compute new transitions (without epsilon) ---------- */
void computeNewTransitions() {
    int eclosure1[MAX_states], size1;
    int eclosure2[MAX_states], size2;
    int visited[MAX_states];

    char symbols[3] = {'a', 'b', 'c'};

    printf("\nNew transitions (without epsilon):\n");

    for (int s = 0; s < nbstates; s++) {
        for (int sym = 0; sym < 3; sym++) {

            // reset visited
            for (int i = 0; i < MAX_states; i++)
                visited[i] = 0;

            // ε-closure(s)
            size1 = 0;
            computeEclosure(s, eclosure1, &size1);

            // from ε-closure(s) by symbol
            for (int i = 0; i < size1; i++) {
                int p = eclosure1[i];

                for (int t = 0; t < NFA[p].count; t++) {
                    if (NFA[p].trans[t].symbol == symbols[sym]) {
                        int dest = NFA[p].trans[t].to;

                        // ε-closure(dest)
                        size2 = 0;
                        computeEclosure(dest, eclosure2, &size2);

                        for (int k = 0; k < size2; k++)
                            visited[eclosure2[k]] = 1;
                    }
                }
            }

            // display transition
            int printed = 0;
            for (int i = 0; i < nbstates; i++) {
                if (visited[i]) {
                    if (!printed) {
                        printf("δ(%d, %c) = { ", s, symbols[sym]);
                        printed = 1;
                    }
                    printf("%d ", i);
                }
            }
            if (printed)
                printf("}\n");
        }
    }
}

/* ---------- Compute final states ---------- */
void computeFinalstates() {
    int eclosure[MAX_states];
    int size;

    printf("\nfinal states after epsilon removal:\n");

    for (int s = 0; s < nbstates; s++) {
        if(s==initialstate)
        continue;
        size = 0;
        computeEclosure(s, eclosure, &size);

        for (int i = 0; i < size; i++) {
            for (int f = 0; f < nbfinalstates; f++) {
                if (eclosure[i] == finalstates[f]) {
                    printf("state %d is final\n", s);
                    goto next_state;
                }
            }
        }
        next_state:;
    }
}

/* ---------- Display NFA ---------- */
void displayNFA() {
    printf("\nNFA transition:\n");
    for (int s = 0; s < nbstates; s++) {
        for (int i = 0; i < NFA[s].count; i++) {
            if (NFA[s].trans[i].symbol == EPS)
                printf("%d --eps--> %d\n", s, NFA[s].trans[i].to);
            else
                printf("%d --%c--> %d\n", s, NFA[s].trans[i].symbol, NFA[s].trans[i].to);
        }
    }
}
/* ---------- Display New Automaton ---------- */
void displayNewAutomaton() {
    int eclosure1[MAX_states], size1;
    int eclosure2[MAX_states], size2;
    int visited[MAX_states];
    char symbols[3] = {'a', 'b', 'c'};

    printf("\n===== New Automaton (without epsilon) =====\n");

    printf("States: { ");
    for (int i = 0; i < nbstates; i++)
        printf("%d ", i);
    printf("}\n");

    printf("Initial state: %d\n", initialstate);

    printf("Final states: { ");
    for (int s = 0; s < nbstates; s++) {
        if (s == initialstate) continue;

        size1 = 0;
        computeEclosure(s, eclosure1, &size1);
        for (int i = 0; i < size1; i++) {
            if (eclosure1[i] == finalstates[0]) {
                printf("%d ", s);
                break;
            }
        }
    }
    printf("}\n");

    printf("\nTransitions:\n");

    for (int s = 0; s < nbstates; s++) {
        for (int sym = 0; sym < 3; sym++) {

            for (int i = 0; i < MAX_states; i++)
                visited[i] = 0;

            size1 = 0;
            computeEclosure(s, eclosure1, &size1);

            for (int i = 0; i < size1; i++) {
                int p = eclosure1[i];
                for (int t = 0; t < NFA[p].count; t++) {
                    if (NFA[p].trans[t].symbol == symbols[sym]) {
                        int dest = NFA[p].trans[t].to;
                        size2 = 0;
                        computeEclosure(dest, eclosure2, &size2);
                        for (int k = 0; k < size2; k++)
                            visited[eclosure2[k]] = 1;
                    }
                }
            }

            int printed = 0;
            for (int i = 0; i < nbstates; i++) {
                if (visited[i]) {
                    if (!printed) {
                        printf("%d --%c--> { ", s, symbols[sym]);
                        printed = 1;
                    }
                    printf("%d ", i);
                }
            }
            if (printed)
                printf("}\n");
        }
    }
}

/* ---------- Main ---------- */
int main() {
    readNFA();
    displayNFA();
    removeEpsilon();
    computeNewTransitions();
    computeFinalstates();
    displayNewAutomaton();
    
    return 0;
}
