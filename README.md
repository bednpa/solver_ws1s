
## WS1S solver

V tomto souboru je popsán překlad, spuštění, formát vstupního souboru a adresářová struktura pro WS1S solver. Nástroj pracuje tak, že má na vstupu WS1S formuli, v syntaxi používané nástrojem Mona[^1] (detailní syntax se nachází v manuálu Mony[^3]), na výstupu vypíše zda je platná, nesplnitelná nebo splnitelná (včetně ukázkového přiřazení proměnným) a vytvoří korespondující automat.

### Překlad a spuštění

Spustitelný program vytvoříme tímto příkazem:

```
make
 ```

Nápovědu zobrazíme příkazem:

```
./bin/ws1s_solver -h
```

Příklad spuštění:

```
./bin/ws1s_solver -f ./benchmark/horn-sub/horn_sub02.mona -s -t
```

Pomocí následujícího příkazu můžeme vygenerovat obrázek automatu, kde symboly na hranách a/n znamenají přechodový symbol a a délku hrany n, reprezentujícího vypočtenou formuli:

```
make aut
 ```

### Syntax formulí

Nástrojem podporovaná podmnožina syntaxe Mony je naznačena zde (gramatika převzata z nástroje Gaston[^4]):

```
program ::= (header;)? (declaration;)+
header ::=  ws1s

declaration ::= formula
             |  var0 (varname)+
             |  var1 (varname)+
             |  var2 (varname)+
             |  'pred' varname (params)? = formula
             |  'macro' varname (params)? = formula

formula ::= 'true' | 'false' | (formula)
         |  zero-order-var
         | ~formula
         | formula | formula
         | formula & formula
         | formula => formula
         | formula <=> formula
         | first-order-term = first-order-term
         | first-order-term ~= first-order-term
         | first-order-term < first-order-term
         | first-order-term > first-order-term
         | first-order-term <= first-order-term
         | first-order-term >= first-order-term
         | second-order-term = second-order-term
         | second-order-term = { (int)+ }
         | second-order-term ~= second-order-term
         | second-order-term 'sub' second-order-term
         | first-order-term 'in' second-order-term
         | ex0 (varname)+ : formula
         | all0 (varname)+ : formula
         | ex1 (varname)+ : formula
         | all1 (varname)+ : formula
         | ex2 (varname)+ : formula
         | all2 (varname)+ : formula

first-order-term ::= varname | (first-order-term)
                  |  int
                  | first-order-term + int

second-order-term ::= varname | (second-order-term)
                   |  second-order-term + int
```

### Adresářová struktura

Adresářová struktura je následující:

```
├── benchmark
├── build
├── thesis
├── doc
├── dot
├── include
├── bin
└── src
    ├── ast
    ├── mata
    └── core
```

Složka ```benchmark``` obsahuje podsložky s WS1S formulemi použitými k testování a evaluaci. V adresáři ```build``` se vytvoří pomocné soubory během překladu. Složka ```thesis``` obsahuje zdrojové kódy a výsledný text diplomové práce na toto téma . Do adresáře ```doc``` se příkazem ```make doc``` vygeneruje doxygen dokumentace. Složka ```dot``` obsahuje vygenerované obrázky automatů po spuštění programu na nějaké formuli (z obrázkové reprezentace se vygeneruje obrázek příkazem ```make aut```). Adresář ```include``` obsahuje použité knihovny z Mona a Mata[^2]. Do složky ```bin``` se po překladu vytvoří spustitelný soubor. V adresářích ```src/ast``` (převzato z Mona a upraveno), ```src/mata``` (převzato z Mata a upraveno) a ```src/core``` se nachází zdrojové kódy programu.

[^1]: https://www.brics.dk/mona/
[^2]: https://github.com/VeriFIT/mata
[^3]: http://www.brics.dk/mona/mona14.pdf
[^4]: https://github.com/tfiedor/gaston
