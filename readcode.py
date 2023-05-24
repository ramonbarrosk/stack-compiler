import subprocess
import os
import shutil
import re

def substituir_operador_soma(codigo):
    def substituicao(match):
        valor1 = match.group(1)
        valor2 = match.group(2)
        return f'{valor1} + {valor2}'

        # Busca todas as ocorrências do padrão e as substitui
    new_content = re.sub(r'\+\(([^,]+),([^)]+)\)', lambda m: m.group(1).strip()+'+'+m.group(2).strip(),codigo)

    # Retorna o conteúdo transformado
    return new_content


def converter_codigo_pascal():
    # Verificar a data de modificação dos arquivos
    example_file_mod_time = os.path.getmtime('example-file.txt')
    a_pas_mod_time = os.path.getmtime('a.pas')

    # Se example-file.txt for mais recente que a.pas, converter o código novamente
    with open('example-file.txt', 'r') as arquivo_txt:
        codigo = arquivo_txt.read()

        # Realize as substituições necessárias
        codigo = substituir_operador_soma(codigo)
        codigo = codigo.replace('read(', 'readln(')
        codigo = codigo.replace('write(', 'writeln(')

        print(substituir_operador_soma(codigo))
        # Salve o código convertido em um novo arquivo temporário
        with open('a_temp.pas', 'w') as arquivo_pascal:
            arquivo_pascal.write(codigo)

        # Mover o arquivo temporário para substituir o a.pas existente
        shutil.move('a_temp.pas', 'a.pas')

    # Executar o código Pascal atualizado
    executar_codigo_pascal()

def executar_codigo_pascal():
    # Compilar e executar o arquivo a.pas
    result = subprocess.run(['fpc', 'a.pas'], capture_output=True, text=True)
    subprocess.run(['./a'])

    # Filtrar as linhas desejadas
    linhas_saida = result.stdout.split('\n')
    linhas_desejadas = [linha for linha in linhas_saida if linha.startswith(('2', 'doug'))]

    # Imprimir as linhas desejadas
    for linha in linhas_desejadas:
        print(linha)

converter_codigo_pascal()
