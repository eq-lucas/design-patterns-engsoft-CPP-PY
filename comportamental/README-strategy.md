# Padrão Comportamental: Strategy

## 1. O Problema
No sistema de autenticação de usuários, precisávamos permitir o login através de múltiplos identificadores: **E-mail**, **Telefone** ou **Nome de Usuário**. 

A abordagem inicial e ingênua seria criar uma série de condicionais complexas (`if/else`) dentro da View ou do Backend de autenticação para verificar o formato do texto digitado e decidir qual campo do banco de dados consultar. Isso viola o princípio *Open/Closed* (Aberto para extensão, fechado para modificação), pois para adicionar um novo método (ex: CPF), teríamos que alterar a lógica principal de login.

## 2. A Solução (Strategy)
Utilizamos o padrão **Strategy** para encapsular a lógica de validação e busca de usuário em classes separadas. O "Contexto" (nosso backend de autenticação) não precisa saber os detalhes de como validar um e-mail ou telefone; ele apenas percorre uma lista de estratégias disponíveis até encontrar uma que aceite o identificador fornecido.

## 3. Estrutura do Padrão

A imagem abaixo ilustra o conceito geral do padrão, conforme catalogado pelo Refactoring Guru:

![Diagrama Conceitual do Strategy](UML-strategy.png)

### Como funciona nesta implementação (Diferenças Práticas)

Diferente do diagrama clássico onde o cliente injeta a estratégia, nesta implementação o **Contexto é autossuficiente**.

1.  **Instanciação Interna:** A classe `Contexto` (em `contexto.py`) instancia todas as estratégias possíveis (`EmailStrategy`, `TelefoneStrategy`, `UsernameStrategy`) dentro de uma lista própria.
2.  **Seleção Automática (Loop):**
    * Quando a view solicita a autenticação, o Contexto itera sobre essa lista usando um **loop `for`**.
    * Para cada estratégia, ele chama o método `validar(identifier)` (parte da interface comum).
    * Se a estratégia retorna `True` (o formato é válido), o loop é interrompido imediatamente.
3.  **Execução:**
    * A estratégia escolhida executa o método `buscar_suposto_usuario`, que vai ao banco de dados verificar se o identificador e a senha correspondem.
    * Se encontrar, devolve o objeto `User` para a View realizar o login e criar o cookie de sessão.
    * Se passar por todas as estratégias e nenhuma aceitar ou encontrar o usuário, retorna erro.

Essa abordagem retira a responsabilidade do cliente instanciar uma estratégia (sabendo sua existência) e setar no contexto. Ao invés de forçar o cliente a escolher, o sistema faz o teste na lista interna, prosseguindo para a busca do suposto usuário. Isso retira da View a responsabilidade de saber "qual tipo de dado o usuário digitou", centralizando essa inteligência nas estratégias.


### 4. Análise Crítica: A Teoria vs. Nossa Prática

Para adaptar o padrão Strategy ao mundo real de uma aplicação Web moderna, fizemos uma mudança arquitetural importante em relação ao exemplo clássico.

#### O Jeito "Clássico" (Refactoring Guru)
Na teoria acadêmica, o **Cliente** (quem usa o código) precisa decidir qual estratégia usar *antes* de chamar o contexto.
* **No código:** `contexto.strategy = EmailStrategy()`
* **Na vida real (UX Ruim):** Isso exigiria que tivéssemos um botão ou checkbox no frontend onde o usuário fosse obrigado a marcar: *"Vou logar com E-mail"* ou *"Vou logar com Telefone"*.

#### A Nossa Abordagem 
Priorizamos a **Experiência do Usuário (UX)**. O usuário não deve ser obrigado a categorizar o dado; o sistema deve ser inteligente o suficiente para descobrir sozinho.

Por isso, movemos a decisão de "qual estratégia usar" do **Cliente** para dentro do **Contexto**.

* **No nosso código (`contexto.py`):**
    ```python
    # Em vez de receber a estratégia pronta, nós a descobrimos:
    for strategy in self.strategies:
        if strategy.validar(identifier):  # <- O "Cérebro" da adaptação
            return strategy.buscar_suposto_usuario(identifier)
    ```

**Conclusão Técnica:**
Enquanto o Strategy clássico usa **Injeção de Dependência** (o cliente injeta a estratégia), nossa implementação usa **Descoberta de Estratégia (Strategy Discovery)** baseada no formato do dado de entrada. Isso mantém o desacoplamento do código (classes separadas) mas adiciona uma camada de inteligência automática.