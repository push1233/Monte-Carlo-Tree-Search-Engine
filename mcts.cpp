#include "mcts.h"
#include <new>
#include <random>
#include <thread>
#include <vector>
#include <cmath>
uint64_t z[64][12];
Ar<1024 * 1024 * 64> ta[9];
thread_local int t_id = 8;
thread_local XR tx = {{1337ULL, 7331ULL}};
thread_local RB tb;

void iz() {
    std::mt19937_64 r(1337);
    for(int i = 0; i < 64; ++i) {
        for(int j = 0; j < 12; ++j) {
            z[i][j] = r();
        }
    }
}

uint64_t ch(int b[64]) {
    uint64_t h = 0;
    for(int i = 0; i < 64; ++i) {
        if(b[i] != -1) h ^= z[i][b[i]];
    }
    return h;
}

uint64_t uh(uint64_t h, int p, int f, int t, int cp) {
    h ^= z[f][p];
    h ^= z[t][p];
    if(cp != -1) h ^= z[t][cp];
    return h;
}

Nd* cn() {
    void* m = ta[t_id].al(sizeof(Nd));
    if (!m) return nullptr;
    return new(m) Nd();
}

bool ex(Nd* p, int a, Nd* nc) {
    Nd* e = nullptr;
    if (p->fc.compare_exchange_strong(e, nc, std::memory_order_release, std::memory_order_relaxed)) return true;
    
    Nd* c = p->fc.load(std::memory_order_acquire);
    while (c) {
        if (c->m == a) return false;
        Nd* n = c->ns.load(std::memory_order_acquire);
        if (!n) {
            e = nullptr; 
            if (c->ns.compare_exchange_strong(e, nc, std::memory_order_release, std::memory_order_relaxed)) return true;
        }
        c = c->ns.load(std::memory_order_acquire);
    }
    return false;
}

void avl(Nd* n) {
    while(n) {
        n->vl.fetch_add(1, std::memory_order_relaxed);
        n = n->p;
    }
}

void rvl(Nd* n) {
    while(n) {
        n->vl.fetch_sub(1, std::memory_order_relaxed);
        n = n->p;
    }
}

void bp(Nd* n, double w) {
    while(n) {
        n->v.fetch_add(1, std::memory_order_relaxed);
        double e = n->s.load(std::memory_order_relaxed);
        while(!n->s.compare_exchange_weak(e, e + w, std::memory_order_relaxed, std::memory_order_relaxed));
        n = n->p;
    }
}

Nd* dc(Nd* o, Ar<1024 * 1024 * 64>& a) {
    if (!o) return nullptr;
    void* m = a.al(sizeof(Nd));
    if (!m) return nullptr;
    Nd* n = new(m) Nd();
    n->v.store(o->v.load(std::memory_order_relaxed), std::memory_order_relaxed);
    n->s.store(o->s.load(std::memory_order_relaxed), std::memory_order_relaxed);
    n->m = o->m;
    
    Nd* c = o->fc.load(std::memory_order_acquire);
    Nd* l = nullptr;
    while (c) {
        Nd* nc = dc(c, a);
        if (nc) {
            nc->p = n;
            if (!l) n->fc.store(nc, std::memory_order_release);
            else l->ns.store(nc, std::memory_order_release);
            l = nc;
        }
        c = c->ns.load(std::memory_order_acquire);
    }
    return n;
}

void cl(Ar<1024 * 1024 * 64>& a) {
    a.o = 0;
}

TT::TT(uint64_t s) {
    m = s - 1;
    a = new std::atomic<uint64_t>[s];
    for (uint64_t i = 0; i < s; ++i) a[i].store(0, std::memory_order_relaxed);
}

TT::~TT() {
    delete[] a;
}

void TT::w(uint64_t h, uint16_t v, uint16_t s) {
    uint64_t i = h & m;
    uint64_t g = h >> 32;
    uint64_t n = (g << 32) | ((uint64_t)v << 16) | s;
    
    uint64_t o = a[i].load(std::memory_order_relaxed);
    while (true) {
        if (o != 0) {
            uint16_t ov = (o >> 16) & 0xFFFF;
            if (v <= ov && (o >> 32) == g) break;
        }
        if (a[i].compare_exchange_weak(o, n, std::memory_order_relaxed, std::memory_order_relaxed)) break;
    }
}

