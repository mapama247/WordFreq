#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
using namespace std;

namespace std
{
template <typename T1, typename T2>
ostream &operator<<(ostream &out, pair<T1, T2> const &p)
{
    return out << setw(10) << p.first << setw(10) << " " << p.second;
}
} // namespace std

void my_print(vector<string> s)
{
    copy(begin(s), end(s), ostream_iterator<string>(cout, " "));
    cout << "\n";
}

void print_pair(vector<pair<string, int>> s)
{
    copy(begin(s), end(s), ostream_iterator<pair<string, int>>(cout, "\n"));
    cout << "\n";
}

vector<string> clean_text(vector<string> words)
{
    transform(begin(words), end(words), begin(words), [&words](string &s) {
        bool invalid_word = false;
        //REMOVE LEADING JUNK
        int pos = s.find_first_not_of("(\"'");
        if (pos != string::npos)
            s.erase(0, pos);
        //REMOVE TRAILING JUNK
        pos = s.find_last_not_of("!?);:,.'\"");
        if (pos != string::npos)
            s.erase(pos + 1, s.size());
        //REMOVE POSSESSIVES
        if ((s.at(s.size() - 1) == 's') && (s.at(s.size() - 2) == '\''))
            s.erase(s.size() - 2, s.size() - 1);
        //REMOVE SHORT WORDS
        if (s.size() < 3)
            s.erase(0, s.size());
        //REMOVE WORDS WITH JUNK CHARACTERS INSIDE
        pos = s.find_first_of("!?();:,.'\"");
        if (pos != string::npos)
            s.erase(0, s.size());
        //REMOVE WRONG WORDS WITH HYPHENS
        pos = s.find_first_of("-");
        if (pos >= 0 && (pos == s.size() - 1 || pos == 0 || s.at(pos + 1) == '-'))
            s.erase(0, s.size());
        //REMOVE WORDS THAT DO NOT CONTAIN ONLY LETTERS OR SIMPLE HYPHENS
        transform(begin(s), end(s), begin(s), [&invalid_word](char &c) {
            if ((!('A' <= c && c <= 'Z') && !('a' <= c && c <= 'z') && !(c == '-')))
            {
                invalid_word = true;
            }
            return tolower(c); //convert all letters to lower case
        });
        if (invalid_word)
        {
            s.erase(0, s.size());
            invalid_word = false;
        }
        return s;
    });
    return words;
}

map<string, int> from_vector_to_map(vector<string> words, map<string, int> m)
{
    for_each(begin(words), end(words), [&m](string &s) {
        if (!s.empty())
            m[s]++;
        return s;
    });
    return m;
}

vector<pair<string, int>> from_map_to_vector(vector<pair<string, int>> v, map<string, int> m)
{
    for_each(begin(m), end(m), [&v](auto &p) {
        v.push_back(p);
        return p;
    });
    return v;
}

void order_and_print(map<string, int> m, string s, int n, vector<string> w)
{
    if (s == "-a")
    {
        cout << "\nLIST OF WORDS IN ALPHABETICAL ORDER: " << endl;
        ostream_iterator<pair<string, int>> oos{cout, "\n"};
        copy(begin(m), end(m), oos);
    }
    else if (s == "-f")
    {
        cout << "\nLIST OF WORDS IN FREQUENCY ORDER: " << endl;
        vector<pair<string, int>> v;
        v = from_map_to_vector(v, m);
        sort(v.begin(), v.end(), [](const auto &a, const auto &b) {
            return a.second > b.second;
        });
        print_pair(v);
    }
    else if (s == "-o")
    {
        cout << "\nPRINT ORIGINAL TEXT IN LINES OF " << n << " CHARACTERS:" << endl;
        int counter = 0;
        transform(begin(w), end(w), begin(w), [&counter, &n](string &s) {
            if (!s.empty())
            {
                if (counter + s.size() < n)
                {
                    counter = counter + s.size();
                    cout << s << " ";
                }
                else
                {
                    cout << endl;
                    cout << s << " ";
                    counter = s.size();
                }
            }
            return s;
        });
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    vector<string> words{};
    vector<string> unsorted_words{};
    map<string, int> m{};

    if (argc == 1)
    {
        cerr << "Error: No arguments given" << endl;
        cerr << "Usage: a.out FILE [-a] [-f] [-o N]" << endl;
        exit(EXIT_FAILURE);
    }
    else if (argc > 1)
    {
        ifstream file{argv[1]};
        istream_iterator<string> iib{file};
        istream_iterator<string> iie{};
        copy(iib, iie, back_inserter(words));
        if (words.size() == 0)
        {
            cerr << "unable to open file or the file is empty." << endl;
            exit(1);
        }
        cout << "\nORIGINAL TEXT:" << endl;
        my_print(words);

        words = clean_text(words);
        unsorted_words = words;
        sort(begin(words), end(words));

        m = from_vector_to_map(words, m);

        if ((argv[2] == string("-a") || argv[2] == string("-f")) && argc == 3)
        {
            order_and_print(m, string(argv[2]), -1, words);
        }
        else if (argv[2] == string("-o") && argc == 4)
        {
            int num_words = stoi(argv[3]);
            if (num_words > 0)
            {
                order_and_print(m, string(argv[2]), num_words, unsorted_words);
            }
            else
            {
                cerr << "Error: N must be a positive number." << endl;
                cerr << "Usage: a.out FILE [-a] [-f] [-o N]" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            cerr << "Error: Second argument missing or invalid." << endl;
            cerr << "Usage: a.out FILE [-a] [-f] [-o N]" << endl;
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
