from django.db import transaction
from .forms import CadastroClienteForm
from .models import UserPersonalizado
from clientes.models import Cliente
import json

class CadastroFacade:
    """
    Facade responsável por orquestrar todo o processo de criação de conta.
    Esconde a complexidade de validar formulário, abrir transação e salvar
    em duas tabelas diferentes (User e Cliente).
    """

    def cadastrar_cliente(self, dados_dict):
        # 1. Instancia o formulário (Validação de Entrada)
        formulario = CadastroClienteForm(dados_dict)

        # 2. Verifica se é válido
        if not formulario.is_valid():
            # Se falhar, processa os erros e devolve
            erros_string = formulario.errors.as_json()
            erros_dict = json.loads(erros_string)
            return {
                'sucesso': False, 
                'erros': erros_dict
            }

        # 3. Se passou, pegamos os dados limpos
        dados_limpos = formulario.cleaned_data

        try:
            # 4. Orquestra a persistência (Transação Atômica)
            # Isso garante que se falhar ao criar o Cliente, ele desfaz a criação do User
            with transaction.atomic():
                
                # Passo A: Cria o Usuário (Auth)
                novo_user = UserPersonalizado.objects.create_user(
                    username=dados_limpos['username'],
                    telefone=dados_limpos['telefone'],
                    email=dados_limpos['email'],
                    password=dados_limpos['password'],
                )

                # Passo B: Cria o Cliente vinculado (Regra de Negócio)
                Cliente.objects.create(fk_user=novo_user)
            
            # Retorna sucesso e o objeto user para a view fazer o login
            return {
                'sucesso': True, 
                'user': novo_user
            }

        except Exception as e:
            # Se der erro de banco ou lógica interna
            return {
                'sucesso': False, 
                'erro_interno': str(e)
            }