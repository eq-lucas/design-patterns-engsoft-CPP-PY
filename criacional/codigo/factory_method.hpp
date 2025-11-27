#ifndef FACTORY_METHOD_HPP
#define FACTORY_METHOD_HPP

#include <iostream>
#include <string>


/**
 * Define a interface comum para todos os objetos que a fábrica pode criar.
 * No exemplo real: Seria a interface "Transporte" com o método "entregar()".
 */
class Product {
public:
    virtual ~Product() {}

    // Método puramente virtual. Todo produto concreto deve implementar.
    virtual std::string Operation() const = 0;
};

/**
 * Implementação Concreta 1.
 * No exemplo real: Seria a classe "Caminhao".
 */
class ConcreteProduct1 : public Product {
public:
    std::string Operation() const override {
        return "{Resultado do Produto Concreto 1 (Ex: Caminhao)}";
    }
};

/**
 * Implementação Concreta 2.
 * No exemplo real: Seria a classe "Navio".
 */
class ConcreteProduct2 : public Product {
public:
    std::string Operation() const override {
        return "{Resultado do Produto Concreto 2 (Ex: Navio)}";
    }
};

/**
 * A classe Creator declara o método fábrica (FactoryMethod).
 * Importante: A responsabilidade principal dela NÃO é criar produtos,
 * mas sim conter a lógica de negócio (SomeOperation) que DEPENDE do produto.
 * * No exemplo real: Seria a classe "Logistica".
 */
class Creator {
public:
    virtual ~Creator() {};

    /**
     * O FACTORY METHOD (Abstrato/Puro)
     * As subclasses (CreatorConcreto) que vão decidir o que retornar aqui.
     */
    virtual Product* FactoryMethod() const = 0;

    /**
     * Lógica de Negócio Central.
     * Note que esta função usa o produto, mas NÃO sabe qual produto é (1 ou 2).
     * Ela trabalha apenas com a interface 'Product'.
     */
    std::string SomeOperation() const {
        // 1. Chama o método fábrica para criar um objeto Produto.
        // O 'this->' garante que chamaremos a versão da subclasse correta.
        Product* product = this->FactoryMethod();
        
        // 2. Usa o produto.
        std::string result = "Creator: O mesmo codigo criador funcionou com -> " + product->Operation();
        
        // 3. Limpeza (O Creator criou para usar, agora descarta).
        delete product;
        
        return result;
    }
};


/**
 * Sobrescreve o método fábrica para retornar um Produto 1.
 * No exemplo real: Seria "LogisticaViaria" criando um "Caminhao".
 */
class ConcreteCreator1 : public Creator {
public:
    Product* FactoryMethod() const override {
        return new ConcreteProduct1();
    }
};

/**
 * Sobrescreve o método fábrica para retornar um Produto 2.
 * No exemplo real: Seria "LogisticaMaritima" criando um "Navio".
 */
class ConcreteCreator2 : public Creator {
public:
    Product* FactoryMethod() const override {
        return new ConcreteProduct2();
    }
};

#endif