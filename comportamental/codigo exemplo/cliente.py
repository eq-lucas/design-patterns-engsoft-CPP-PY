from django.shortcuts import render,redirect
from django.http import JsonResponse
import json
from django.views.decorators.csrf import ensure_csrf_cookie
from django.contrib.auth import authenticate, login

 

@ensure_csrf_cookie
def login_view(request):
    
        
    if request.method == 'GET':
        return render(request,t.LOGIN) 
                                            
                                                 
    if request.method == 'POST':

        try:
            dicionario=json.loads(request.body)
            
            
            identificador_front = dicionario.get('identifier') 
            senha_front = dicionario.get('password')

          
            user = authenticate(request, username=identificador_front,password=senha_front)                                                                                   
                                                                                            
            if user is not None:
                         
                login(request, user) 

                return JsonResponse({'status': 'ok', 'redirect_url': '/clientes/dashboard/'})     

            else:
                
                return JsonResponse({'error': 'Usuário ou senha incorretos.'}, status=400)

        except Exception as e:

            return JsonResponse({'error': f'Erro interno: {str(e)}'}, status=500)
        
