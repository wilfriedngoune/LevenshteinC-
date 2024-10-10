#include <vector>
#include <string>
#include "../headers/Levenshtein.hpp"
#include <iostream>


 inline std::ostream& operator<<(
   std::ostream& str, const std::vector<std::string>& v)
   {
   std::string blank = "";
   for (auto iter : v)
   {
   str << blank << iter;
   blank = " ";
   }
   return str;
 }


bool LevenshteinDistTierTor()
{
    std::vector<std::string> s1 = {"T", "i", "e" , "r" };
    std::vector<std::string> s2 = { "T", "o", "r" };
    Levenshtein dist(s1, s2);
    return 2 == dist.CalcLevenshteinDistance();
}

bool LevenshteinDistRenTierTiere()
 {
    std::vector<std::string> s1{  "R", "e", "n", "T", "i", "e", "r" };
    std::vector<std::string> s2{  "T", "i", "e", "r", "e"  };
    Levenshtein dist(s1, s2);
   std::cout << "Needed steps: " << dist.backtrace() << std::endl;
   return (4 == dist.CalcLevenshteinDistance());
 }

 bool LevenshteinDistTiereRenTier()
 {
   std::vector<std::string> s1{ "T", "i", "e", "r", "e" };
   std::vector<std::string> s2{ "R", "e", "n", "T", "i", "e", "r" };
   Levenshtein dist(s1, s2);
   std::cout << "Needed steps: " << dist.backtrace() << std::endl;
   return (4 == dist.CalcLevenshteinDistance());
 }



 bool LevenshteinDistWords()
 {
    std::vector<std::string> s1{ "Tango", "ind", "echo", "romeo" };
    std::vector<std::string> s2{ "Ind", "echo", "romeo", "abba"};
    Levenshtein dist(s1, s2);
    std::cout << "Needed steps: " << dist.backtrace() << std::endl;
    return (3 == dist.CalcLevenshteinDistance());
 } 

 bool PrettyLevenshteinDistWords()
 {
   std::vector<std::string> s1{ "Tango", "ind", "echo", "romeo" };
   std::vector<std::string> s2{ "Ind", "echo", "romeo", "abba"};
   Levenshtein dist(s1, s2);
   auto ld = dist.CalcLevenshteinDistance();
   std::cout << dist.GetPrettyPrint() << "\n";
   return 3 == ld;
 }