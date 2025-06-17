#include "MergeSort.hpp"
#include "config.hpp"

void MergeSort::merge(Registro* arr, int inicio, int meio, int fim) {
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    
    // Arrays temporários
    Registro* L = new Registro[n1];
    Registro* R = new Registro[n2];
    
    // Copia dados para os arrays temporários
    for (i = 0; i < n1; i++)
        L[i] = arr[inicio + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[meio + 1 + j];
    
    // Combina os arrays temporários de volta no array original
    i = 0;
    j = 0;
    k = inicio;
    
    while (i < n1 && j < n2) {
        // Compara as chaves primárias para ordenação decrescente
        if (L[i].getChavePrimaria() > R[j].getChavePrimaria()) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copia os elementos restantes de L[], se houver
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copia os elementos restantes de R[], se houver
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

void MergeSort::mergeSort(Registro* arr, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        mergeSort(arr, inicio, meio);
        mergeSort(arr, meio + 1, fim);
        
        merge(arr, inicio, meio, fim);
    }
}

void MergeSort::ordenar(Registro* arr, int tamanho) {
    if (arr == nullptr || tamanho <= 1) return;
    // Cria array temporário apenas com registros válidos
    int countValidos = 0;
    for (int i = 0; i < tamanho; ++i) {
        if (arr[i].getStatus() == FLAGS::ATIVO) {
            countValidos++;
        }
    }
    if (countValidos <= 1) return;
    Registro* temp = new Registro[countValidos];
    int idx = 0;
    for (int i = 0; i < tamanho; ++i) {
        if (arr[i].getStatus() == FLAGS::ATIVO) {
            temp[idx++] = arr[i];
        }
    }
    // Ordena apenas os válidos
    mergeSort(temp, 0, countValidos - 1);
    // Copia de volta para o início do array original
    for (int i = 0; i < countValidos; ++i) {
        arr[i] = temp[i];
    }
    delete[] temp;
    // Opcional: os registros inválidos permanecem nas posições finais
}
