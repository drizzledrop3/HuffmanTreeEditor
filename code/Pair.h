#ifndef PAIR_H
#define PAIR_H
template<typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;
public:
    Pair() {}

    Pair(T1 First, T2 Second) {
        first = First;
        second = Second;
    }

    T1 First() { return first; }

    T2 Second() { return second; }

    void SecondUp(int up) { second += up; }
};
#endif // PAIR_H
