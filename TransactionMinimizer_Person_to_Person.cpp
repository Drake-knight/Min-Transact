#include <bits/stdc++.h>
using namespace std;

class Friend {
public:
    string name;
    int netAmount;
    set<string> paymentModes;
};

int getMinIndex(Friend listOfNetAmounts[], int numFriends) {
    int min = INT_MAX, minIndex = -1;
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount < min) {
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(Friend listOfNetAmounts[], int numFriends) {
    int max = INT_MIN, maxIndex = -1;
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount > max) {
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int, string> getMaxIndex(Friend listOfNetAmounts[], int numFriends, int minIndex, Friend input[], int maxNumPaymentModes) {
    int max = INT_MIN;
    int maxIndex = -1;
    string matchingPaymentMode;
    
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;
        if (listOfNetAmounts[i].netAmount < 0) continue;

        vector<string> v(maxNumPaymentModes);
        vector<string>::iterator ls = set_intersection(listOfNetAmounts[minIndex].paymentModes.begin(), listOfNetAmounts[minIndex].paymentModes.end(), listOfNetAmounts[i].paymentModes.begin(), listOfNetAmounts[i].paymentModes.end(), v.begin());
        
        if ((ls - v.begin()) != 0 && max < listOfNetAmounts[i].netAmount) {
            max = listOfNetAmounts[i].netAmount;
            maxIndex = i;
            matchingPaymentMode = *(v.begin());
        }
    }
    
    return make_pair(maxIndex, matchingPaymentMode);
}

void printAns(vector<vector<pair<int, string>>> ansGraph, int numFriends, Friend input[]) {
    cout << "\nThe transactions for minimum cash flow are as follows: \n\n";
    for (int i = 0; i < numFriends; i++) {
        for (int j = 0; j < numFriends; j++) {
            if (i == j) continue;
            if (ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0) {
                if (ansGraph[i][j].first == ansGraph[j][i].first) {
                    ansGraph[i][j].first = 0;
                    ansGraph[j][i].first = 0;
                } else if (ansGraph[i][j].first > ansGraph[j][i].first) {
                    ansGraph[i][j].first -= ansGraph[j][i].first;
                    ansGraph[j][i].first = 0;
                    cout << input[i].name << " pays Rs " << ansGraph[i][j].first << " to " << input[j].name << " via " << ansGraph[i][j].second << endl;
                } else {
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;
                    cout << input[j].name << " pays Rs " << ansGraph[j][i].first << " to " << input[i].name << " via " << ansGraph[j][i].second << endl;
                }
            } else if (ansGraph[i][j].first != 0) {
                cout << input[i].name << " pays Rs " << ansGraph[i][j].first << " to " << input[j].name << " via " << ansGraph[i][j].second << endl;
            } else if (ansGraph[j][i].first != 0) {
                cout << input[j].name << " pays Rs " << ansGraph[j][i].first << " to " << input[i].name << " via " << ansGraph[j][i].second << endl;
            }
            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout << "\n";
}

void minimizeCashFlow(int numFriends, Friend input[], unordered_map<string, int>& indexOf, int numTransactions, vector<vector<int>>& graph, int maxNumPaymentModes) {
    Friend listOfNetAmounts[numFriends];
    
    for (int b = 0; b < numFriends; b++) {
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].paymentModes = input[b].paymentModes;
        
        int amount = 0;
        for (int i = 0; i < numFriends; i++) {
            amount += (graph[i][b]);
        }
        for (int j = 0; j < numFriends; j++) {
            amount += ((-1) * graph[b][j]);
        }
        listOfNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int, string>>> ansGraph(numFriends, vector<pair<int, string>>(numFriends, {0, ""}));
    int numZeroNetAmounts = 0;
    
    for (int i = 0; i < numFriends; i++) {
        if (listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }

    while (numZeroNetAmounts != numFriends) {
        int minIndex = getMinIndex(listOfNetAmounts, numFriends);
        pair<int, string> maxAns = getMaxIndex(listOfNetAmounts, numFriends, minIndex, input, maxNumPaymentModes);
        int maxIndex = maxAns.first;
        
        if (maxIndex == -1) {
            ansGraph[minIndex][0].first += abs(listOfNetAmounts[minIndex].netAmount);
            ansGraph[minIndex][0].second = *(input[minIndex].paymentModes.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numFriends);
            ansGraph[0][simpleMaxIndex].first += abs(listOfNetAmounts[minIndex].netAmount);
            ansGraph[0][simpleMaxIndex].second = *(input[simpleMaxIndex].paymentModes.begin());
            
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            
            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
        } else {
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            
            ansGraph[minIndex][maxIndex].first += transactionAmount;
            ansGraph[minIndex][maxIndex].second = maxAns.second;
            
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            
            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
    }
    
    printAns(ansGraph, numFriends, input);
}

int main() {
    cout << "\n\t\t\t\t********************* Welcome to TRANSACTION MINIMIZER ***********************\n\n\n";
    cout << "This system minimizes the number of transactions among multiple friends in different parts of the world that use different modes of payment. \n\n";
    cout << "Enter the number of friends participating in the transactions.\n";
    int numFriends;
    cin >> numFriends;
    
    Friend input[numFriends];
    unordered_map<string, int> indexOf;
    
    cout << "Enter the names of the friends.\n";
    for (int i = 0; i < numFriends; i++) {
        cout << "Friend " << i + 1 << ": ";
        cin >> input[i].name;
        indexOf[input[i].name] = i;
    }
    
    cout << "Enter the number of transactions.\n";
    int numTransactions;
    cin >> numTransactions;
    
    vector<vector<int>> graph(numFriends, vector<int>(numFriends, 0));
    
    cout << "Enter the details of each transaction as stated:\n";
    cout << "Debtor Friend, Creditor Friend, and Amount\n";
    for (int i = 0; i < numTransactions; i++) {
        cout << (i + 1) << "th transaction: ";
        string debtor, creditor;
        int amount;
        cin >> debtor >> creditor >> amount;
        
        graph[indexOf[debtor]][indexOf[creditor]] = amount;
    }
    
    cout << "Enter the payment modes for each friend.\n";
    for (int i = 0; i < numFriends; i++) {
        cout << "Payment modes for " << input[i].name << " (separated by space, end with -1): ";
        string mode;
        while (cin >> mode && mode != "-1") {
            input[i].paymentModes.insert(mode);
        }
    }

    int maxNumPaymentModes = 0;
    for (int i = 0; i < numFriends; i++) {
        maxNumPaymentModes = max(maxNumPaymentModes, (int)input[i].paymentModes.size());
    }
    
    minimizeCashFlow(numFriends, input, indexOf, numTransactions, graph, maxNumPaymentModes);
    return 0;
}
