#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> pfc(vector<int> score){
    int choix1;
    int choix2;
    cin >> choix1;
    cin >> choix2;

    if (choix2 == choix1 + 1)
        return {score[0], score[1] + 1};
    else if (choix1 == choix2)
        return score;
    else
        return {score[1] + 1, score[0]};
}

vector<int> des(vector<int> score){
    int choix1;
    int choix2;
    cin >> choix1;
    cin >> choix2;

    if (choix2 > choix1)
        return {score[0], score[1] + 1};
    else if (choix1 == choix2)
        return score;
    else
        return {score[0] + 1, score[1]};

}

vector<int> pf_facegagnant(vector<int> score){
    string choix1;
    string choix2;
    cin >> choix1;
    cin >> choix2;

    if (choix1 == choix2)
        return score;
    else if (choix1 == "face")
        return {score[0] + 1, score[1]};
    else if (choix2 == "pile")
        return {score[0], score[1] + 1};
}


vector<int> bO3(string j1, string j2){
    vector<int> score =  {0, 0};
    score = pfc(score);
    score = des(score);
    if (score[0] == score[1])
        score = pf_facegagnant(score);
    return score;
}


int main(){
    return 0;
}
