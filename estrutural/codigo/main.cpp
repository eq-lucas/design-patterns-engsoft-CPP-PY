#include "cliente.hpp"

int main() {
    SignupView view;

    // CENÁRIO 1: Sucesso (Dados válidos)
    view.post("joao_silva", "joao@email.com", "senha123");

    // CENÁRIO 2: Erro de Validação (Senha curta, email sem @)
    view.post("joao_errado", "email_ruim", "12");

    // CENÁRIO 3: Erro de Banco de Dados (Rollback)
    // O nome "erro_db" ativa a simulação de falha no facade.hpp
    view.post("erro_db", "joao@email.com", "senha123");

    return 0;
}