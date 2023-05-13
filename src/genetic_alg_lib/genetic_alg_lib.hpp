//
// Created by Edumarek on 06/02/2023.
//

#ifndef GENETIC_ALG_LIB_HPP
#define GENETIC_ALG_LIB_HPP

#include "../fuzzy_lib/fuzzy_lib.hpp"
#include <Arduino.h>

#define _ponto double

const int COEFICIENTE_C_EK = 5000;
const int COEFICIENTE_C_DEK = 500;
const int COEFICIENTE_C_DU = 100;
const int COEFICIENTE_B_EK = 5000;
const int COEFICIENTE_B_DEK = 500;

struct Individuo : public ControladorFuzzy
{
    _ponto somatorioEk = 0;
    _ponto somatorioUk = 0;
    _ponto score = 0;
    _ponto coeficienteMutacao;

    Individuo()
    {
        centrosEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));

        for (int i = 0; i < N_Regras; i++)
        {
            centrosEk[i] = (rand() % (2 * COEFICIENTE_C_EK)) - COEFICIENTE_C_EK;
            centrosDEk[i] = (rand() % (2 * COEFICIENTE_C_DEK)) - COEFICIENTE_C_DEK;
            centrosDU[i] = (rand() % (2 * COEFICIENTE_C_DU)) - COEFICIENTE_C_DU;
            baseEk[i] = (rand() % (2 * COEFICIENTE_B_EK)) - COEFICIENTE_B_EK;
            baseDEk[i] = (rand() % (2 * COEFICIENTE_B_DEK)) - COEFICIENTE_B_DEK;
        }
    }
    Individuo(_ponto min_uK, _ponto max_uK, _ponto _coeficienteMutacao)
    {
        min_Uk = min_uK;
        max_Uk = max_uK;
        coeficienteMutacao = _coeficienteMutacao;
    }
    Individuo(const Individuo &I)
    {
        centrosEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));

        for (int i = 0; i < N_Regras; i++)
        {
            centrosEk[i] = (rand() % (2 * COEFICIENTE_C_EK)) - COEFICIENTE_C_EK;
            centrosDEk[i] = (rand() % (2 * COEFICIENTE_C_DEK)) - COEFICIENTE_C_DEK;
            centrosDU[i] = (rand() % (2 * COEFICIENTE_C_DU)) - COEFICIENTE_C_DU;
            baseEk[i] = (rand() % (2 * COEFICIENTE_B_EK)) - COEFICIENTE_B_EK;
            baseDEk[i] = (rand() % (2 * COEFICIENTE_B_DEK)) - COEFICIENTE_B_DEK;
        }

        min_Uk = I.min_Uk;
        max_Uk = I.max_Uk;
        coeficienteMutacao = I.coeficienteMutacao;

        for (int i = 0; i < N_Regras; i++)
        {
            centrosEk[i] = I.centrosEk[i];
            centrosDEk[i] = I.centrosDEk[i];
            centrosDU[i] = I.centrosDU[i];
            baseEk[i] = I.baseEk[i];
            baseDEk[i] = I.baseDEk[i];
        }
    }
    Individuo(const Individuo &A, const Individuo &B)
    {
        centrosEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        centrosDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        baseDEk = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDE = (_ponto *)malloc(N_Regras * sizeof(_ponto));
        miDU = (_ponto *)malloc(N_Regras * sizeof(_ponto));

        for (int i = 0; i < N_Regras; i++)
        {
            centrosEk[i] = (rand() % (2 * COEFICIENTE_C_EK)) - COEFICIENTE_C_EK;
            centrosDEk[i] = (rand() % (2 * COEFICIENTE_C_DEK)) - COEFICIENTE_C_DEK;
            centrosDU[i] = (rand() % (2 * COEFICIENTE_C_DU)) - COEFICIENTE_C_DU;
            baseEk[i] = (rand() % (2 * COEFICIENTE_B_EK)) - COEFICIENTE_B_EK;
            baseDEk[i] = (rand() % (2 * COEFICIENTE_B_DEK)) - COEFICIENTE_B_DEK;
            min_Uk = A.min_Uk;
            max_Uk = A.max_Uk;
            coeficienteMutacao = A.coeficienteMutacao;
        }

        for (int i = 0; i < N_Regras; i++)
        {
            // cross-over
            //  centrosEk
            if (rand() % 2)
                centrosEk[i] = A.centrosEk[i];
            else
                centrosEk[i] = B.centrosEk[i];

            // centrosDEk
            if (rand() % 2)
                centrosDEk[i] = A.centrosDEk[i];
            else
                centrosDEk[i] = B.centrosDEk[i];

            // centrosDU
            if (rand() % 2)
                centrosDU[i] = A.centrosDU[i];
            else
                centrosDU[i] = B.centrosDU[i];

            // basesEk
            if (rand() % 2)
                baseEk[i] = A.baseEk[i];
            else
                baseEk[i] = B.baseEk[i];

            // basesDEk
            if (rand() % 2)
                baseDEk[i] = A.baseDEk[i];
            else
                baseDEk[i] = B.baseDEk[i];

            // mutacao
            if ((rand() % 10000) / 100 < coeficienteMutacao)
            {
                switch (rand() % N_Regras)
                {
                case 0:
                    centrosEk[rand() % N_Regras] = rand() % COEFICIENTE_C_EK;
                    break;
                case 1:
                    centrosDEk[rand() % N_Regras] = rand() % COEFICIENTE_C_DEK;
                    break;
                case 2:
                    centrosDU[rand() % N_Regras] = rand() % COEFICIENTE_C_DU;
                    break;
                case 3:
                    baseEk[rand() % N_Regras] = rand() % COEFICIENTE_B_EK;
                    break;
                case 4:
                    baseEk[rand() % N_Regras] = rand() % COEFICIENTE_B_DEK;
                    break;
                default:
                    break;
                }
            }
        }
    }

    void calcula_score()
    {
        _ponto alfa = 1, beta = 1;

        score = (alfa * (somatorioEk * somatorioEk)) + (beta * (somatorioUk * somatorioUk));
    }
};

struct GAsandbox
{
    int tamanhoPopulacao;
    Individuo **populacao;
    Individuo **backupPopulacao;
    int numeroGeracao = 0;

    GAsandbox(int _tamanhoPopulacao, _ponto min_uK, _ponto max_uK, _ponto coeficienteMutacao);
    ~GAsandbox();

    int selecao_individuo(int max_score, _ponto *arr);
    void iteracao();
};

#endif // GENETIC_ALG_HPP
