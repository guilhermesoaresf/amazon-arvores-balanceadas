#include "arvores.hpp"
#include <memory>
int main() {
    std::cout << "Sistema de encomendas - Arvores balanceadas\n1 - AVL\n2 - Rubro-Negra\nEscolha: ";
    int tipo; if (!(std::cin >> tipo) || (tipo!=1 && tipo!=2)) { std::cout << "Opcao invalida.\n"; return 1; }
    std::unique_ptr<Arvore> a;
    if(tipo==1) a=std::make_unique<AVL>(); else a=std::make_unique<RubroNegra>();
    while(true) {
        std::cout << "\n1 Inserir | 2 Buscar | 3 Remover | 4 Em ordem | 5 Visualizar | 6 Estatisticas | 0 Sair\nOpcao: ";
        int op; if(!(std::cin>>op) || op==0) break;
        if(op>=1 && op<=3) {
            int id; std::cout << "Identificador inteiro: "; if(!(std::cin>>id)){ std::cout << "Entrada invalida.\n"; break; }
            if(op==1) std::cout << (a->inserir(id)?"Inserido.\n":"Identificador repetido; nao inserido.\n");
            if(op==2) std::cout << (a->buscar(id)?"Encontrado.\n":"Nao encontrado.\n");
            if(op==3) std::cout << (a->remover(id)?"Removido.\n":"Nao encontrado; nada removido.\n");
        } else if(op==4) { auto valores=a->emOrdem(); if(valores.empty()) std::cout << "Arvore vazia"; for(int v:valores) std::cout << v << ' '; std::cout << '\n'; }
        else if(op==5) { std::cout << "Arvore de lado (raiz a esquerda; direita acima):\n"; a->imprimir(); }
        else if(op==6) std::cout << "Altura: " << a->altura() << " | Rotacoes: " << a->rotacoes << " | Propriedades: " << (a->validar()?"OK":"FALHA") << '\n';
        else std::cout << "Opcao invalida.\n";
    }
}
