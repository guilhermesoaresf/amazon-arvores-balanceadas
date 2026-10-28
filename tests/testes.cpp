#include "../src/arvores.hpp"
#include <set>
#include <random>
#include <cassert>
#include <iostream>
#include <numeric>
#include <algorithm>
void conferir(Arvore& a, const std::set<int>& ref) { assert(a.validar()); assert(a.emOrdem()==std::vector<int>(ref.begin(),ref.end())); }
void casos(Arvore& a) {
    std::set<int> ref; assert(!a.buscar(1) && !a.remover(1)); conferir(a,ref);
    assert(a.inserir(10)); ref.insert(10); conferir(a,ref); assert(!a.inserir(10) && a.buscar(10));
    assert(a.remover(10)); ref.erase(10); conferir(a,ref);
    for(int x: {20,10,30,5,15,25,35,13,17,27}) { assert(a.inserir(x)); ref.insert(x); conferir(a,ref); }
    for(int x: {5,25,10,20,35,13,15,17,27,30}) { assert(a.remover(x)); ref.erase(x); conferir(a,ref); }
    assert(!a.remover(999));
}
void aleatorio(Arvore& a) {
    std::set<int> ref; std::mt19937 rng(2026);
    for(int i=0;i<20000;i++) {
        int v=static_cast<int>(rng()%401)-200, op=rng()%3;
        if(op==0) assert(a.inserir(v)==ref.insert(v).second);
        else if(op==1) assert(a.remover(v)==(ref.erase(v)!=0));
        else assert(a.buscar(v)==(ref.count(v)!=0));
        conferir(a,ref);
    }
}
void experimento(const char* nome, std::vector<int> dados) {
    AVL a; RubroNegra r;
    for(int v:dados){ assert(a.inserir(v) && r.inserir(v)); assert(a.validar() && r.validar()); }
    std::cout << nome << ",apos insercao,AVL," << a.altura() << ',' << a.rotacoes << ',' << a.ajustes << '\n';
    std::cout << nome << ",apos insercao,Rubro-Negra," << r.altura() << ',' << r.rotacoes << ',' << r.ajustes << '\n';
    for(int i=0;i<500;i++){ assert(a.remover(dados[i]) && r.remover(dados[i])); assert(a.validar() && r.validar()); }
    assert(a.emOrdem()==r.emOrdem());
    std::cout << nome << ",apos 500 remocoes,AVL," << a.altura() << ',' << a.rotacoes << ',' << a.ajustes << '\n';
    std::cout << nome << ",apos 500 remocoes,Rubro-Negra," << r.altura() << ',' << r.rotacoes << ',' << r.ajustes << '\n';
}
int main(){
    { AVL a; casos(a); } { RubroNegra r; casos(r); }
    { AVL a; aleatorio(a); } { RubroNegra r; aleatorio(r); }
    std::cout << "cenario,etapa,arvore,altura,rotacoes_acumuladas,ajustes_acumulados\n";
    std::vector<int> v(1000); std::iota(v.begin(),v.end(),1); experimento("crescente",v);
    std::reverse(v.begin(),v.end()); experimento("decrescente",v);
    std::mt19937 rng(42); std::shuffle(v.begin(),v.end(),rng); experimento("embaralhado",v);
    std::cerr << "Todos os testes passaram.\n";
}
