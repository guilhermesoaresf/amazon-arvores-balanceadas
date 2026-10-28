#ifndef ARVORES_HPP
#define ARVORES_HPP
#include <algorithm>
#include <iostream>
#include <climits>
#include <string>
#include <cstdlib>
#include <vector>
#include <limits>

class Arvore {
public:
    long rotacoes = 0, ajustes = 0;
    virtual ~Arvore() = default;
    virtual bool inserir(int) = 0;
    virtual bool remover(int) = 0;
    virtual bool buscar(int) const = 0;
    virtual std::vector<int> emOrdem() const = 0;
    virtual int altura() const = 0; // altura em numero de nos; arvore vazia = 0
    virtual void imprimir() const = 0;
    virtual bool validar() const = 0;
};

class AVL : public Arvore {
    struct No { int valor, altura = 1; No *esq = nullptr, *dir = nullptr; explicit No(int v): valor(v) {} };
    No* raiz = nullptr;
    int h(No* n) const { return n ? n->altura : 0; }
    void atualizar(No* n) { n->altura = 1 + std::max(h(n->esq), h(n->dir)); }
    No* direita(No* y) { No* x=y->esq; y->esq=x->dir; x->dir=y; atualizar(y); atualizar(x); ++rotacoes; return x; }
    No* esquerda(No* x) { No* y=x->dir; x->dir=y->esq; y->esq=x; atualizar(x); atualizar(y); ++rotacoes; return y; }
    No* balancear(No* n) {
        if (!n) return n;
        atualizar(n); int f=h(n->esq)-h(n->dir);
        if (f>1) { ++ajustes; if (h(n->esq->esq)<h(n->esq->dir)) n->esq=esquerda(n->esq); return direita(n); }
        if (f< -1) { ++ajustes; if (h(n->dir->dir)<h(n->dir->esq)) n->dir=direita(n->dir); return esquerda(n); }
        return n;
    }
    No* inserir(No* n, int v, bool& ok) {
        if (!n) { ok=true; return new No(v); }
        if (v<n->valor) n->esq=inserir(n->esq,v,ok);
        else if (v>n->valor) n->dir=inserir(n->dir,v,ok);
        else return n;
        return balancear(n);
    }
    No* remover(No* n, int v, bool& ok) {
        if (!n) return nullptr;
        if (v<n->valor) n->esq=remover(n->esq,v,ok);
        else if (v>n->valor) n->dir=remover(n->dir,v,ok);
        else {
            ok=true;
            if (!n->esq || !n->dir) { No* filho=n->esq ? n->esq : n->dir; delete n; return filho; }
            No* s=n->dir; while (s->esq) s=s->esq;
            n->valor=s->valor; bool ignorado=false; n->dir=remover(n->dir,s->valor,ignorado);
        }
        return balancear(n);
    }
    void ordem(No* n, std::vector<int>& v) const { if(n){ ordem(n->esq,v); v.push_back(n->valor); ordem(n->dir,v); } }
    void mostrar(No* n, int nivel) const { if(n){ mostrar(n->dir,nivel+1); std::cout << std::string(nivel*4,' ') << n->valor << " (h=" << n->altura << ")\n"; mostrar(n->esq,nivel+1); } }
    void limpar(No* n) { if(n){ limpar(n->esq); limpar(n->dir); delete n; } }
    int verificar(No* n, long long min, long long max) const {
        if (!n) return 0;
        if(n->valor<=min || n->valor>=max) return -1;
        int e=verificar(n->esq,min,n->valor), d=verificar(n->dir,n->valor,max);
        if(e<0 || d<0 || std::abs(e-d)>1 || n->altura!=1+std::max(e,d)) return -1;
        return 1+std::max(e,d);
    }
public:
    ~AVL() override { limpar(raiz); }
    bool inserir(int v) override { bool ok=false; raiz=inserir(raiz,v,ok); return ok; }
    bool remover(int v) override { bool ok=false; raiz=remover(raiz,v,ok); return ok; }
    bool buscar(int v) const override { No* n=raiz; while(n){ if(v==n->valor) return true; n=v<n->valor?n->esq:n->dir; } return false; }
    std::vector<int> emOrdem() const override { std::vector<int> v; ordem(raiz,v); return v; }
    int altura() const override { return h(raiz); }
    void imprimir() const override { if(!raiz) std::cout << "(arvore vazia)\n"; else mostrar(raiz,0); }
    bool validar() const override { return verificar(raiz,LLONG_MIN,LLONG_MAX)>=0; }
};

