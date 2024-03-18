# Primeiro, vamos criar um exemplo de script Python que faz o que você precisa.
# Este script lerá um arquivo de texto contendo floats separados por novas linhas (\n)
# e em seguida plotará um gráfico de linha com esses números.

# Vamos supor que o arquivo de texto se chame 'data.txt' e esteja localizado no mesmo diretório do script.
import os
import matplotlib.pyplot as plt

# Função para ler os dados do arquivo e converter em lista de floats
def read_data_from_file(file_path):
    with open(file_path, 'r') as file:
        data = file.read().splitlines()  # Lê o arquivo e divide em linhas
        data = [line.split() for line in data]  # Divide cada linha em uma lista de strings separadas por espaço
        data = [[float(num) for num in line] for line in data]  # Converte cada string em float
        data = [[line[0], line[1]] for line in data if line]  # Seleciona apenas as duas primeiras colunas
        data1 = [line[0] for line in data]
        data2 = [line[1] for line in data]
    return data1, data2

# Função para plotar o gráfico
def plot_data(data1, data2):
    plt.title('Tempo para encontrar primos com diferentes threads e controle por mutex')  # Define o título do gráfico
    plt.xlabel('Threads')
    plt.ylabel('Tempo')
    plt.xticks(range(1, len(data1)+1))  # Define os ticks do eixo x como números inteiros de 1 até o tamanho dos dados
    plt.xlim(0, len(data1)+1)  # Define os limites do eixo x de 1 até o tamanho dos dados
    plt.plot(range(1, len(data1)+1), data1, label="Não balanceado")  # Plota os dados
    plt.plot(range(1, len(data2)+1), data2, label="Balanceado")  # Plota os dados
    plt.legend()  # Adiciona a legenda ao gráfico
    
    # Verifica se a pasta 'fig' existe. Se não, cria a pasta.
    if not os.path.exists('fig'):
        os.makedirs('fig')
    
    # Salva o gráfico na pasta 'fig' com o nome 'grafico_linha.png'
    plt.savefig('fig/grafico_linha.png')
    plt.close()  # Fecha a figura após salvar, liberando recursos
    
    print("Gráfico salvo em: fig/grafico_linha.png")

# Caminho para o arquivo de dados
file_path = 'data//times.txt'

# Ler os dados do arquivo
data1, data2 = read_data_from_file(file_path)

# Plotar os dados
plot_data(data1, data2)
# plot_data(data2, " controlando concorrência com o uso de mutex", 2)

# Este script deve ser salvo como um arquivo .py e executado em um ambiente que tenha acesso
# ao matplotlib e seja capaz de exibir gráficos. Certifique-se de que o arquivo 'data.txt' esteja
# no mesmo diretório do script, ou altere o caminho do arquivo conforme necessário.
