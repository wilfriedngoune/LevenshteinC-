#include <iostream>
#include "../headers/Levenshtein.hpp"
#include "../headers/Tests.hpp"





using namespace std;


int main(){
    cout << "Test 1" << endl;
    cout << LevenshteinDistTierTor(); 
    cout << endl;
    cout << endl;

    cout << "Test 2" << endl;
    cout << LevenshteinDistRenTierTiere();
    cout << endl;
    cout << endl;

    cout << "Test 3" << endl;
    cout << LevenshteinDistTiereRenTier();
    cout << endl;
    cout << endl;

    cout << "Test 4" << endl;
    cout << LevenshteinDistWords();
    cout << endl;
    cout << endl;

    cout << "Test 5" << endl;
    PrettyLevenshteinDistWords();

    return 0;
}