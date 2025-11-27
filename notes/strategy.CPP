#include <iostream>
#include <string>
#include <algorithm> // Para usar o sort (ordenar)
#include <functional> // Para usar o greater (maior que)

using namespace std;

// =================================================================
// 1. A INTERFACE (O "Contrato")
// =================================================================
class Strategy {
public:
    // -------------------------------------------------------------
    // POR QUE 'VIRTUAL' NO DESTRUTOR?
    // -------------------------------------------------------------
    // Se você fizer: Strategy* p = new EstrategiaConcretaA();
    // E depois:      delete p;
    // Sem o 'virtual', o compilador só roda o destrutor de 'Strategy' e para.
    // A parte da memória exclusiva da 'EstrategiaConcretaA' vazaria.
    // O 'virtual' diz: "Antes de destruir, verifique se sou um objeto filho".
    virtual ~Strategy() {}

    // -------------------------------------------------------------
    // POR QUE 'VIRTUAL' NA FUNÇÃO?
    // -------------------------------------------------------------
    // 'virtual': Diz "Não execute essa função vazia. Procure a versão do Filho".
    // '= 0': Diz "Essa função é PURA (abstrata). Eu (pai) não tenho código
    // para ela. O filho É OBRIGADO a escrever esse código."
    // 'const': Diz "Prometo que essa função não muda variáveis da classe".
    virtual string fazerAlgoritmo(string dados) const = 0;
};


// =================================================================
// 2. ESTRATÉGIA A (Concreta)
// =================================================================
// ": public Strategy" -> Herança. "Eu SOU UM tipo de Strategy".
// Isso permite que o ponteiro Strategy* aponte para mim.
class EstrategiaConcretaA : public Strategy {
public:
    // 'override': Avisa o compilador "Estou substituindo a função do meu pai".
    // Se você errar o nome da função, o compilador te avisa graças a isso.
    string fazerAlgoritmo(string dados) const override {
        // Lógica específica A: Ordenar Crescente (A -> Z)
        sort(dados.begin(), dados.end());
        return dados;
    }
};


// =================================================================
// 3. ESTRATÉGIA B (Concreta)
// =================================================================
class EstrategiaConcretaB : public Strategy {
public:
    string fazerAlgoritmo(string dados) const override {
        // Lógica específica B: Ordenar Decrescente (Z -> A)
        sort(dados.begin(), dados.end(), greater<char>());
        return dados;
    }
};


// =================================================================
// 4. O CONTEXTO (Quem usa a estratégia)
// =================================================================
class Contexto {
private:
    // -------------------------------------------------------------
    // A MÁGICA DO POLIMORFISMO
    // -------------------------------------------------------------
    // Eu não guardo "EstrategiaConcretaA" nem "B".
    // Eu guardo um ponteiro para o PAI (Strategy).
    // Como A e B são filhas, esse ponteiro aceita endereço de qualquer uma.
    Strategy* estrategia_; 

public:
    // Construtor: Recebe o endereço de qualquer filho de Strategy
    Contexto(Strategy* estrategiaInicial) {
        this->estrategia_ = estrategiaInicial;
    }

    // Destrutor: O Contexto é dono da estratégia, ele limpa a bagunça.
    ~Contexto() {
        if (this->estrategia_ != nullptr) {
            // Chama o destrutor. Graças ao 'virtual ~Strategy' lá em cima,
            // ele vai limpar tudo corretamente, seja A ou B.
            delete this->estrategia_;
        }
    }

    // Setter: Troca a "peça" (estratégia) com o carro andando.
    void setEstrategia(Strategy* novaEstrategia) {
        // 1. Apaga a velha da memória (senão vaza memória RAM)
        if (this->estrategia_ != nullptr) {
            delete this->estrategia_;
        }
        // 2. Guarda o endereço da nova
        this->estrategia_ = novaEstrategia;
    }

    // O Trabalho Real
    void executarLogica() const {
        if (this->estrategia_) {
            cout << "Contexto: Ordenando..." << endl;
            
            // O PULO DO GATO (Dynamic Dispatch):
            // O compilador vê: "Ah, é um ponteiro Strategy, vou chamar Strategy::fazerAlgoritmo".
            // Mas ele vê a palavra 'virtual'.
            // Então ele muda para: "Vou olhar na memória quem está aqui. Ah, é uma ConcretaA!
            // Então chame ConcretaA::fazerAlgoritmo".
            string resultado = this->estrategia_->fazerAlgoritmo("banana");
            
            cout << "Resultado: " << resultado << endl;
        }
    }
};


// =================================================================
// 5. MAIN (Teste)
// =================================================================
int main() {
    
    // Passo 1: Criar o objeto Filho (A) na memória.
    // O 'new' devolve o endereço de memória (Ex: 0x500).
    // O ponteiro 'p' (tipo Pai) guarda esse endereço 0x500.
    Strategy* p = new EstrategiaConcretaA();

    // Passo 2: O Contexto recebe e guarda esse ponteiro.
    Contexto* contexto = new Contexto(p);

    cout << "--- Usando A ---" << endl;
    contexto->executarLogica(); // Chama o código da A

    cout << "\n--- Trocando para B ---" << endl;
    
    // Passo 3: Trocamos o ponteiro interno para apontar para um novo objeto B.
    // O setter deleta o A antigo e guarda o B novo.
    contexto->setEstrategia(new EstrategiaConcretaB());
    
    contexto->executarLogica(); // Chama o código da B

    // Passo 4: Limpeza
    delete contexto; // Destroi contexto -> Destroi Strategy B

    return 0;
}