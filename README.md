# Projetos da disciplina de Projeto e Análise de Algoritmos 2025.2

* Divisão e Conquista

merge sort: A empresa de automação portuária Poxim Tech está desenvolvendo um sistema para movimentação automatizada dos contêineres de carga de origem internacional no Porto de Sergipe para maximizar a eficiência da fiscalização aduaneira. Todos os contêineres possuem um cadastro eletrônico contendo informações sobre o código do contêiner, o CNPJ da empresa importadora e o peso líquido em quilos. A inspeção dos contêineres é realizada sempre que existe alguma divergência entre as informações cadastradas, como o CNPJ informado ou a diferença percentual maior do que 10% no peso líquido. Na triagem dos contêineres são fiscalizados os contêineres com a seguinte ordem de prioridade: 1. Divergência de CNPJ, 2. Maior diferença percentual de peso líquido.

Formato de arquivo de entrada:
[#n contêineres cadastrados]
[Código 1] [CNPJ 1] [Peso 1]
...
[Código n] [CNPJ n] [Peso n]
[#m contêineres selecionados]
[Código 1] [CNPJ 1] [Peso 1]
...
[Código m] [CNPJ m] [Peso m]

Exemplo de entrada:
6
QOZJ7913219 34.699.211/9365-11 13822
FCCU4584578 50.503.434/5731-28 16022
KTAJ0603546 20.500.522/6013-58 25279
ZYHU3978783 43.172.263/4442-14 24543
IKQZ7582839 51.743.446/1183-18 12160
HAAZ0273059 25.699.428/4746-79 16644
5
ZYHU3978783 43.172.263/4442-14 29765
IKQZ7582839 51.743.446/1113-18 18501
KTAJ0603546 20.500.522/6113-58 17842
QOZJ7913219 34.699.211/9365-11 16722
FCCU4584578 50.503.434/5731-28 16398

Formato de arquivo de saída:
A sequência de fiscalização dos contêineres do navio, com a causa da triagem e seguindo a ordem de cadastramento.

Exemplo de saída:
KTAJ0603546:20.500.522/6013-58<->20.500.522/6113-58
IKQZ7582839:51.743.446/1183-18<->51.743.446/1113-18
QOZJ7913219:2900kg(21%)
ZYHU3978783:5222kg(21%)

quick sort: A empresa de desenvolvimento de sistemas Poxim Tech está realizando um experimento para determinar qual variante do algoritmo de ordenação crescente do Quicksort apresenta o melhor resultado para um determinado conjunto de sequências numéricas. Neste experimento foram utilizadas as seguintes variantes: Lomuto padrão (LP), Lomuto por mediana de 3 (LM), Lomuto por pivô aleatório (LA), Hoare padrão (HP), Hoare por mediana de 3 (HM) e Hoare por pivô aleatório (HA). Técnicas de escolha do pivô: Mediana de 3: V1 = V[n/4], V2 = V[n/2], V3 = V[3n/4], Aleatório: Va = V[ini + |V[ini]| mod n].

Formato de arquivo de entrada:
[#n total de vetores]
[#N1 números do vetor 1]
[E1] ... [EN1]
...
[#Nn números do vetor n]
[E1] ... [ENn]

Exemplo de entrada:
4
6
-23 10 7 -34 432 3
4
955 -32 1 9
7
834 27 39 19 3 -1 -33
10
847 38 -183 -13 94 -2 -42 54 28 100

Formato de arquivo de saída:
Para cada vetor é impressa a quantidade total de números N e a sequência com ordenação estável contendo o número de trocas e de chamadas.

Exemplo de saída:
[6]:LP(15),HP(16),LM(19),HM(19),HA(20),LA(22)
[4]:LP(10),HP(10),LM(11),LA(11),HM(12),HA(12)
[7]:HP(17),LM(18),LP(23),HM(26),HA(27),LA(30)
[10]:LM(28),HP(28),LP(33),HA(35),HM(37),LA(38)

* Transformação e Conquista

Heap sort: A empresa de telecomunicações Poxim Tech está construindo um sistema de comunicação, baseada na priorização de pacotes em redes TCP/IP para implementação de qualidade de serviço (QoS). Os dados de cada pacote são sequências de bytes de tamanho variável, mas limitados até 512 bytes. Devido às características de roteamento de redes TCP/IP, os níveis de prioridade variam entre 0 (mínimo) e 63 (máximo), considerando que o roteador possui um limite máximo de bytes que consegue processar por vez. Sempre que a capacidade de processamento do roteador for atingida ou não for possível receber mais nenhum pacote, é feita a ordenação dos pacotes levando em consideração os seus respectivos níveis de priorização.

Formato de arquivo de entrada:
[Número de pacotes (n)] [Quantidade de bytes]
[Prioridade do pacote1] [#m1 Tamanho do pacote] [B1] ... [Bm1]
...
[Prioridade do pacoten] [#mn Tamanho do pacote] [B1] ... [Bmn]

Exemplo de entrada:
6 8
0 3 01 02 03
2 4 06 07 08 09
63 2 0F 10
15 6 11 12 13 14 15 16
32 5 0A 0B 0C 0D 0E
11 2 04 05

Formato de arquivo de saída:
Quando uma quantidade máxima de dados é recebida, é feita a ordenação dos pacotes armazenados para priorização do roteamento.

Exemplo de saída:
|06,07,08,09|01,02,03|
|0F,10|11,12,13,14,15,16|
|0A,0B,0C,0D,0E|04,05|

* Programação Dinâmica

A empresa de transportes Poxim Tech está tornando as entregas de encomendas de última milha mais eficientes e rápidas, através da maximização do valor total do frete e das características de cada tipo de veículo. Para codificação das placas de trânsito dos veículos e dos códigos de rastreamento dos pacotes são utilizados os símbolos L e N, que representam letras maiúsculas e números, respectivamente. As unidades utilizadas para peso é quilo (kg), valor é reais (R$) e volume é litro (l). Cada veículo é identificado pela sua placa de trânsito, nos formatos LLLNNNN ou LLLNLNN, além da sua capacidade máxima de carga (peso e volume). Os pacotes possuem um código de rastreamento descrito no padrão LLNNNNNNNNNLL, juntamente com informações sobre o valor, peso e volume.

Formato do arquivo de entrada:
[#n]
[Placa0] [Peso0] [Volume0]
...
[Placan–1] [Peson–1] [Volumen–1]
[#m]
[Código0] [Valor0] [Peso0] [Volume0]
...
[Códigom–1] [Valorm–1] [Pesom–1] [Volumem–1]

Exemplo de entrada:
2
AAA1234 50 100
BBB5C67 2000 12000
5
AB111222333CD 49.99 2 1
EF444555666GH 5000.01 1234 7000
IJ777888999KL 100 49 10
MN000111222OP 65.01 3 125
QR333444555ST 200.01 13 4875

Formato do arquivo de saída:
Para cada veículo é gerada uma sequência de carregamento dos pacotes que maximizam o valor transportado, sem exceder a capacidade de carga.

Exemplo de saída:
[AAA1234]R$100.00,49KG(98%),10L(10%)->IJ777888999KL
[BBB5C67]R$5265.03,1250KG(63%),12000L(100%)->EF444555666GH,MN000111222OP,QR333444555ST
PENDENTE:R$49.99,2KG,1L->AB111222333CD

* Busca em Cadeias

A empresa de biotecnologia Poxim Tech possui um sistema de diagnóstico para doenças genéticas, comparando a sequência de DNA com genes conhecidos. A sequência de DNA é composta somente pelos símbolos A, C, G e T para codificação dos genes. Uma doença genética pode possuir vários genes associados, com sequências de tamanho entre 100 até 1000 símbolos, denotada exclusivamente por letras maiúsculas e números com tamanho entre 4 e 8 caracteres. Para tratar os efeitos da mutação nos genes que alteram sua codificação, é feita a busca por combinações que possuam o tamanho mínimo de subcadeia, com pelo menos 90% de compatibilidade para manifestação da doença. No diagnóstico será calculada a probabilidade de manifestação da doença, de acordo com a quantidade de genes detectados na sequência de DNA. Diagnóstico da doença CTRLF4 com genes AATTTGGCCC e GGGGGGGGGG. DNA: AAAAAAAAAATTTTTTTTTTTGGGGGGGGG. Tamanho da subcadeia: 3. AATTTGGCCC: 5 combinações = 50%. GGGGGGGGGG: 9 combinações = 90%. Chance de 50% de ocorrência da doença CTRLF4.

Formato do arquivo de entrada:
[#Tamanho da subcadeia]
[B0 ... BN–1]
[#Número de doenças]
[Código0] [#Genes0] [G00] ... [G0i–1]
...
[CódigoM–1] [#GenesM–1] [GM–10] ... [GM–1j–1]

Exemplo de entrada:
3
AAAATTTCGTTAAATTTGAACATAGGGATA
4
ABCDE 3 AAA AAT AAAG
XY1WZ2AB 1 TTTTTTGGGG
H1N1 4 ACTG AACCGGTT AATAAT AAAAAAAAGA
HUEBR 1 CATAGGGATT

Formato do arquivo de saída:
É feita a ordenação estável em ordem decrescente dos resultados, utilizando como critério de ordenação a probabilidade de ocorrência da doença e fazendo o arredondamento dos percentuais para fins de comparação.

Exemplo de saída:
XY1WZ2AB ->100%
HUEBR ->100%
ABCDE ->67%
H1N1 ->25%

* Compressão de Dados

A empresa de telecomunicações Poxim Tech está desenvolvendo um sistema para compressão de dados, para minimizar o uso de banda na transmissão dos dados, avaliando qual técnica tem a melhor taxa de compressão. São fornecidas sequências de bytes em formato hexadecimal que possuem valores entre 00 até FF, com tamanho máximo de 10.000 caracteres. As codificações de 8 bits Run-Length Encoding (RLE) e de Huffman (HUF) são utilizadas para compressão. Na implementação da fila de prioridade mínima é utilizada uma estrutura de heap. Os símbolos são inseridos ordenados em um vetor. É feita a construção da árvore em tempo linear. A técnica que apresentar menor quantidade de bytes é selecionada para a transmissão dos dados.

Formato do arquivo de entrada:
[#Quantidade de sequências]
[#T1] [B11 ... B1n]
...
[#TN] [BN1 ... BNm]

Exemplo de entrada:
4
5 AA AA AA AA AA
7 10 20 30 40 50 60 70
9 FF FF FF FF FF FF FF FF FF
4 FA FA C1 C1

Formato do arquivo de saída:
Cada linha da saída gerada deve conter o algoritmo utilizado na compressão dos dados (RLE ou HUF) e o valor da taxa de compressão com duas casas decimais. Em uma situação onde ambos as técnicas apresentarem o mesmo número de bytes na codificação, devem ser impressas ambas as saídas, seguindo a ordem HUF e RLE.

Exemplo de saída:
0->HUF(20.00%)=00
1->HUF(42.86%)=9C6B50
2->HUF(22.22%)=0000
2->RLE(22.22%)=09FF
3->HUF(25.00%)=C0

* Backtracking

A medida que vai explorando o ambiente, o robô cria um mapa interno para as rotas exploradas. Caso uma rota não gere uma solução, outro caminho deve ser escolhido para ser explorado até que a solução seja obtida ou que não existam mais opções, ou seja, a busca é finalizada no ponto de partida. Para demonstrar suas habilidades exploratórias, são criados labirintos com exatamente 1 entrada e até 1 saída, com tamanho máximo de 100 por 100 posições. É possível que nenhuma rota seja possível para atravessar o labirinto criado, mas se existir uma saída, é sempre um espaço na borda do labirinto que não é o ponto de partida.

Formato do arquivo de entrada:
#NL
[Largura] × [Altura]
Mx,y = 0 (espaço), 1 (parede), X (partida)
M0,0 ··· M0,L–1
...
MA–1,0 ··· MA–1,L–1

Exemplo de entrada:
2
5 4
1 1 1 1 1
1 0 0 0 1
1 0 X 1 1
1 1 0 1 1
3 4
1 1 1
1 X 1
1 0 1
1 1 1

Formato do arquivo de saída:
A rota é descrita pelas coordenadas visitadas.

Exemplo de saída:
L0:INI@2,2|F->1,2|D->1,3|BT@1,3->1,2|E->1,1|T->2,1|BT@2,1->1,1|BT@1,1->1,2|BT@1,2->2,2|T->3,2|FIM@3,2
L1:INI@1,1|T->2,1|BT@2,1->1,1|FIM@-,-

* Criptografia

A empresa de tecnologia Poxim Tech está aplicando técnicas de criptografia em todos os seus sistemas de transmissão de dados, visando proteger os dados de acessos não autorizados. Os dados transmitidos entre as partes são representados por bytes no formato hexadecimal, com alinhamento de 128 bits (completando com zeros) para as mensagens utilizadas pelo AES. A criptografia simétrica AES é aplicada no modo ECB com chaves de 128, 192 ou 256 bits (tamanho definido a partir das chaves privadas a ou b). No compartilhamento das chaves privadas é utilizado o Diffie-Hellman com parâmetros de até 2.048 bits.

Formato do arquivo de entrada:
Número de operações (n)
dh a b g p
d c
e m

Exemplo de entrada:
3
dh 2F333D84630F102FDA0B594D4FF7CA46 FBDB83740FB1D83EE415C34725D377FF C54B073C6A2B3745AEAC545F8493439A568BBF2902BE07D20A359A20A9BBD26E06DAAA7005E2B5B48E0913129C57ACF2E26B1BE42923B633585054010B266F11 1219C943937D661A8CA99AA1DC0CCBC2D28018D60CAB90A8D9097BC5981C99AA3662EEC9DF54E36CFD7D0DD98AD99B5C59B332655FC20E38CB89FE63A5970EDB
d F0FA40FAF0F0CA
e 00112233445566778899AABBCCDDEEFF50C0440

Formato do arquivo de saída:
Para cada comando é exibido seu resultado.

Exemplo de saída:
s=E8613C49876806B074535ACF62DD673D
m=C6085D6C870841046C213D192E8979F3
c=4B0200E09FF592BA32668E18CAA7FDF8A4CDB7C7C48EF142575F4A5836AFBB4D
