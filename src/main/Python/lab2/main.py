class DFA:
    adjList = []
    vector = []


class NFA:
    adjList = []
    vector = []


class Node:
    def __init__(self, qx, vt, qy):
        self.xTerm = qx
        self.yTerm = vt
        self.celula = qy

    xTerm = " "
    Yterm = " "
    celula = " "


Q = []
E = []
F = []
nfa = NFA()
dfa = DFA()
indDFA = 0


def read():
    try:  # read the Q
        temp = input("Q = ")
        for i in range(len(temp)):
            if temp[i].islower():
                Q.append(temp[i] + temp[i + 1])
        # read the Epsilon
        temp = input("E = ")
        for i in range(len(temp)):
            if temp[i].islower():
                E.append(temp[i])
        # read the Final
        temp = input("F = ")
        for i in range(len(temp)):
            if temp[i].islower():
                F.append(temp[i] + temp[i + 1])
    finally:
        pass


def writing():
    tempstring = ','
    while tempstring[len(tempstring) - 1] == ',':
        tempstring = input("Delta >> ")
        tmp = Node(tempstring[1] + tempstring[2], tempstring[4], tempstring[7] + tempstring[8])
        nfa.vector.append(tmp)


def creating(fa):
    fa.adjList = [["" for i in range(len(E))] for j in range(len(Q))]
    for i in range(len(E)):
        for j in range(len(fa.vector)):
            if E[i] == fa.vector[j].yTerm:
                k = int(fa.vector[j].xTerm[1])
                fa.adjList[k][i] += fa.vector[j].celula
    print(fa.adjList)


def converting(q0):
    global indDFA
    b = True
    for i in range(indDFA+1):
        if dfa.vector[i] == q0:
            return None

    if b == True:
        dfa.vector.insert(len(dfa.vector),q0)
        #print(dfa.vector)
        indDFA = indDFA + 1
        adjtemp=[]
        adjtemp = nfa.adjList[int(q0[1])][:]
        if len(q0) > 2:
            ind = 2
            t = len(q0)
            while ind < t:
                index = int(q0[ind + 1])
                adjtemp[0] = adjtemp[0] + nfa.adjList[index][0]
                adjtemp[1] = adjtemp[1] + nfa.adjList[index][1]
                adjtemp[2] = adjtemp[2] + nfa.adjList[index][2]
                ind = ind + 2
        dfa.adjList.append(adjtemp)
        print(dfa.adjList[indDFA])
        for k in range(len(E)):
            if dfa.adjList[indDFA][k] != "":
                converting(dfa.adjList[indDFA][k])

def printing(fa):
    print("   "+"a    "+"b    "+"c")
    for i in range(len(fa.vector)):
        print(fa.vector[i])
        print(fa.adjList[i])

read()
writing()
creating(nfa)
dfa.adjList.append(nfa.adjList[0])
dfa.vector.append("q0")
for k in range(len(E)):
    if dfa.adjList[0][k] != "":
        converting(dfa.adjList[0][k])
printing(dfa)