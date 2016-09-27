#!/bin/bash

usage () {
    echo "/******************************************************************/"
    echo "Instruções: "
    echo "  1.Coloque esse script no diretório que contem os arquivos de teste."
    echo "  2.O padrão de nomeação dos arquivos de teste deve ser arqXX.in (entrada) e arqXX.res (saída esperada)."
    echo "  3.O padrão de nomeação do arquivo de código é labXX.c."
    echo "  4.O primeiro parametro deve ser o número do lab."
    echo "  5.O segundo parametro deve ser o número de testes a serem rodados."
    echo "  6.A saída será a diferença entre os arquivos de saída gerados pelo programa no padrão arqXX.out e os arquivos arqXX.res."
    echo
    echo "Exemplo: sh run_tests.sh 1 20"
    echo "/******************************************************************/"
    exit 0
}

if test $# -lt 2; then
    usage
fi

if test $1 -lt 10; then
    labNumber=0$1
else
    labNumber=$1
fi

labFolder=lab$labNumber
packagesFolder=$labFolder/target
testsFolder=$labFolder/tests

if [ ! -d $packagesFolder ]; then
    mkdir $packagesFolder
fi

if [ ! -d $testsFolder ]; then
    mkdir $testsFolder
fi

if g++ -ansi -pedantic -Wall -lm $labFolder/*.cpp -o $packagesFolder/lab$labNumber; then

    if test $2 -gt 0; then
        echo "Running tests..."
    fi

    for i in $(seq 01 $2)
    do
        if test $i -lt 10; then
            testName=arq0$i
        else
            testName=arq$i
        fi

        if [ ! -f $testsFolder/$testName.in ]; then
            curl -k -s -o $testsFolder/$testName.in https://susy.ic.unicamp.br:9999/mc558ab/$labNumber/dados/$testName.in > /dev/null
        fi

        if [ ! -f $testsFolder/$testName.res ]; then
            curl -k -s -o $testsFolder/$testName.res https://susy.ic.unicamp.br:9999/mc558ab/$labNumber/dados/$testName.res > /dev/null
        fi

        ./$packagesFolder/lab$labNumber < $testsFolder/$testName.in > $testsFolder/$testName.out

        if [[ -z  $(diff "$testsFolder/$testName.res" "$testsFolder/$testName.out") ]]; then
            echo "test $testName: correct"
        else
            echo "test $testName: incorrect"
            diff $testsFolder/$testName.res $testsFolder/$testName.out
        fi
    done
fi