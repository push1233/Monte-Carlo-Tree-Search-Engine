#ifndef MCTS_H
#define MCTS_H

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <vector>

extern uint64_t z[64][12];

void iz();
uint64_t ch(int b[64]);
uint64_t uh(uint64_t h, int p, int f, int t, int cp = -1);

struct XR {
    uint64_t s[2];
    inline uint64_t n() {
        uint64_t a = s[0];
        uint64_t b = s[1];
        uint64_t r = a + b;
        b ^= a;
        s[0] = ((a << 24) | (a >> 40)) ^ b ^ (b << 16);
        s[1] = (b << 37) | (b >> 27);
        return r;
    }
};

extern thread_local XR tx;

struct RB {
    int b[64];
    int p;
    int t;
};

extern thread_local RB tb;

template<size_t Z>
struct Ar {
    char* orig;
    char* b;
    size_t o;

    Ar() {
        orig = (char*)malloc(Z + 64);
        if (!orig) {
            b = nullptr;
            o = Z + 1; 
            return;
        }
        size_t p = (size_t)orig;
        b = orig + (64 - (p % 64));
        o = 0;
    }

    ~Ar() {
        free(orig);
    }

    void* al(size_t s) {
        size_t n = o + s;
        if (n > Z || !b) return nullptr;
        void* r = b + o;
        o = n;
        return r;
    }
};

extern Ar<1024 * 1024 * 64> ta[9];

struct alignas(64) Nd {
    std::atomic<int> v{0};
    std::atomic<double> s{0.0};
    std::atomic<int> vl{0};
    std::atomic<Nd*> fc{nullptr};
    std::atomic<Nd*> ns{nullptr};
    Nd* p = nullptr;
    int m = -1;
};

Nd* cn();
bool ex(Nd* p, int a, Nd* nc);
void avl(Nd* n);
void rvl(Nd* n);
void bp(Nd* n, double w);
Nd* dc(Nd* o, Ar<1024 * 1024 * 64>& a);
void cl(Ar<1024 * 1024 * 64>& a);

struct TT {
    std::atomic<uint64_t>* a;
    uint64_t m;

    TT(uint64_t s);
    ~TT();
    void w(uint64_t h, uint16_t v, uint16_t s);
    bool r(uint64_t h, uint16_t& v, uint16_t& s);
};
int ex_mcts(int state, int threads);
#endif