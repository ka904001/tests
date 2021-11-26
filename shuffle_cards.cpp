#include <vector>
#include <stack>
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    random_device rd;
    mt19937 gen(rd());
    vector<int> cards{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8};
    vector<int> discarded;
    discarded.reserve(cards.size());
    uniform_int_distribution<> distrib(0, cards.size() - 1);
    uniform_int_distribution<> stuck(0, 8);

    map<int, int> stats;

    int to_be_sorted = 100000;
    int num_sorted = 0;
    int not_sorted = 0;

    for (size_t k = 0; k < to_be_sorted; k++)
    {
        //randomize
        for (size_t i = 0; i < 1000; i++)
        {
            swap(cards[distrib(gen)], cards[distrib(gen)]);
        }
        bool done = false;
        int num_shuffles = 0;
        while (!done)
        {
            // for (auto &card : cards)
            // {
            //     cout << card << ' ';
            // }
            // cout << '\n';
            int counter = 0;
            while (!cards.empty())
            {
                bool removed = false;
                for (auto it = cards.begin(); it < cards.end();)
                {
                    if (counter == *it)
                    {
                        discarded.push_back(*it);
                        it = cards.erase(it);
                        removed = true;
                    }
                    else
                    {
                        it++;
                    }
                    counter++;
                    counter %= 9;
                }
                if (!removed)
                {
                    counter = stuck(gen);
                }
            }
            cards = move(discarded);
            discarded.reserve(cards.size());
            // cout << "cards before check\n";
            // for (auto &card : cards)
            // {
            //     cout << card << ' ';
            // }
            // cout << '\n';
            int val = 0;
            done = true;
            for (auto &card : cards)
            {
                if (card != val)
                {
                    done = false;
                    break;
                }
                val++;
                val %= 9;
            }
            num_shuffles++;
            if (done)
            {
                stats[num_shuffles]++;
                // cout << "done: " << num_shuffles << '\n';
                // cout << '+';
                num_sorted++;
            }
            if (num_shuffles > 30)
            {
                // cout << '*';
                not_sorted++;
                break;
            }
        }
    }
    cout << "\ntested_num: " << to_be_sorted << '\n';
    cout << "sorted    : " << num_sorted << '\n';
    cout << "not_sorted: " << not_sorted << '\n';

    string name;
    name = to_string(to_be_sorted) + '_' + to_string(num_sorted) + '_' + to_string(not_sorted) + ".dat";
    ofstream output(name.data());
    for (auto &p : stats)
    {
        output << p.first << ' ' << p.second << '\n';
    }
    return 0;
}