class RubroNegra : public Arvore {
    enum Cor { VERMELHO, PRETO };
    struct No { int valor=0; Cor cor=PRETO; No *pai=nullptr, *esq=nullptr, *dir=nullptr; };
    No *nil, *raiz;
    void esquerda(No* x) { No* y=x->dir; x->dir=y->esq; if(y->esq!=nil) y->esq->pai=x; y->pai=x->pai; if(x->pai==nil) raiz=y; else if(x==x->pai->esq) x->pai->esq=y; else x->pai->dir=y; y->esq=x; x->pai=y; ++rotacoes; }
    void direita(No* y) { No* x=y->esq; y->esq=x->dir; if(x->dir!=nil) x->dir->pai=y; x->pai=y->pai; if(y->pai==nil) raiz=x; else if(y==y->pai->dir) y->pai->dir=x; else y->pai->esq=x; x->dir=y; y->pai=x; ++rotacoes; }
    void cor(No* n, Cor c) { if(n!=nil && n->cor!=c){ n->cor=c; ++ajustes; } }
    void corrigirInsercao(No* z) {
        while(z->pai->cor==VERMELHO) {
            if(z->pai==z->pai->pai->esq) {
                No* t=z->pai->pai->dir;
                if(t->cor==VERMELHO){ cor(z->pai,PRETO); cor(t,PRETO); cor(z->pai->pai,VERMELHO); z=z->pai->pai; }
                else { if(z==z->pai->dir){ z=z->pai; esquerda(z); } cor(z->pai,PRETO); cor(z->pai->pai,VERMELHO); direita(z->pai->pai); }
            } else {
                No* t=z->pai->pai->esq;
                if(t->cor==VERMELHO){ cor(z->pai,PRETO); cor(t,PRETO); cor(z->pai->pai,VERMELHO); z=z->pai->pai; }
                else { if(z==z->pai->esq){ z=z->pai; direita(z); } cor(z->pai,PRETO); cor(z->pai->pai,VERMELHO); esquerda(z->pai->pai); }
            }
        }
        cor(raiz,PRETO);
    }
    void trocar(No* u, No* v) { if(u->pai==nil) raiz=v; else if(u==u->pai->esq) u->pai->esq=v; else u->pai->dir=v; v->pai=u->pai; }
    void corrigirRemocao(No* x) {
        while(x!=raiz && x->cor==PRETO) {
            if(x==x->pai->esq) {
                No* w=x->pai->dir;
                if(w->cor==VERMELHO){ cor(w,PRETO); cor(x->pai,VERMELHO); esquerda(x->pai); w=x->pai->dir; }
                if(w->esq->cor==PRETO && w->dir->cor==PRETO){ cor(w,VERMELHO); x=x->pai; }
                else { if(w->dir->cor==PRETO){ cor(w->esq,PRETO); cor(w,VERMELHO); direita(w); w=x->pai->dir; } cor(w,x->pai->cor); cor(x->pai,PRETO); cor(w->dir,PRETO); esquerda(x->pai); x=raiz; }
            } else {
                No* w=x->pai->esq;
                if(w->cor==VERMELHO){ cor(w,PRETO); cor(x->pai,VERMELHO); direita(x->pai); w=x->pai->esq; }
                if(w->dir->cor==PRETO && w->esq->cor==PRETO){ cor(w,VERMELHO); x=x->pai; }
                else { if(w->esq->cor==PRETO){ cor(w->dir,PRETO); cor(w,VERMELHO); esquerda(w); w=x->pai->esq; } cor(w,x->pai->cor); cor(x->pai,PRETO); cor(w->esq,PRETO); direita(x->pai); x=raiz; }
            }
        }
        cor(x,PRETO);
    }
    No* procurar(int v) const { No* n=raiz; while(n!=nil && n->valor!=v) n=v<n->valor?n->esq:n->dir; return n; }
    void ordem(No* n,std::vector<int>& v) const { if(n!=nil){ ordem(n->esq,v); v.push_back(n->valor); ordem(n->dir,v); } }
    int altura(No* n) const { return n==nil ? 0 : 1+std::max(altura(n->esq),altura(n->dir)); }
    void mostrar(No* n,int nivel) const { if(n!=nil){ mostrar(n->dir,nivel+1); std::cout << std::string(nivel*4,' ') << n->valor << (n->cor==PRETO?" [P]\n":" [V]\n"); mostrar(n->esq,nivel+1); } }
    void limpar(No* n) { if(n!=nil){ limpar(n->esq); limpar(n->dir); delete n; } }
    int verificar(No* n,long long min,long long max) const {
        if(n==nil) return 1;
        if(n->valor<=min || n->valor>=max || (n->esq!=nil && n->esq->pai!=n) || (n->dir!=nil && n->dir->pai!=n)) return -1;
        if(n->cor==VERMELHO && (n->esq->cor==VERMELHO || n->dir->cor==VERMELHO)) return -1;
        int e=verificar(n->esq,min,n->valor), d=verificar(n->dir,n->valor,max);
        if(e<0 || d<0 || e!=d) return -1;
        return e+(n->cor==PRETO);
    }
public:
    RubroNegra() { nil=new No; nil->pai=nil->esq=nil->dir=nil; raiz=nil; }
    ~RubroNegra() override { limpar(raiz); delete nil; }
    bool inserir(int v) override {
        No *p=nil,*n=raiz; while(n!=nil){ p=n; if(v==n->valor) return false; n=v<n->valor?n->esq:n->dir; }
        No* z=new No; z->valor=v; z->cor=VERMELHO; z->pai=p; z->esq=z->dir=nil;
        if(p==nil) raiz=z; else if(v<p->valor) p->esq=z; else p->dir=z;
        corrigirInsercao(z); return true;
    }
    bool remover(int v) override {
        No* z=procurar(v); if(z==nil) return false;
        No *y=z,*x; Cor original=y->cor;
        if(z->esq==nil){ x=z->dir; trocar(z,z->dir); }
        else if(z->dir==nil){ x=z->esq; trocar(z,z->esq); }
        else { y=z->dir; while(y->esq!=nil) y=y->esq; original=y->cor; x=y->dir;
            if(y->pai==z) x->pai=y; else { trocar(y,y->dir); y->dir=z->dir; y->dir->pai=y; }
            trocar(z,y); y->esq=z->esq; y->esq->pai=y; cor(y,z->cor);
        }
        delete z; if(original==PRETO) corrigirRemocao(x); return true;
    }
    bool buscar(int v) const override { return procurar(v)!=nil; }
    std::vector<int> emOrdem() const override { std::vector<int> v; ordem(raiz,v); return v; }
    int altura() const override { return altura(raiz); }
    void imprimir() const override { if(raiz==nil) std::cout << "(arvore vazia)\n"; else mostrar(raiz,0); }
    bool validar() const override { return nil->cor==PRETO && (raiz==nil || (raiz->cor==PRETO && raiz->pai==nil)) && verificar(raiz,LLONG_MIN,LLONG_MAX)>0; }
};
#endif
