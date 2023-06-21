/*Escreva um programa em C/C++ que simule o funcionamento de um sistema de pedágio em uma rodovia.
O programa deve usar threads para representar os veículos que passam pelo pedágio e deve explorar a solução do problema da seção crítica
(solucionando com trava ou semáforo) e sincronismo condicional (usando barreira).
Cada veículo deve escolher aleatoriamente uma das cabines de pedágio para passar e deve esperar sua vez para pagar o pedágio.
Quando um veículo paga o pedágio, ele deve incrementar o valor total arrecadado pelo pedágio.
O programa deve garantir que apenas um veículo por vez possa pagar o pedágio em cada cabine
(usando trava ou semáforo para solucionar o problema da seção crítica).
Além disso, o programa deve usar uma barreira para sincronizar os veículos de forma que eles só possam sair do pedágio depois que
todos os veículos tenham pago o pedágio. Depois que todos os veículos saírem do pedágio, o programa deve imprimir
o valor total arrecadado pelo pedágio.
O programa deve terminar quando todos os veículos tiverem passado pelo pedágio.*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

const int QUANT_CAB = 3; // quantidade de cabines de pedágio
const int QUANT_VEICULOS = 10; // quantidade de veículos que passarão pelo pedágio
const int VALOR_PEDAGIO = 5; // valor do pedágio

mutex mtx; // trava para controlar o acesso as cabines

int lucroPedagio = 0; // valor total arrecadado pelo pedágio
int veiculosPassaram = 0; // contador de veículos que passaram pelo pedágio

void pagamentoPedagio(int idVeiculo) { // simula o processamento do pagamento do pedágio
    
    this_thread::sleep_for(chrono::milliseconds(100));

    lucroPedagio += VALOR_PEDAGIO; // incrementa o valor total arrecadado pelo pedágio
    veiculosPassaram++; // incrementa a quantidade de veiculos que ja passaram

}

void threadVeiculos(int idVeiculo) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, QUANT_CAB - 1); // escolhe aleatoriamente uma cabine de pedágio
    int idCabine = dist(gen);

    unique_lock<mutex> lock(mtx); // aguarda a vez de passar pelo pedágio

    pagamentoPedagio(idVeiculo); // paga o pedágio

    if (veiculosPassaram == QUANT_VEICULOS) {
        cout << "O pedágio arrecadou " << lucroPedagio << " reais" << endl;
    }
}

int main() {
    vector<thread> threads;

    for (int i = 0; i < QUANT_VEICULOS; i++) { // cria as threads dos veículos
        threads.emplace_back(threadVeiculos, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}