#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include "facade.hpp"
#include <iostream>

// Simula sua 'views.py'
class SignupView {
public:
    // Simula o recebimento de um JSON/POST
    void post(std::string username_front, std::string email_front, std::string password_front) {
        std::cout << "\n>>> VIEW: Recebendo POST de " << username_front << "\n";

        // 1. Instancia a Facade
        CadastroFacade fachada;

        // 2. Chama o método simples (escondendo a complexidade)
        ResultadoFacade resultado = fachada.cadastrar_cliente(username_front, email_front, password_front);

        // 3. Avalia o resultado (Simula JsonResponse)
        if (resultado.sucesso) {
            std::cout << "HTTP 200 OK: " << resultado.mensagem << "\n";
            std::cout << "Redirecionando para /dashboard/ user: " << resultado.user->getUsername() << "\n";
            
            // Limpeza do user criado (apenas pq estamos em C++)
            delete resultado.user; 
        } else {
            // Erro 400 ou 500
            std::cout << "HTTP 400/500 ERROR: " << resultado.mensagem << "\n";
        }
    }
};

#endif