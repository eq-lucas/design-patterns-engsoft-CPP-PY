#include "factory_method.hpp"

/**
 * Código Cliente.
 * Trabalha com uma instância de um criador, mas através da interface base.
 * Isso permite passar qualquer subclasse de criador para o cliente.
 */
void ClientCode(const Creator& creator) {
    std::cout << "Client: Eu nao sei a classe exata do criador, mas ainda funciona.\n"
              << creator.SomeOperation() << std::endl;
}

int main() {
    std::cout << "App: Iniciado com o ConcreteCreator1 (Logistica Viaria).\n";
    // 1. Cria o Criador 1
    Creator* creator1 = new ConcreteCreator1();
    ClientCode(*creator1); // O cliente usa sem saber que é o 1
    std::cout << std::endl;

    std::cout << "App: Iniciado com o ConcreteCreator2 (Logistica Maritima).\n";
    // 2. Cria o Criador 2
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2); // O cliente usa sem saber que é o 2

    // Limpeza dos Criadores
    delete creator1;
    delete creator2;

    return 0;
}