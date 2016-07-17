#include <stdexcept>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using BigIntType = boost::multiprecision::cpp_int;

struct Key
{
    std::vector<std::pair<int, int> > columns;

    Key(size_t n)
    {
        for (auto i = 0; i < n; ++i)
        {
            columns.push_back(std::make_pair(n/2, n/2));
        }
    }
    Key(const Key &) = default;
    Key(Key&&) = default;

    bool isValid() const
    {
        return std::all_of(columns.begin(), columns.end(),
                           [](const auto &e) { return e.first >= 0 && e.second >= 0;});
    }
    
    bool isAllZero() const
    {
        return std::all_of(columns.begin(), columns.end(),
                           [](const auto &e) { return e.first == 0 && e.second == 0;});
    }

    Key minusRow(const std::vector<int> &row) const
    {
        Key ret = *this;
        
        for (auto i = 0; i < columns.size(); ++i)
        {
            if (row[i] == 0)
            {
                --ret.columns[i].first;
            }
            else
            {
                --ret.columns[i].second;
            }
        }
        return ret;
    }
};

bool operator<(const Key &lhs, const Key &rhs)
{
    if (lhs.columns.size() < rhs.columns.size())
    {
        return true;
    }
    if (lhs.columns.size() > rhs.columns.size())
    {
        return false;
    }
    for (auto i = 0; i < lhs.columns.size(); ++i)
    {
        if (lhs.columns[i] < rhs.columns[i])
        {
            return true;
        }
        if (lhs.columns[i] > rhs.columns[i])
        {
            return false;
        }
    }
    return false;
}

struct Permutations
{
    Permutations(size_t n)
    {
        for (int i = 0; i < n/2; ++i)
        {
            row_.push_back(0);
        }
        for (int i = 0; i < n/2; ++i)
        {
            row_.push_back(1);
        }
    }

    bool next()
    {
        return next_permutation(row_.begin(), row_.end());
    }

    std::vector<int> row_;
};

class Balanced_0_1_Array_NumCalc
{
public:
    Balanced_0_1_Array_NumCalc(const size_t n) : n_(n)
    {
        if (n & 1)
        {
            throw std::runtime_error("N must be an even number");
        }
    }
    
    BigIntType calc()
    {
        Key key(n_);
        return iteratorForK(key, n_);
    }
private:
    size_t n_;
    std::map<Key, BigIntType> map_;

    BigIntType iteratorForK(const Key &key, size_t k)
    {
        BigIntType ret = 0;
        if (key.isValid())
        {
            if (k == 0)
            {
                if (key.isAllZero())
                {
                    ret = 1;
                }
            }
            else
            {
                Permutations permutations(key.columns.size());

                do
                {
                    const Key newKey = key.minusRow(permutations.row_);
                    auto location = map_.find(newKey);
                    if (location != map_.end())
                    {

                        ret += location->second;
                    }
                    else
                    {
                        auto val = iteratorForK(newKey, k - 1);
                        ret += val;
                    }
                } while (permutations.next());
            }
        }

        map_[key] = ret;
        return ret;
    }

};

void test()
{
    assert(Balanced_0_1_Array_NumCalc(0).calc() == 1);
    cout << "Test passed for 0" << endl;
    
    assert(Balanced_0_1_Array_NumCalc(2).calc() == 2);
    cout << "Test passed for 2" << endl;

    assert(Balanced_0_1_Array_NumCalc(4).calc() == 90);
    cout << "Test passed for 4" << endl;
    
    assert(Balanced_0_1_Array_NumCalc(6).calc() == 297200);
    cout << "Test passed for 6" << endl;

    assert(Balanced_0_1_Array_NumCalc(8).calc() == BigIntType("116963796250"));
    cout << "Test passed for 8" << endl;
}

int main(int argc, char *argv[])
{
    test();

    return 0;
}

