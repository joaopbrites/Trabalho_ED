cmake_minimum_required(VERSION 3.15) # versão mínima para rodar esse o cmake
#verificar no linux do laboratorio

project(Teste VERSION 1.0) #nome provisório 



add_executable(
    ${PROJECT_NAME} 
    src/components/Buffer.cpp
    src/components/type_block.cpp
    src/components/type_dado.cpp
    src/components/Logger.cpp

    src/arquivo-manager/gravacao/gravar_blocos_binarios.cpp
    src/arquivo-manager/leitura/LeitorBin.cpp 
    src/arquivo-manager/leitura/LeitorCSV.cpp

    src/ordenacao/merge_kway_manager.cpp
    src/ordenacao/MergeSort.cpp

    src/main.cpp
)


# Adiciona o diretório 'include' ao caminho de busca por arquivos .h/.hpp
target_include_directories(${PROJECT_NAME} PUBLIC headers)
target_include_directories(${PROJECT_NAME} PUBLIC protocolos)
target_include_directories(${PROJECT_NAME} PUBLIC teste)


# Adiciona flags de compilação que o compilador normalmente ignora.
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g)


#verificar aaversão de c++ do laboratorio
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
