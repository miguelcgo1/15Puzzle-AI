
# Buscas para o 15-puzzle game
Trabalho desenvolvido para a cadeira de Inteligência Artifical (CC2006) da Faculdade de Ciências da Universidade do Porto.

O código implementado foi escrito em C++, compilado e testado em Ubuntu 22.04 LTS.
## Algoritmos Implementados
Para este trabalho foram implementados os seguintes algorimtos de busca:

 - BFS (Breadth First Search)
 - DFS (Depth First Search)
 - IDFS (Iterative Deepening Depth First Search)
 - A*-misplaced (A Star Search com heurística Misplaced Tiles)
 - A*-Manhattan (A Star Search com heurística Misplaced Tiles)
 - Greedy-misplaced (Greedy Search com heurística Misplaced Tiles)
 - Greedy-Manhattan (Greedy Search com heurística Manhattan Distance)

## Compilação
Para compilar o programa basta correr o comando no terminal: `$ make`

## Execução
Para executar o programa é necessário utilizar o seguinte comando: `$ ./main <search_algorithm> <input_file>`, onde *<search_algorithm>* é o nome abreviado do algoritmo que quer testar conforme escrito na secção *Algoritmos Implementados*.

Exemplo de execução: `$ ./main A*misplaced ConfigSol12.txt`

## Ficheiro de input
Para o argumento **<input_file>** este deve ter duas configurações em cada linha. A primeira deve corresponder à configuração inicial e a da segunda linha deve ser a configuração final.

Por exemplo, *ConfigSol12.txt*:

    1 2 3 4 5 6 8 12 13 9 0 7 14 11 10 15
    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0

## Resultados

 1. *ConfigSol12.txt*

| Algortimo           |BFS	        |DFS		      |IDFS |A*-MisplacedTiles	|A*-Manhattan |Greedy-Manhattan 	| Greedy-MisplacedTiles	|
|---------------------|-------------|-------------|-----|-------------------|-------------|------------|------|
|Path          	 			|DLLURRRULDDR	|URDLURDLDLLURRDR							|DLLURRRULDDR			|DLLURRRULDDR				|DLLURRRULDDR	|DLLURRRULDDR	| RULDDLLURRDR |
|Steps          			|12						|16							|12			|12									|12						|12	|12
|Nº of Expanded Nodes	|17889				|244175							|486578			|68									|26						|15	|17
|Memory Used					|35728				|244182							|486588			|140								|52						|32	|39
|Time									|0.053206s		|0.445438s							|0.501213s			|0.000260s					|0.000137s		|0.000079s	| 0.000089s

 2. *ConfigSol16.txt*
 
| Algoritmo					|BFS|DFS|IDFS|A*-MisplacedTiles|A*-Manhattan|Greedy-Manhattan|Greedy-MisplacedTiles|
|-------------------|--|--|--|--|--|--|--|
|Path 							|RRULDRRDLDLLURRU|RRDDLULDRURUULDDLDRUURRDLU | |RRULDRRDLDLLURRU|RRRDLDLLURRUULDR| |RRRDLUULDRDDLLURRU
|Steps							|16|26 | |16|16| |18			
|Nº Expanded Nodes	|279222|1595317 | |110|212106| |24
|Memory Used				|545378|1595336 | |249|406765| |57
|Time								|1.059529s|3.147430s | |0.000451s|1.040513s|	|0.000118s	 

 3. *ConfigSol7.txt*

|Algoritmo|BFS|DFS|IDFS|A*-MisplacedTiles|A*-Manhattan|Greedy-Manhattan|Greedy-MisplacedTiles|
|--|--|--|--|--|--|--|--|
|Path|UURDRDR|UUURRDDDRULUULLDRDRRD|UURDRDR|UURDRDR|UURDRDR|UURDRDR|UURDRDR|
|Steps|7|21|7|7|7|7|7|
|Nº Expanded Nodes|231|64170|107|8|8|8|8
|Memory Used|474|64181|112|17|17|17|17|
|Time|0.000694s|0.100068s|0.000155s|0.000054s|0.000051s|0.000048s|0.000052s|


 4. *ConfigSol1.txt*

|Algoritmo|BFS|DFS|IDFS|A*-MisplacedTiles|A*-Manhattan|Greedy-Manhattan|Greedy-MisplacedTiles
|--|--|--|--|--|--|--|--|
|Path|R|R|R|R|R|R|R|
|Steps|1|117681|1|1|1|1|1|
|Nº Expanded Nodes|4|117680|4|2|2|2|2|
|Memory Used|8|369470|3|3|3|3|3|
|Time|0.000031s|0.204331s|0.000016s|0.000022s|0.000025s|0.000023s|0.000022s|

## Autores
José Torres
Miguel Gomes 
Rodrigo Castro