bool TT::r(uint64_t h, uint16_t& v, uint16_t& s) {
    uint64_t i = h & m;
    uint64_t g = h >> 32;
    uint64_t e = a[i].load(std::memory_order_relaxed);
    
    if ((e >> 32) == g) {
        v = (e >> 16) & 0xFFFF;
        s = e & 0xFFFF;
        return true;
    }
    return false;
}

int ck(int b[9]) {
    int w[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i = 0; i < 8; ++i) {
        if(b[w[i][0]] != 0 && b[w[i][0]] == b[w[i][1]] && b[w[i][1]] == b[w[i][2]]) return b[w[i][0]];
    }
    return 0;
}

int ex_mcts(int s, int t) {
    int ob[9];
    int tp = s;
    int pcs = 0;
    for(int i = 8; i >= 0; --i) {
        ob[i] = tp % 10;
        if(ob[i] != 0) pcs++;
        tp /= 10;
    }
    int root_cp = (pcs % 2 == 0) ? 1 : 2; 
    
    Nd* rt = cn();
    if(!rt) return -1;
    
    auto wr = [&](int th) {
        t_id = th; 
        ta[th].o = 0;
        std::mt19937 r(1337 + th);
        for(int i = 0; i < 100000; ++i) {
            Nd* c = rt;
            int cb[9];
            for(int j = 0; j < 9; ++j) cb[j] = ob[j];
            int cp = root_cp; 
            
            while(true) {
                int res = ck(cb);
                if(res != 0) break;
                
                std::vector<int> lm;
                for(int j = 0; j < 9; ++j) if(cb[j] == 0) lm.push_back(j);
                if(lm.empty()) break;
                
                std::vector<int> ux;
                for(int m : lm) {
                    bool f = false;
                    Nd* ch = c->fc.load(std::memory_order_acquire);
                    while(ch) {
                        if(ch->m == m) { f = true; break; }
                        ch = ch->ns.load(std::memory_order_acquire);
                    }
                    if(!f) ux.push_back(m);
                }
                
                if(!ux.empty()) {
                    int nm = ux[r() % ux.size()];
                    Nd* nn = cn();
                    if(nn) {
                        nn->m = nm;
                        nn->p = c;
                        if(ex(c, nm, nn)) {
                            c = nn;
                            cb[nm] = cp;
                            cp = 3 - cp;
                            break; 
                        }
                    }
                } else {
                    Nd* bc = nullptr;
                    double mx = -1e9;
                    Nd* ch = c->fc.load(std::memory_order_acquire);
                    while(ch) {
                        double vi = ch->v.load(std::memory_order_relaxed);
                        double base = ch->s.load(std::memory_order_relaxed) / (vi + 1e-5);
                        double exp = 1.41 * std::sqrt(std::log(c->v.load(std::memory_order_relaxed) + 1) / (vi + 1e-5));
                        double u = (cp == 2) ? (base + exp) : ((1.0 - base) + exp);
                        if(u > mx) { mx = u; bc = ch; }
                        ch = ch->ns.load(std::memory_order_acquire);
                    }
                    c = bc;
                    cb[c->m] = cp;
                    cp = 3 - cp;
                }
            }
            
            int res = ck(cb);
            while(res == 0) {
                std::vector<int> rm;
                for(int j = 0; j < 9; ++j) if(cb[j] == 0) rm.push_back(j);
                if(rm.empty()) break;
                int nm = rm[r() % rm.size()];
                cb[nm] = cp;
                cp = 3 - cp;
                res = ck(cb);
            }
            
            double w = (res == 2) ? 1.0 : ((res == 0) ? 0.5 : 0.0);
            bp(c, w);
        }
    };
    
    std::vector<std::thread> ts;
    for(int i = 0; i < t; ++i) ts.emplace_back(wr, i);
    for(auto& th : ts) th.join();
    
    int bm = -1;
    double mv = -1;
    Nd* c = rt->fc.load(std::memory_order_acquire);
    while(c) {
        double cv = c->v.load(std::memory_order_relaxed);
        if(cv > mv) { mv = cv; bm = c->m; }
        c = c->ns.load(std::memory_order_acquire);
    }
    
    return bm;
}