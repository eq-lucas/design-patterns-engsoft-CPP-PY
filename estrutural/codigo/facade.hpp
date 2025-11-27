#ifndef FACADE_HPP
#define FACADE_HPP

#include "subsistemas.hpp"
#include <string>

// Estrutura para retornar o resultado complexo (sucesso/erro/user)
// Equivalente ao dicionário que você retorna no Python
struct ResultadoFacade {
    bool sucesso;
    std::string mensagem; // Erro ou sucesso
    User* user;           // O objeto criado (se sucesso)
};

class CadastroFacade {
private:
    // A Facade possui as ferramentas necessárias
    CadastroClienteForm* form;
    UserModelManager* userModel;
    ClienteModelManager* clienteModel;
    TransactionManager* transacao;

public:
    CadastroFacade() {
        // Inicializa os subsistemas
        this->form = new CadastroClienteForm();
        this->userModel = new UserModelManager();
        this->clienteModel = new ClienteModelManager();
        this->transacao = new TransactionManager();
    }

    ~CadastroFacade() {
        delete form;
        delete userModel;
        delete clienteModel;
        delete transacao;
    }

    // O MÉTODO MÁGICO (cadastrar_cliente)
    ResultadoFacade cadastrar_cliente(std::string username, std::string email, std::string password) {
        std::cout << "--- [Facade] Iniciando processo de cadastro ---\n";

        // 1. Validação (Formulário)
        if (this->form->is_valid(email, password) == false) {
            return {false, "Erros de validacao no formulario", nullptr};
        }

        // 2. Orquestração de Persistência (Transação)
        this->transacao->atomic_begin();

        try {
            // Passo A: Cria User
            User* novo_user = this->userModel->create_user(username, email, password);

            // Passo B: Cria Cliente (Vinculado)
            // Simulamos um erro aqui: se o nome for "erro_db", falha.
            if (username == "erro_db") throw 500; 

            this->clienteModel->create(novo_user);

            // Se chegou aqui, deu tudo certo
            this->transacao->atomic_commit();
            
            std::cout << "--- [Facade] Cadastro finalizado com sucesso ---\n";
            return {true, "Cadastro OK", novo_user};

        } catch (...) {
            // Se der erro no meio do caminho, desfaz tudo (Rollback)
            this->transacao->atomic_rollback();
            return {false, "Erro Critico no Banco de Dados", nullptr};
        }
    }
};

#endif