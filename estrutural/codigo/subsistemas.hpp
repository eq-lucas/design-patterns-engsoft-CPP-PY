#ifndef SUBSISTEMAS_HPP
#define SUBSISTEMAS_HPP

#include <iostream>
#include <string>

// --- MODEL USER (Simula UserPersonalizado) ---
class User {
private:
    std::string username;
    std::string email;
public:
    User(std::string u, std::string e) : username(u), email(e) {}
    
    std::string getUsername() const { return username; }
};

// --- SUBSISTEMA 1: DB MANAGER (Simula User.objects.create) ---
class UserModelManager {
public:
    User* create_user(std::string username, std::string email, std::string password) {
        std::cout << "   [DB] UserPersonalizado criado: " << username << "\n";
        return new User(username, email);
    }
};

// --- SUBSISTEMA 2: CLIENTE MANAGER (Simula Cliente.objects.create) ---
class ClienteModelManager {
public:
    void create(User* user) {
        std::cout << "   [DB] Perfil 'Cliente' vinculado ao user " << user->getUsername() << "\n";
    }
};

// --- SUBSISTEMA 3: FORMULÁRIO (Simula forms.CadastroClienteForm) ---
class CadastroClienteForm {
public:
    bool is_valid(std::string email, std::string password) {
        // Validação simples: Email tem '@' e senha > 3 digitos
        bool email_ok = (email.find('@') != std::string::npos);
        bool pass_ok = (password.length() > 3);

        if (!email_ok) std::cout << "   [Form] Erro: Email invalido.\n";
        if (!pass_ok) std::cout << "   [Form] Erro: Senha muito curta.\n";

        return email_ok && pass_ok;
    }
};

// --- SUBSISTEMA 4: TRANSAÇÃO (Simula transaction.atomic) ---
class TransactionManager {
public:
    void atomic_begin() { std::cout << "   [Transaction] BEGIN (Iniciando bloco atomico)...\n"; }
    void atomic_commit() { std::cout << "   [Transaction] COMMIT (Salvando tudo)...\n"; }
    void atomic_rollback() { std::cout << "   [Transaction] ROLLBACK (Desfazendo alteracoes)...\n"; }
};

#endif