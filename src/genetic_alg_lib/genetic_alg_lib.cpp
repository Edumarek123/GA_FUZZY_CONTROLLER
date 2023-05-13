//
// Created by Edumarek on 06 / 02 / 2023.
//
#include "genetic_alg_lib.hpp"

GAsandbox::GAsandbox(int _tamanhoPopulacao, _ponto min_uK, _ponto max_uK, _ponto coeficienteMutacao)
{
    tamanhoPopulacao = _tamanhoPopulacao;

    populacao = (Individuo **)malloc(tamanhoPopulacao * sizeof(Individuo *));
    for (int i = 0; i < tamanhoPopulacao; i++)
        populacao[i] = new Individuo(min_uK, max_uK, coeficienteMutacao);

    backupPopulacao = (Individuo **)malloc(tamanhoPopulacao * sizeof(Individuo));
}

GAsandbox::~GAsandbox()
{
    for (int i = 0; i < tamanhoPopulacao; i++)
    {
        delete populacao[i];
    }
    free(populacao);
    free(backupPopulacao);
}

int GAsandbox::selecao_individuo(int max_score, _ponto *arr)
{
    _ponto valor;
    valor = rand() % (max_score * 100) / 100;

    for (int i = 0; i < tamanhoPopulacao; i++)
    {
        if (valor < arr[i])
            return i;
    }
}

void GAsandbox::iteracao()
{
    _ponto max_score = -1;
    _ponto somatorioScore = 0;
    for (int i = 0; i < tamanhoPopulacao; i++)
    {
        populacao[i]->calcula_score();
        if (populacao[i]->score > max_score)
            max_score = populacao[i]->score;
        somatorioScore += populacao[i]->score;
    }

    _ponto *arraySelecao = (_ponto *)malloc(tamanhoPopulacao * sizeof(_ponto));

    for (int i = 0; i < tamanhoPopulacao; i++)
    {
        populacao[i]->score /= somatorioScore;
        if (i == 0)
            arraySelecao[i] = populacao[i]->score;
        else
            arraySelecao[i] = populacao[i]->score + arraySelecao[i - 1];
    }
    max_score = arraySelecao[tamanhoPopulacao - 1];

    for (int i = 0; i < tamanhoPopulacao; i++)
        backupPopulacao[i] = new Individuo(*populacao[selecao_individuo(max_score, arraySelecao)], *populacao[selecao_individuo(max_score, arraySelecao)]);
    free(arraySelecao);

    for (int i = 0; i < tamanhoPopulacao; i++)
    {
        delete populacao[i];
        populacao[i] = new Individuo(*backupPopulacao[i]);
        delete backupPopulacao[i];
    }

    numeroGeracao++;
    Serial.println("Geracao: " + String(numeroGeracao) + " | Max Score: " + String(max_score));
}
