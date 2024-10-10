#include <iostream>
#include <algorithm>
#include "../headers/Levenshtein.hpp"
#include <unordered_set>
#include <vector>
#include <string>

// Declaration of Hilffunktionnen
std::vector<std::string> EqualPart(const std::vector<std::string>& str1, const std::vector<std::string>& str2);
void constructStr1(std::vector<std::string>& finalTab1, 
                   const std::vector<std::string>& finalTab2, 
                   const std::vector<std::string>& tab, 
                   const std::vector<std::string>& equalPart);
int firstOccurenceOf(const std::vector<std::string>& table, const std::vector<std::string>& subTable);
void constructStr2(std::vector<std::string>& finalTab, const std::vector<std::string>& Tab, bool fromStart) ;


// Constructor
Levenshtein::Levenshtein(const std::vector<std::string>& astr1, const std::vector<std::string>& astr2)
    : mstr1(astr1), mstr2(astr2), 
      mi_zCnt_m1(astr1.size()), mi_spCnt_n2(astr2.size()) 
{
    if (mi_zCnt_m1 + 1 > 1000 || mi_spCnt_n2 + 1 > 1000) {
        throw std::runtime_error("Error !! Grosse von mpi_mat nicht genug !");
    }
}


// Lev Distace calculation
int Levenshtein::CalcLevenshteinDistance() {
    mpi_mat[0] = 0; 
    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        mpi_mat[i * (mi_spCnt_n2 + 1)] = i; 
    }
    for (int j = 1; j <= mi_spCnt_n2; ++j) {
        mpi_mat[j] = j; 
    }

    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        for (int j = 1; j <= mi_spCnt_n2; ++j) {
            int cost = (mstr1[i - 1] == mstr2[j - 1]) ? 0 : 1;
            int del = mpi_mat[(i - 1) * (mi_spCnt_n2 + 1) + j] + 1; 
            int ins = mpi_mat[i * (mi_spCnt_n2 + 1) + (j - 1)] + 1; 
            int sub = mpi_mat[(i - 1) * (mi_spCnt_n2 + 1) + (j - 1)] + cost; 
             
            mpi_mat[i * (mi_spCnt_n2 + 1) + j] = std::min({del, ins, sub});
        }
    }

    return mpi_mat[mi_zCnt_m1 * (mi_spCnt_n2 + 1) + mi_spCnt_n2];
}


// Back trace function
std::string Levenshtein::backtrace() const {
    int matrix[1000];
    matrix[0] = 0; 
    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        matrix[i * (mi_spCnt_n2 + 1)] = i; 
    }
    for (int j = 1; j <= mi_spCnt_n2; ++j) {
        matrix[j] = j; 
    }

    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        for (int j = 1; j <= mi_spCnt_n2; ++j) {
            int cost = (mstr1[i - 1] == mstr2[j - 1]) ? 0 : 1;
            int del = matrix[(i - 1) * (mi_spCnt_n2 + 1) + j] + 1; 
            int ins = matrix[i * (mi_spCnt_n2 + 1) + (j - 1)] + 1; 
            int sub = matrix[(i - 1) * (mi_spCnt_n2 + 1) + (j - 1)] + cost; 
             
            matrix[i * (mi_spCnt_n2 + 1) + j] = std::min({del, ins, sub});
        }
    }
    // Levenshtein Distance
    int levDist = matrix[mi_zCnt_m1 * (mi_spCnt_n2 + 1) + mi_spCnt_n2];
    std::vector<std::string> equalSubString = EqualPart(mstr1, mstr2);
    int tableLength = equalSubString.size() + levDist;


    // Creation of the two tables
    std::vector<std::string> str1NewTab(tableLength); 
    std::vector<std::string> str2NewTab(tableLength); 
    

    // Construction Str2
    bool str2FromStart = firstOccurenceOf(mstr1, equalSubString) < firstOccurenceOf(mstr2, equalSubString);
    constructStr2(str2NewTab, mstr2, str2FromStart);

    // Construction of Str1
    constructStr1(str1NewTab, str2NewTab, mstr1, equalSubString);

    //COmputation of the steps
    std::string steps;
    for(int i=0;i<tableLength;i++){
            if(str1NewTab[i] != "" && str2NewTab[i] != "" ){
                if(str1NewTab[i] == str2NewTab[i]){
                    steps = steps + "Eq";
                }
                else{
                    steps = steps + "Subs";
                }
            }
            if(str1NewTab[i] != "" && str2NewTab[i] == ""){
                 steps = steps + "Del";
            }
            if(str1NewTab[i] == "" && str2NewTab[i] != ""){
                steps = steps + "Ins";
            }
            
            steps = steps + " ";
    }

    return steps;
}

