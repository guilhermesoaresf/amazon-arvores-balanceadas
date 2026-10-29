# Roteiro de estudo e defesa

Este roteiro ajuda a estudar a implementação e os resultados. Não substitui a apresentação em PDF. Execute o programa e os testes antes de apresentar.

## Problema e proposta

Uma árvore de busca comum pode ficar com altura proporcional ao número de nós quando recebe valores ordenados. Isso faz a busca, na pior situação, percorrer uma cadeia. A AVL e a Rubro-Negra impõem regras que limitam a altura, garantindo busca, inserção e remoção em O(log n). O protótipo usa IDs inteiros únicos; não simula todo o sistema logístico da Amazon.

## Como explicar a AVL

O fator de balanceamento de um nó é a altura da esquerda menos a da direita. Para todos os nós, esse valor deve estar entre -1 e 1. A implementação atualiza as alturas no caminho de volta após inserir ou remover. Se houver desequilíbrio, usa uma rotação simples ou uma combinação de duas rotações. Por exemplo, 30, 20, 10 causa uma rotação à direita em 30, que deixa 20 na raiz. As rotações preservam a ordem de busca.

As quatro configurações são esquerda-esquerda, direita-direita, esquerda-direita e direita-esquerda. As duas primeiras usam uma rotação; as duas últimas usam duas. O teste `rotacoesAVL()` verifica as quatro.

## Como explicar a Rubro-Negra

As regras exigem raiz e folhas sentinelas pretas, impedem que um nó vermelho tenha filho vermelho e exigem a mesma quantidade de nós pretos nos caminhos de um nó às folhas. A implementação usa uma sentinela compartilhada `nil` para representar filhos vazios. Quando uma inserção cria um pai vermelho com filho vermelho, o algoritmo examina o tio: se ele for vermelho, pode recolorir; se for preto, pode recolorir e rotacionar. A remoção de um nó preto pode exigir correção adicional com base no irmão e nos filhos dele.

## Como interpretar os testes

As duas árvores receberam as mesmas entradas nos experimentos. Cada cenário insere 1.000 IDs, mede, remove os primeiros 500 dessa mesma sequência e mede novamente. Altura é contada em nós, e rotações são acumuladas. A sequência embaralhada usa semente 42; a implementação da biblioteca padrão pode gerar uma permutação diferente em outro ambiente.

Na entrada crescente, após a inserção, a AVL teve altura 10 e a Rubro-Negra 17. Depois das remoções, ficaram com 10 e 15. Na sequência embaralhada, as alturas empataram nas duas medições. A Rubro-Negra teve menos rotações acumuladas em todos os cenários, mas durante as remoções isoladas fez 249 contra 248 nos casos ordenados e 226 contra 212 no embaralhado.

O contador de ajustes tem significados diferentes: nós rebalanceados na AVL e mudanças de cor na Rubro-Negra. Não é correto comparar os números como se fossem o mesmo custo. Também não medimos tempo, memória ou tráfego real. O resultado serve para justificar uma escolha para os cenários estudados, e não para afirmar que uma estrutura sempre é mais rápida.

## Perguntas que podem aparecer

**Por que a AVL foi recomendada?** A prioridade assumida foi limitar a altura e, portanto, o maior caminho de busca. Nos dados ordenados ela foi mais baixa e, nos embaralhados, empatou. Se o padrão de operações ou outras prioridades mudarem, a recomendação precisa ser reavaliada.

**O que acontece com IDs repetidos?** A inserção é recusada e a árvore permanece igual. Busca ou remoção de um ID ausente retorna uma mensagem.

**O que acontece ao remover um nó com dois filhos?** O código encontra o sucessor em ordem (o menor valor da subárvore direita), reorganiza a árvore e depois recupera as propriedades de balanceamento.

**A árvore Rubro-Negra tem altura igual à AVL?** Não necessariamente. Ela permite uma diferença maior entre caminhos, mas suas regras ainda garantem altura O(log n).

**O `std::set` substitui alguma das árvores?** Não. Ele aparece apenas nos testes como referência para verificar se os valores e o percurso ordenado estão corretos. AVL, Rubro-Negra, rotações e ajustes foram implementados no projeto.

**Quais são as limitações?** O programa guarda dados apenas em memória, usa IDs inteiros e não foi testado sob carga real ou concorrência. Os experimentos não medem tempo. O executável incluído é para o ambiente do autor; outros ambientes precisam compilar o código.
