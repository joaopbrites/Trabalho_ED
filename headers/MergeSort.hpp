#pragma once

#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include "type_dado.hpp"

class MergeSort {
private:
    // Método auxiliar para combinar duas partes ordenadas
    static void merge(Registro* arr, int inicio, int meio, int fim);
    
    // Método recursivo principal do merge sort
    static void mergeSort(Registro* arr, int inicio, int fim);

public:
    // Método público para ordenar um array de registros
    static void ordenar(Registro* arr, int tamanho);
};

#endif
