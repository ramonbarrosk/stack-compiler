import subprocess
import os
import shutil

def converter_codigo_pascal():
    # Verificar a data de modificação dos arquivos
    example_file_mod_time = os.path.getmtime('example-file.txt')
    a_pas_mod_time = os.path.getmtime('a.pas')

    # Se example-file.txt for mais recente que a.pas, converter o código novamente
    if example_file_mod_time > a_pas_mod_time:
        with open('example-file.txt', 'r') as arquivo_txt:
            codigo = arquivo_txt.read()

            # Realize as substituições necessárias
            codigo = codigo.replace('read(', 'readln(')
            codigo = codigo.replace('write(', 'writeln(')

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