// Pretty Print function
std::string Levenshtein::GetPrettyPrint() const {
    int matrix[1000];
    matrix[0] = 0; 
    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        matrix[i * (mi_spCnt_n2 + 1)] = i; 
    }
    for (int j = 1; j <= mi_spCnt_n2; ++j) {
        matrix[j] = j; 
    }

    for (int i = 1; i <= mi_zCnt_m1; ++i) {
        for (int j = 1; j <= mi_spCnt_n2; ++j) {
            int cost = (mstr1[i - 1] == mstr2[j - 1]) ? 0 : 1;
            int del = matrix[(i - 1) * (mi_spCnt_n2 + 1) + j] + 1; 
            int ins = matrix[i * (mi_spCnt_n2 + 1) + (j - 1)] + 1; 
            int sub = matrix[(i - 1) * (mi_spCnt_n2 + 1) + (j - 1)] + cost; 
             
            matrix[i * (mi_spCnt_n2 + 1) + j] = std::min({del, ins, sub});
        }
    }
    // Levenshtein Distance
    int levDist = matrix[mi_zCnt_m1 * (mi_spCnt_n2 + 1) + mi_spCnt_n2];
    std::vector<std::string> equalSubString = EqualPart(mstr1, mstr2);
    int tableLength = equalSubString.size() + levDist;

    // Creation of the two tables
    std::vector<std::string> str1NewTab(tableLength); 
    std::vector<std::string> str2NewTab(tableLength); 

    

    // Construction Str2
    bool str2FromStart = firstOccurenceOf(mstr1, equalSubString) < firstOccurenceOf(mstr2, equalSubString);
    constructStr2(str2NewTab, mstr2, str2FromStart);

    // Construction of Str1
    constructStr1(str1NewTab, str2NewTab, mstr1, equalSubString);
     
    // Formating of the pretty string
    std::string steps;
    steps = steps + "s1: ";
    for (const auto& str : str1NewTab) {
        if(str == ""){
            steps = steps + "_";
        }
        else{
            steps = steps + str;
        }
        steps = steps + " ";
    }
    steps = steps + "\n";
    steps = steps + "s2: ";
    for (const auto& str : str2NewTab) {
        if(str == ""){
            steps = steps + "_";
        }
        else{
            steps = steps + str;
        }
        steps = steps + " ";
    }

    steps = steps + "\n";

    return steps;
}


// {*Hilffunktionnen*}

// Function to compute the biggest equal part in two string
std::vector<std::string> EqualPart(const std::vector<std::string>& str1, const std::vector<std::string>& str2) {

    int n = str1.size();
    int m = str2.size();
    int limite = m;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    int longueurMax = 0;
    int finIndex = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > longueurMax && dp[i][j] <= limite) {
                    longueurMax = dp[i][j];
                    finIndex = i; 
                }
            }
        }
    }

    std::vector<std::string> sousTableauCommun;
    if (longueurMax > 0) {
        int startIndex = finIndex - longueurMax;
        for (int i = startIndex; i < finIndex; ++i) {
            sousTableauCommun.push_back(str1[i]);
        }
    }

    return sousTableauCommun;
}


// Function to get the first occurence of a substring subtable in a string table
int firstOccurenceOf(const std::vector<std::string>& table, const std::vector<std::string>& subTable) {
    size_t n = table.size();
    size_t m = subTable.size();

    for (size_t i = 0; i <= n - m; ++i) {
        bool match = true;
        for (size_t j = 0; j < m; ++j) {
            if (table[i + j] != subTable[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i; 
        }
    }
    return -1; 
}

// Function to reconstruct the string 1 but the consideration of the offseet
void constructStr1(std::vector<std::string>& finalTab1, 
                   const std::vector<std::string>& finalTab2, 
                   const std::vector<std::string>& tab, 
                   const std::vector<std::string>& equalPart) {
    std::fill(finalTab1.begin(), finalTab1.end(), "");

    std::string equalPartStr = "";
    for (const auto& part : equalPart) {
        equalPartStr += part;
    }

    std::string tabStr = "";
    for (const auto& t : tab) {
        tabStr += t;
    }

    std::string finalTab2Str = "";
    for (const auto& str : finalTab2) {
        finalTab2Str += str;
    }

    size_t pos = finalTab2Str.find(equalPartStr);
    if (pos != std::string::npos) {
        size_t startIndex = pos; 

        if (startIndex + tab.size() <= finalTab1.size()) {
            for (size_t i = 0; i < tab.size(); ++i) {
                finalTab1[startIndex + i] = tab[i];
            }
        } else {
            for (size_t i = 0; i < finalTab1.size() && i < tab.size(); ++i) {
                finalTab1[i] = tab[i];
            }
        }
    } else {
        for (size_t i = 0; i < finalTab1.size() && i < tab.size(); ++i) {
            finalTab1[i] = tab[i];
        }
    }
}

// Function to reconstruct the secont string with consideration of tge offset
void constructStr2(std::vector<std::string>& finalTab, const std::vector<std::string>& Tab, bool fromStart) {
    std::fill(finalTab.begin(), finalTab.end(), "");

    if (fromStart) {
        for (size_t i = 0; i < Tab.size() && i < finalTab.size(); ++i) {
            finalTab[i] = Tab[i];
        }
    } else {
        size_t startIndex = finalTab.size() - Tab.size();
        for (size_t i = 0; i < Tab.size() && startIndex + i < finalTab.size(); ++i) {
            finalTab[startIndex + i] = Tab[i];
        }
    }
}



