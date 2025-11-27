# Padrão Estrutural: Facade (Fachada)

## 1. O Problema
No fluxo de **Cadastro de Usuários (Signup)** do nosso sistema, a lógica estava se tornando excessivamente complexa para ser mantida diretamente na Camada de Apresentação (View).

Para cadastrar um cliente, a `signup_view` precisava assumir responsabilidades demais:
1.  Receber e decodificar o JSON.
2.  Instanciar e validar o `CadastroClienteForm`.
3.  Gerenciar transações de banco de dados (`transaction.atomic`) para garantir integridade.
4.  Criar o registro na tabela de Autenticação (`User`).
5.  Criar o registro na tabela de Perfil (`Cliente`).
6.  Tratar exceções de banco e erros de validação.

Isso gerava o anti-padrão conhecido como **"Fat View"** (View Gorda), resultando em alto acoplamento: se a regra de criação de perfil mudasse, teríamos que alterar o arquivo `views.py`.

## 2. A Solução (Facade)
Utilizamos o padrão **Facade** para criar uma camada de abstração entre a View e as regras de negócio. 

Criamos a classe `CadastroFacade`, que atua como um "orquestrador". Ela encapsula a complexidade dos subsistemas (Forms, Models, Database Transactions) e fornece um método simples: `cadastrar_cliente(dados)`. A View passa a atuar apenas como um controlador HTTP, delegando todo o "trabalho pesado" para a fachada.

## 3. Estrutura do Padrão

A imagem abaixo ilustra o conceito geral do padrão, conforme catalogado pelo Refactoring Guru:

![Diagrama Conceitual do Facade](../images/UML-facade.png)

### Aplicação no Projeto (Mapeamento)

No nosso contexto, os papéis do padrão são desempenhados pelas seguintes classes:

1.  **Cliente (Client):** É a nossa `signup_view`. Ela consome a fachada.
2.  **Fachada (Facade):** É a classe `CadastroFacade`.
3.  **Subsistemas Complexos:**
    * `CadastroClienteForm` (Validação).
    * `UserPersonalizado` e `Cliente` (Persistência/Models).
    * `django.db.transaction` (Controle de Atomicidade).

---

## 4. Análise Crítica: A Teoria vs. Nossa Prática

#### A Definição Clássica
Na literatura (como no Refactoring Guru), o Facade é frequentemente exemplificado como um wrapper para **bibliotecas de terceiros** ou códigos legados complexos (ex: um conversor de vídeo `ffmpeg`).

#### A Nossa Abordagem (Service Layer)
No desenvolvimento Web moderno com Django, adaptamos o Facade para atuar como uma **Camada de Serviço**.
* **Objetivo:** Garantir o princípio de **"Thin Views"** (Views Magras).
* **Benefício:** A Facade centraliza a regra de negócio. Se amanhã precisarmos criar usuários via **Linha de Comando (CLI)** ou via **API Rest Externa**, basta reutilizar a `CadastroFacade`, sem duplicar a lógica que estaria presa na View.

---

## 5. Implementação Técnica (C++ vs Python)

Para este trabalho, realizamos a implementação do padrão em **C++**, focando na estrutura de classes e gerenciamento de memória. 

O caso de uso original (Python/Django), que motivou a escolha deste padrão, foi mantido no repositório como material de estudo e comparação.

### Organização dos Arquivos

| Componente | Implementação do Padrão (C++) | Caso de Uso Real (Python) |
| :--- | :--- | :--- |
| **Localização** | `estrutural/codigo/` | `notes/casos de usos/estrutural signup/` |
| **View/Cliente** | `cliente.hpp` | `cliente.py` |
| **Facade** | `facade.hpp` | `facade.py` |
| **Subsistemas** | `subsistemas.hpp` | `forms.py`, `models.py` |

### Como executar a implementação (C++)

O código C++ possui um `Makefile` configurado para compilação automática.

**Para rodar a partir da raiz do projeto:**
```bash
make -C estrutural/codigo