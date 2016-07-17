#include <stdexcept>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions/factorials.hpp>

using namespace std;
using BigIntType = boost::multiprecision::cpp_int;

struct Key
{
    std::vector<std::pair<int, int> > columns;

    Key(size_t n)
    {
        if (n & 1)
        {
            throw std::runtime_error("n expected to be an even number");
        }
        for (auto i = 0; i < n; ++i)
        {
            columns.push_back(std::make_pair(n/2, n/2));
        }
    }

    bool isValid() const
    {
        bool ret = true;
        for (const auto &elm : columns)
        {
            if (elm.first < 0 || elm.second < 0)
            {
                ret = false;
                break;
            }
        }

        return ret;
    }
    
    bool isAllZero() const
    {
        bool ret = true;
        for (const auto &elm : columns)
        {
            if (elm.first != 0 || elm.second != 0)
            {
                ret = false;
                break;
            }
        }

        return ret;
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

BigIntType getBalancedArrayNum(const size_t n)
{
    Key key(n);
    return iteratorForK(key, n);
}

void test()
{
    assert(getBalancedArrayNum(0) == 1);
    cout << "Test passed for 0" << endl;
    
    assert(getBalancedArrayNum(2) == 2);
    cout << "Test passed for 2" << endl;

    assert(getBalancedArrayNum(4) == 90);
    cout << "Test passed for 4" << endl;
    
    assert(getBalancedArrayNum(6) == 297200);
    cout << "Test passed for 6" << endl;

    assert(getBalancedArrayNum(8) == BigIntType("116963796250"));
    cout << "Test passed for 8" << endl;
}

int main(int argc, char *argv[])
{
    test();

    return 0;
}

