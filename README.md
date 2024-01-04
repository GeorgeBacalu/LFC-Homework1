# Diferența între un sistem de rescriere și o gramatică

Un sistem de rescrierea era format din 2 elemente (V = vocabular/alfabet și F = reguli de rescriere/producții). Producțiile făcând trecerea de la o secvență de elemente din vocabular la o alta secvență (practic un replace, adica o rescriere :) ) si aveau ca scop principal, asa cum le spune numele, rescrierea unui cuvant dupa un anume set de reguli.

Gramaticile sunt un sistem usor mai complex, fiind formate din 4 elemente:

1. $V_N$: mulțimea **N**eterminalelor
1. $V_T$: mulțimea **T**erminalelor
3. $S$: simbolul de **S**tart
4. $P = \{(u,v)| \text{unde } u,v \in (V_N \cup V_T)^* \}$: mulțimea de **P**roducții de tip $ u \rightarrow v$. Vom numi $u$ partea stângă și $v$ partea dreaptă a producției

si au ca scop principal generarea de cuvinte, urmand un anume set de reguli (definite prin mulțimea de producții). Daca la Markov verificam ca un cuvant poate fi rescris intr-un anume mod, la gramatici vom verifica daca un cuvant respecta sau nu regulile gramaticii (dacă aparține gramaticii, daca este "*acceptat*").

Un exemplu de gramatică este:

$G = (\{S, A\}, \{a,b\}, S, P), \text{cu producțiile}\\
(1)\ S \rightarrow abS\\
(2)\ S \rightarrow aSAb\\
(3)\ S \rightarrow aA\\
(4)\ A \rightarrow aAb\\
(5)\ A \rightarrow a
$

sau

$G = (\{S, B, C\}, \{a,b,c\}, S, P), \text{cu producțiile}\\
(1)\ S \rightarrow aSBC\\
(2)\ S \rightarrow aBC\\
(3)\ CB \rightarrow BC\\
(4)\ bB \rightarrow bb\\
(5)\ bC \rightarrow bc\\
(6)\ cC \rightarrow Cc\\
(7)\ aB \rightarrow ab
$

Ca observații:
1. observăm că în partea stângă e obligatoriu să avem macar un **N**eterminal, dar putem avea oricâte terminale/neterminale (așa cum poate nu s-ar înțelege din primul exemplu de gramatica)

1. la generarea unui cuvant in gramatica, spre deosebire de Markov (unde alegeam mereu prima regula aplicabila si o aplicam asupra primului grup), la gramatici vom alege random o productie din cele aplicabile, la o pozitie random din cele aplicabile (dacă există mai multe).

1. nu exista conceptul de regula finala. Vom aplica reguli pana cand nu mai exista nici o regula aplicabila.

# Exemplu de fisier de intrare
```python
3       # numarul de elemente din multimea V_N
S B C   # elementele din V_N separate prin spatiu
3       # numarul de elemente din multimea V_T
a b c   # elementele din V_T separate prin spatiu
S       # simbolul de start
7       # numarul de productii
S aSBC  # productia 1, avand membrul stang separat de membrul drept prin spatiu
S aBC   # ...
CB BC
bB bb
bC bc
cC Cc
aB ab
```

# Pseudocodul algoritmului de generare
```python
# 1. initializez cuvantul curent cu simbolul de start
# 2. determin productiile aplicabile cautand daca membrul stang al unei productii se regaseste in cuvantul curent
# 3. selectez random o productie aplicabila
# 4. determin pozitiile unde poate fi aplicata productia
# 5. aleg o pozitie random si aplic regula (inlocuiesc aparitia membrului stang cu membrul drept)
# 6. repeta incepand cu pasul 2 pana cand nu mai sunt productii aplicabile
# 7. returneaza cuvantul curent
```

> Obs: atenție la inlocuirea in cazul producțiilor care au caracterul *lambda*, caz in care nu se va face o inlocuire, ci o stergere.
