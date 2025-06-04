# 🌳 WinRar — Codificação e Decodificação com Árvore de Huffman

Este projeto simula um sistema de **compactação e descompactação de dados** utilizando o **Algoritmo de Huffman**, uma técnica clássica e eficiente para compressão de dados baseada na frequência de palavras.

---

## Funcionalidades

- Leitura da frequência das palavras no texto.
- Construção da **Árvore de Huffman**.
- Geração de códigos binários para cada palavra.
- Compressão do texto original em formato binário.
- Descompressão e reconstrução do texto original.
- Armazenamento dos códigos e dos dados compactados em arquivos.

---

## Tecnologias Utilizadas

- **Estruturas:** Árvores binárias, listas duplamente encadeadas e pilhas.
- Algoritmo clássico de **Huffman**.

---

## 📁 Arquivos

- `codifica.cpp` → Responsável pela leitura do texto, construção da árvore de Huffman, geração dos códigos e codificação dos dados em arquivo.
- `decodifica.cpp` → Responsável pela leitura dos dados codificados, reconstrução da árvore e descompactação para recuperar o texto original.

---

## 📜 Como Funciona

1. O programa **Codifica** lê um texto, calcula a frequência das palavras e gera os códigos de Huffman.
2. Gera uma árvore binária onde cada caminho define o código binário de uma palavra (esquerda → `0`, direita → `1`).
3. Salva os códigos e o arquivo compactado.
4. O programa **Decodifica** lê esses dados, reconstrói a árvore e traduz o binário de volta para o texto original.
