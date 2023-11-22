#include "chef.h"
#include <iostream>
#include <sstream>
#include <zconf.h>

unsigned int Chef::contador = 0;

#define CHEFE_COZINHA_DIR "./resources/chefes/"

#define LEITURA 0
#define ESCRITA 1


Chef::Chef() : id(++contador) {
    stringstream nome;
    nome << "ChefeCozinha_" << id;
    this->atualizarArquivo(nome.str());
}

Chef::~Chef() = default;

/**
 * Acho que aqui vair vir o fork()
 * tambem instanceará um atendimento
 * e no atendimento iniciar o pipe
*/
void Chef::iniciarAtendimento(const unsigned int mesa) {
    // Implemente seu código aqui...
    this->atendimento = new Chef::Atendimento(mesa);
    
}

void Chef::atualizarArquivo(const std::string info){
    stringstream nome;
    nome << "ChefeCozinha_" << id;

    ofstream log;
    log.open(CHEFE_COZINHA_DIR+nome.str() + ".txt");
    log << info << endl;
    log.close();
}

void Chef::preparar(const string &pedido) {
    pid_t pid = this->atendimento->pid;   
    int *fd = this->atendimento->fd; 

    this->atendimento->preparar(pedido);
    if(this->atendimento->pid < 1) return;
    std::cout << "Chef está coisando para o atendimento." << endl;
    close(fd[LEITURA]); // Fecha a leitura pois aqui apenas a escrita será usada
    write(fd[ESCRITA], pedido.c_str(), pedido.size()+1);
}

void Chef::finalizarAtendimento() {
    // Implemente seu código aqui...
    delete this->atendimento;
}

Chef::Atendimento::Atendimento(const unsigned int mesa) {
    // Implemente seu código aqui...
    if(pipe(this->fd) < 0){
        std::cerr << "Falhou o pipe!" << endl;
        return;
    }

    std::cout << "iniciando pipe para comunicar o atendimento na mesa "<<mesa<<endl;
    this->pid = fork();
}

Chef::Atendimento::~Atendimento() {
    close(fd[LEITURA]);
    exit(0);
}

void Chef::Atendimento::preparar(const string &pedido) {
    if(this->pid > 0) return;
    char recebimento[pedido.size()+1];
    close(this->fd[ESCRITA]);
    read(this->fd[LEITURA], recebimento, pedido.size()+1);
    std::cout << "O Atendimento está recebendo o seguinte pedido: " << recebimento << endl;
}