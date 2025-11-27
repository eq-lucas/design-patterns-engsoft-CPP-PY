from django.shortcuts import render
from django.http import JsonResponse
import json
from django.views.decorators.csrf import ensure_csrf_cookie
from django.contrib.auth import login
import core.constantes as t
from .facade import CadastroFacade  # <--- Importamos nossa nova Facade

#cliente, eh quem consome a facade ( o gerente responsavel pela ordem da chamadas cozinheiro, barman...)
# aqui no caso seria a views.py


@ensure_csrf_cookie
def signup_view(request):

    if request.method == 'GET':
        return render(request, t.CADASTRO) 
    
    try: 
        if request.method == 'POST':
            
            # 1. Prepara os dados (A view lida com HTTP/JSON)
            dicionario_bruto = json.loads(request.body)

            # 2. Chama a Facade (Simples e Direto)
            fachada = CadastroFacade()
            resultado = fachada.cadastrar_cliente(dicionario_bruto)

            # 3. Avalia o resultado da Facade
            if resultado['sucesso']:
                # Sucesso: Realiza login e redireciona
                user = resultado['user']
                login(request, user)
                return JsonResponse({'status': 'ok', 'redirect_url': '/clientes/dashboard/'})
            
            elif 'erros' in resultado:
                # Erro de Validação (Formulário)
                return JsonResponse({'errors': resultado['erros']}, status=400)
            
            else:
                # Erro Interno (Exception capturada na facade)
                return JsonResponse({'error': f"Erro ao processar: {resultado.get('erro_interno')}"}, status=500)

    except json.JSONDecodeError:
        return JsonResponse({'error': 'JSON inválido'}, status=400)
    except Exception as e: 
        return JsonResponse({'error': f'Erro crítico na View: {str(e)}'}, status=500)