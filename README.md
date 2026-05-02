# 📊 Mini Excel em C — Estruturas de Dados

> Planilha eletrônica desenvolvida do zero em **C puro**, sem frameworks ou bibliotecas externas, utilizando estruturas de dados avançadas para simular funcionalidades de um editor de planilhas real.

Projeto acadêmico desenvolvido como trabalho da disciplina de Estruturas de Dados na Universidade do Oeste Paulista (UNOESTE).

---

## 🇧🇷 Português | 🇺🇸 English

---

## 🇧🇷 Português

### Sobre o Projeto

O projeto implementa um editor de planilhas interativo no terminal, com navegação por teclado, edição de células, suporte a fórmulas com referências entre células, geração de gráficos de barras e persistência de dados em arquivo binário.

O diferencial técnico está na escolha das estruturas de dados:

- **Matriz Esparsa** para armazenar células — apenas posições preenchidas ocupam memória, independente do tamanho da grade
- **Lista Genérica** para representar e avaliar expressões matemáticas com precedência de operadores
- **Pilhas** separadas para operadores (`char`), valores (`float`) e nós de expressão (`ListaGen`) — usadas no algoritmo de parsing e cálculo de fórmulas

---

### 🛠️ Estruturas de Dados Implementadas

| Estrutura | Arquivo | Finalidade |
|---|---|---|
| Matriz Esparsa | `TAD_MatEsp.h` | Armazenamento eficiente de células da planilha |
| Lista Genérica | `TAD_ListaGen.h` | Parsing e avaliação de fórmulas e expressões |
| Pilha de Char | `TAD_Pilha_Char.h` | Controle de operadores durante cálculo |
| Pilha de Float | `TAD_Pilha_Float.h` | Controle de valores durante cálculo |
| Pilha de ListaGen | `TAD_Pilha_ListaGen.h` | Controle de subexpressões com parênteses |

---

### ⚙️ Funcionalidades

**Navegação e Interface:**
- Grade de células com janela deslizante (20 linhas × 8 colunas visíveis)
- Navegação com setas do teclado — a janela se desloca ao atingir as bordas
- Interface colorida no terminal com coordenadas no formato padrão (A1, B2, ...)
- Atalhos: `F2` Editar célula · `F3` Salvar · `F4` Abrir · `F5` Gráfico

**Edição e Fórmulas:**
- Edição direta de células com texto ou valores numéricos
- Suporte a fórmulas iniciadas com `=` (ex: `=A1+B2*3`)
- Referências entre células — uma célula pode referenciar outra
- Suporte a expressões aninhadas com parênteses
- Funções built-in: `SUM()`, `AVG()`, `MAX()`, `MIN()` por intervalo de células (linha ou coluna)

**Persistência:**
- Salvar planilha em arquivo binário
- Abrir planilha salva anteriormente

**Visualização:**
- Geração de **gráfico de barras** colorido no terminal a partir de um intervalo de células
- Suporte a múltiplas séries com cores distintas por linha
- Escala automática baseada nos valores máximo e mínimo do intervalo

---

### 🔬 Destaques Técnicos

**Matriz Esparsa Bidirecional:**
A planilha usa uma estrutura de nós interligados por linha (`pl`) e por coluna (`pc`), com listas encadeadas separadas para linhas e colunas. Isso permite busca eficiente por célula sem alocar memória para células vazias.

**Parser de Expressões:**
O algoritmo `ConstroiListaGen` tokeniza a expressão (`SeparaTermos`), constrói uma Lista Genérica com nós tipados (`ConsV` para valores, `ConsO` para operadores, `ConsF` para funções/referências) e avalia usando o algoritmo de **duas pilhas** com suporte a precedência (`*` e `/` antes de `+` e `-`) e parênteses aninhados.

**Avaliação Recursiva:**
Células com fórmulas que referenciam outras células com fórmulas são avaliadas recursivamente via `VerificaFuncao` → `ConstroiListaGen`, permitindo encadeamento de referências.

---

### ▶️ Como Compilar e Executar

**Pré-requisitos:**
- Compilador GCC (MinGW no Windows)
- Sistema operacional Windows (uso de WinAPI para console)

**Compilar:**
```bash
gcc excel.cpp -o excel -lm
```

**Executar:**
```bash
./excel
```

**Atalhos:**
| Tecla | Ação |
|---|---|
| ↑ ↓ ← → | Navegar entre células |
| F2 | Editar célula selecionada |
| F3 | Salvar planilha |
| F4 | Abrir planilha |
| F5 | Gerar gráfico de barras |
| ESC | Sair |

**Exemplo de fórmulas suportadas:**
```
=A1+B2
=A1*2+B3/4
=(A1+B1)*C1
=SUM(A1:A5)
=AVG(A1:D1)
=MAX(B1:B10)
=MIN(A1:C1)
```

---

### 📁 Estrutura de Arquivos

```
├── excel.cpp              # Interface, navegação e controle principal
├── TAD_MatEsp.h           # Matriz Esparsa + funções de planilha (SUM, AVG, MAX, MIN)
├── TAD_ListaGen.h         # Lista Genérica + parser e avaliador de fórmulas
├── TAD_Pilha_Char.h       # Pilha de caracteres (operadores)
├── TAD_Pilha_Float.h      # Pilha de floats (valores)
└── TAD_Pilha_ListaGen.h   # Pilha de nós ListaGen (subexpressões)
```

---

## 🇺🇸 English

### About

A spreadsheet editor built from scratch in **pure C**, without any external frameworks or libraries, using advanced data structures to simulate real spreadsheet functionality.

Academic project developed for the Data Structures course at Universidade do Oeste Paulista (UNOESTE).

### Key Technical Highlights

- **Sparse Matrix** for cell storage — only filled positions use memory, regardless of grid size
- **Generic List** for expression parsing and evaluation with operator precedence
- **Three separate Stacks** (char, float, ListaGen) for the two-stack arithmetic evaluation algorithm
- **Recursive formula evaluation** — cells can reference other formula cells with full chain resolution
- Built-in functions: `SUM()`, `AVG()`, `MAX()`, `MIN()` over cell ranges
- Bar chart generation in terminal with automatic scaling and colored series
- Binary file persistence for save/load

### How to Build

```bash
gcc excel.cpp -o excel -lm
./excel
```

> ⚠️ Requires Windows (WinAPI used for console rendering). MinGW recommended.

---

## 👨‍💻 Autor / Author

**Guilherme Zorato Vernilo**

[![LinkedIn](https://img.shields.io/badge/LinkedIn-Guilherme_Zorato-0077B5?style=flat&logo=linkedin)](https://linkedin.com/in/guilherme-zorato)
[![GitHub](https://img.shields.io/badge/GitHub-Zoratoo-181717?style=flat&logo=github)](https://github.com/Zoratoo)
