#include <string>
#include <vector>

#ifndef LEVENSHTEIN_HPP
#define LEVENSHTEIN_HPP

class Levenshtein
{
    public:
    // astr1
    Levenshtein(
    const std::vector<std::string>& astr1, // first string compared to astr2
    const std::vector<std::string>& astr2
    ); // i.e. LD between
    // astr1 and astr2 is calculated

    //! returning the Levenshtein distance between astr1 and astr2
    int CalcLevenshteinDistance();
    std::string backtrace() const;
    std::string GetPrettyPrint() const;
    /* maybe other methods */
    private:
    // first index runs over size of mstr1
    int Get(int st1Ind, int st2Ind) const {
    return (mpi_mat[st1Ind * mi_spCnt_n2 + st2Ind]);
    }
    std::vector<std::string> mstr1;
    std::vector<std::string> mstr2;
    int mi_zCnt_m1; // size of mstr1
    int mi_spCnt_n2; // size of mstr2
    // contains the matrix, used for LD calculation as a vector
    int mpi_mat[1000]; // later we create a dynamic matrix, currently sizes
    // of astr1 and astr2 are limited to 30
    /* maybe other members */
};

#endif
