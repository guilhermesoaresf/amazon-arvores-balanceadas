# Árvores balanceadas para encomendas

Trabalho da Unidade II de Estruturas de Dados Básicas II. Autor: Guilherme Soares.

O programa simula o cadastro de identificadores inteiros de encomendas usando uma árvore AVL ou Rubro-Negra escolhida na execução. As árvores e seus algoritmos de balanceamento foram implementados no projeto, sem biblioteca pronta para árvores balanceadas.

## Compilação e execução

É necessário um compilador com suporte a C++17 e o `make`.

```sh
make
./encomendas
make test
```

Em outro sistema, também é possível compilar diretamente: `g++ -std=c++17 -Wall -Wextra -pedantic -O2 src/main.cpp -o encomendas`. O executável incluído foi compilado para o ambiente do autor; em outra plataforma, compile novamente.

Escolha 1 para AVL ou 2 para Rubro-Negra, e depois use o menu. Exemplo: insira 30, 20 e 10 e selecione visualizar para observar a rotação na AVL. A impressão mostra a árvore deitada: a raiz fica à esquerda, o filho direito acima e o esquerdo abaixo. Na Rubro-Negra, `[P]` indica preto e `[V]` vermelho.

Os identificadores são inteiros únicos. Inserir um valor repetido não altera a árvore; buscar ou remover um valor ausente apresenta uma mensagem. A entrada encerra se for informado um valor que não seja inteiro. Os dados existem somente durante a execução.

## Como funciona

A AVL guarda a altura em cada nó. Após inserção ou remoção, atualiza alturas e aplica rotações quando a diferença entre as alturas dos filhos ultrapassa 1. A Rubro-Negra usa cores e um nó sentinela preto para representar folhas vazias. Inserções e remoções são seguidas por mudanças de cor e rotações para manter suas propriedades.

A opção de estatísticas informa altura, rotações acumuladas e validação. A altura conta nós: árvore vazia tem altura 0 e uma árvore com apenas a raiz tem altura 1. As rotações são contadas individualmente, inclusive as duas de uma rotação dupla. O arquivo de resultados também registra ajustes acumulados: na AVL conta cada nó rebalanceado, enquanto na Rubro-Negra conta mudanças efetivas de cor, inclusive ao substituir um nó. **Essas duas contagens de ajustes não medem a mesma coisa e não devem ser comparadas diretamente.**

## Testes e comparação

`make test` verifica ordem e propriedades após operações: ordem de busca, alturas e fator de balanceamento na AVL; raiz e sentinela pretas, ausência de filhos vermelhos de nós vermelhos, mesma altura negra e ponteiros de pais na Rubro-Negra. Inclui árvores vazias e unitárias, duplicados, buscas e remoções inexistentes, remoção de folhas e nós com um ou dois filhos e 20.000 operações pseudaleatórias por árvore comparadas com `std::set` apenas como referência de teste.

Os quatro casos de rotação da AVL e as remoções isoladas de folha, nó com um filho e nó com dois filhos também possuem testes próprios.

O experimento usa 1.000 identificadores em ordem crescente, decrescente e embaralhada (gerador `mt19937` com semente 42). Em cada cenário, insere todos e remove os primeiros 500 da mesma sequência. Os totais no CSV são acumulados desde o início. Execute `./testes > resultados/comparacao.csv` após `make testes` para reproduzir. O embaralhamento pode variar entre implementações da biblioteca padrão, apesar da mesma semente; os dados registrados correspondem ao ambiente usado na execução.

Nos resultados registrados, após as inserções ordenadas, as alturas foram 10 na AVL e 17 na Rubro-Negra. Na sequência embaralhada, ambas ficaram com altura 12; a AVL fez 662 rotações e a Rubro-Negra, 564. Após as remoções, os valores aparecem em `resultados/comparacao.csv`.

Para este protótipo, se a prioridade for limitar a altura para buscas, a AVL é uma escolha razoável: nos dados ordenados ela ficou mais baixa, e nas sequências embaralhadas empatou em altura. A Rubro-Negra é uma alternativa quando o custo de rotações importa: fez menos rotações em todos os cenários medidos. Isso não comprova que ela seja mais rápida: não medimos tempo, acesso à memória ou uma carga real da Amazon. As duas oferecem busca, inserção e remoção em O(log n) quando mantidas suas propriedades.

## Referências

- Enunciado da atividade, IMD0039, Trabalho da Unidade II.
- Cormen, Leiserson, Rivest e Stein. *Introduction to Algorithms*, 3ª ed., capítulos sobre árvores de busca e árvores Rubro-Negras.
- Open Data Structures, capítulo sobre árvores de busca balanceadas: https://opendatastructures.org/

## Apresentação

A versão revisada da apresentação está em `output/apresentacao-revisada.pdf`, com o arquivo editável em `output/apresentacao-revisada.pptx`.
