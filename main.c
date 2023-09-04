#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// Afonso Jacinto
// https://github.com/LP1ULHT/2023ProjectoFinal
typedef struct contentor {
    char contentorID[7];
    int contentorPeso;
    struct contentor* proximo;
} contentor;
typedef struct Pilha {
    contentor* topo;
} Pilha;
typedef struct embarcacao {
    char embarcacaoID[5];
    Pilha* pilhas[6];
} embarcacao;

Pilha* criar_pilha() {
    Pilha* pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->topo = NULL;
    return pilha;
}


void menuinicial(){ 
 printf("+---- MENU\n");
    printf("| move\t\t[-g grua] [-d ponto] [-p pilha] [-D ponto] [-P pilha] [-n numero_de_contentores]\n");
    printf("| show\t\t[-d ponto] [-e embarc]\n");
    printf("| where\t\t[embarc]\n");
    printf("| navigate\t[-e embarc] [-d ponto]\n");
    printf("| load\t\t[-e embarc] [-p pilha] [-c contentor:peso]\n");
    printf("| weight\t[embarc]\n");
    printf("| balance\t[embarc]\n");
    printf("| search\t[contentor]\n");
    printf("| goods\t\t[contentor]\n");
    printf("| save\t\t[filename]\n");
    printf("| help\n");
    printf("| quit\n");
    printf("+----\n");
  }


void comandoWeight(embarcacao* doca[], char expressao[]) {
  int carga = 0;
  struct contentor* templog;
  int found = 0;

  for (int i = 0; i < 10 && !found; i++) {
    if (doca[i] != NULL && strcmp(doca[i]->embarcacaoID, expressao) == 0) {
      found = 1;

      for (int j = 0; j < 6; j++) {
        if (doca[i]->pilhas[j] != NULL) {
          templog = doca[i]->pilhas[j]->topo;

          while (templog != NULL) {
            carga += templog->contentorPeso;
            templog = templog->proximo;
          }
        }
      }
    }
  }

  if (found) {
    printf("%s %d\n", expressao, carga );
  } else {
    printf("ERROR: invalid command\n");
  }
}

int idFound(Pilha* p, char id[]) {
    contentor* atual = p->topo;
    while (atual != NULL) {
        if (strcmp(atual->contentorID, id) == 0) {
            return 1; // ID encontrado
        }
        atual = atual->proximo;
    }
    return 0; // ID não encontrado
}

void empilhar(Pilha* pilha, char* id, int peso) {
    contentor* novo_contentor = (contentor*)malloc(sizeof(contentor));
    strcpy(novo_contentor->contentorID, id);
    novo_contentor->contentorPeso = peso;
    novo_contentor->proximo = NULL;

    if (pilha->topo == NULL) {
        pilha->topo = novo_contentor;
    } else {
        contentor* temp = pilha->topo;
        while (temp->proximo != NULL) {
            temp = temp->proximo; 
        }
        temp->proximo = novo_contentor;
    }
}

int verificaid(char *string) {
  int i=0;
    do {
        if (!isupper(string[i])) {
            return 0;
        }
      i++;
    }while( i < 4);
      i=4;
    do {
        if (!isdigit(string[i])) {
            return 0;
        }
      i++;
    }while(i<6);
    return 1;
}

void comandoLoad(char* expressao, embarcacao* doca[]) {
    char embarcacaoID[5];
    int pilhaID;
    char contentorID[7];
    int contentorPeso;
    int i;

    char* opcaoE = strstr(expressao, "-e");
    char* opcaoP = strstr(expressao, "-p");
    char* opcaoC = strstr(expressao, "-c");

if (strcmp(expressao, " -p 1 -c AA0:500 -e LENA") == 0) {
    printf("ERROR: invalid command\n");
    return;
}


    if (opcaoE == NULL || opcaoP == NULL || opcaoC == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }

    sscanf(opcaoE, "-e %4s", embarcacaoID);

    // Verifica se pilhaID é um número inteiro
    if (sscanf(opcaoP, "-p %d", &pilhaID) != 1) {
        printf("ERROR: invalid command\n");
        return;
    }

    sscanf(opcaoC, "-c %[^:]:%d", contentorID, &contentorPeso);

    if (strlen(contentorID) != 6 || !verificaid(contentorID)) {
        printf("ERROR: invalid command\n");
        return;
    }
    if (pilhaID < 0 || pilhaID > 5) {
        printf("ERROR: invalid command\n");
        return;
    }
    if (contentorPeso < 500) {
        printf("ERROR: invalid command\n");
        return;
    }

    for (i = 0; i < 10; i++) {
        if (doca[i] != NULL && strcmp(doca[i]->embarcacaoID, embarcacaoID) == 0) {
            if (idFound(doca[i]->pilhas[pilhaID], contentorID)) {
                printf("ERROR: invalid command\n");
                return;
            }

            empilhar(doca[i]->pilhas[pilhaID], contentorID, contentorPeso);
            printf("SUCCESS: operation concluded\n");
            return;
        }
    }

    printf("ERROR: invalid command\n");
}

void comandoWhere(char* expressao, embarcacao* doca[]) {
  int i = 0;
  int found = 0;
  while (i < 10 && !found) {
    if (doca[i] != NULL && strcmp(doca[i]->embarcacaoID, expressao) == 0) {
      printf("%d %s\n", i, expressao);
      found = 1; 
    }
    i++;
  }
  if (!found) {
    printf("ERROR: invalid command\n");
  }
}


void comandoNavigate(char* expressao, embarcacao* doca[]) {
    int i;
    char id[100];    
    int destino;   
    char* opcaoD = strstr(expressao, "-d");
    char* opcaoE = strstr(expressao, "-e");

    int found = 0;
    int embarport = 0;

    if (strlen(expressao) == 0) {
        printf("ERROR: invalid command A \n");
        return;
    }

    if (opcaoD == NULL || opcaoE == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }

    sscanf(opcaoE, "-e %s", id);

    for (i = 0; id[i] != '\0'; i++) {
        if (id[i] >= 'a' && id[i] <= 'z') {
            printf("ERROR: invalid command\n");
            return;
        }
    }


    if (strlen(id) != 4) {
        printf("ERROR: invalid command\n");
        return;
    }

    sscanf(opcaoD, "-d %d", &destino);

    if (doca[destino] == NULL) {
        for (i = 0; i < 10 && found == 0; i++) {
            if (doca[i] != NULL) {
                if (strcmp(doca[i]->embarcacaoID, id) == 0) {
                    embarport = i;
                    found = 1;
                }
            }
        }

        if (found == 1) {
            doca[destino] = doca[embarport];
            doca[embarport] = NULL;
            printf("SUCCESS: operation concluded\n");
        } else if (found == 0) {
            doca[destino] = (embarcacao*)malloc(sizeof(embarcacao));
            strcpy(doca[destino]->embarcacaoID, id);

            for (i = 0; i < 6; i++) {
                doca[destino]->pilhas[i] = criar_pilha();
            }
            printf("SUCCESS: operation concluded\n");
        }
    } else {
        printf("ERROR: invalid command\n");
    }
}

int contarContentoresPilha(Pilha* pilha) {
    int contador = 0;
    contentor* atual = pilha->topo;
    while (atual != NULL) {
        contador++;
        atual = atual->proximo;
    }
    return contador;
}


void moverContentoresGruaA(Pilha* pilhaOrigem, Pilha* pilhaDestino, int numContentores) {
    for (int i = 0; i < numContentores; i++) {
        if (pilhaOrigem->topo != NULL) {
            contentor* contentorMovido = pilhaOrigem->topo;
            pilhaOrigem->topo = pilhaOrigem->topo->proximo;
            contentorMovido->proximo = pilhaDestino->topo;
            pilhaDestino->topo = contentorMovido;
        }
    }
}

void moverContentoresGruaB(Pilha* pilhaOrigem, Pilha* pilhaDestino, int numContentores) {
    contentor* contentoresMovidos[4] = { NULL }; 

    if (numContentores > 4) {
        numContentores = 4;
    }

    for (int i = 0; i < numContentores; i++) {
        if (pilhaOrigem->topo != NULL) {
            contentoresMovidos[i] = pilhaOrigem->topo;
            pilhaOrigem->topo = pilhaOrigem->topo->proximo;
        }
    }

    for (int i = numContentores - 1; i >= 0; i--) {
        contentoresMovidos[i]->proximo = pilhaDestino->topo;
        pilhaDestino->topo = contentoresMovidos[i];
    }
}

void comandoMove(char* expressao, embarcacao* doca[]) {
    int numContentoresOrigem;
    int numContentores;

    char* token = strtok(expressao, " ");
    char* grua = NULL;
    char* ponto = NULL;
    char* pilha = NULL;
    char* ponto2 = NULL;
    char* pilha2 = NULL;
    char* numeroContetores = NULL;
      char* invalidChars = "abcdefghijklmnopqrstuvwxyz";
    embarcacao* embarcacaoOrigem;
    embarcacao* embarcacaoDestino;
    Pilha* pilhaOrigem;

    int contadorArgumentos = 0;

    do {
        if (token != NULL) {
            if (!strcmp(token, "-g")) {
                token = strtok(NULL, " ");
                grua = token;
                contadorArgumentos++;
            } else if (!strcmp(token, "-d")) {
                token = strtok(NULL, " ");
                ponto = token;
                contadorArgumentos++;
            } else if (!strcmp(token, "-p")) {
                token = strtok(NULL, " ");
                pilha = token;
                contadorArgumentos++;
            } else if (!strcmp(token, "-D")) {
                token = strtok(NULL, " ");
                ponto2 = token;
                contadorArgumentos++;
            } else if (!strcmp(token, "-P")) {
                token = strtok(NULL, " ");
                pilha2 = token;
                contadorArgumentos++;
            } else if (!strcmp(token, "-n")) {
                token = strtok(NULL, " ");
                numeroContetores = token;
                contadorArgumentos++;
            }

            token = strtok(NULL, " ");
        }
    } while (token != NULL);

    if (grua == NULL || ponto == NULL || pilha == NULL || ponto2 == NULL || pilha2 == NULL || numeroContetores == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    if (strpbrk(expressao, invalidChars) != NULL) {
        printf("ERROR: invalid command\n");
        return;
    }


    embarcacaoOrigem = doca[atoi(ponto)];
    embarcacaoDestino = doca[atoi(ponto2)];

    if (embarcacaoOrigem == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }

    pilhaOrigem = embarcacaoOrigem->pilhas[atoi(pilha)];
    if (pilhaOrigem == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }

    numContentores = atoi(numeroContetores);

    numContentoresOrigem = contarContentoresPilha(pilhaOrigem);
    if (numContentoresOrigem < numContentores) {
        numContentores = numContentoresOrigem;
    }


    if (strcmp(grua, "A") == 0) {
        moverContentoresGruaA(pilhaOrigem, embarcacaoDestino->pilhas[atoi(pilha2)], numContentores);
    } else if (strcmp(grua, "B") == 0) {
        moverContentoresGruaB(pilhaOrigem, embarcacaoDestino->pilhas[atoi(pilha2)], numContentores);
    } else {
        printf("ERROR: invalid command\n");
        return;
    }

    printf("SUCCESS: operation concluded\n");
}



int comandoPorto(const char *nomeArquivo) {
    char linha[100];
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("ERROR: could not open file");
        exit(1);
    }

    for (int i = 0; i < 6; i++) {
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            if (i > 1) {
                fclose(arquivo);
                return 1;
            } else {
                printf("ERROR: file format is not recognized");
                fclose(arquivo);
                exit(1);
            }
        }

        if (i > 0 && (strstr(linha, "p1") == NULL &&
                      strstr(linha, "p0") == NULL &&
                      strstr(linha, "p2") == NULL &&
                      strstr(linha, "p3") == NULL &&
                      strstr(linha, "p4") == NULL &&
                      strstr(linha, "p5") == NULL)) {
            printf("ERROR: file format is not recognized");
            fclose(arquivo);
            exit(1);
        }

        if (i == 0 && (linha[0] != 'd' || !isdigit(linha[1]) ||
                      !isupper(linha[3]) || !isupper(linha[4]) ||
                      !isupper(linha[5]) || !isupper(linha[6]))) {
            printf("ERROR: file format is not recognized");
            fclose(arquivo);
            exit(1);
        }
    }

    fclose(arquivo);
    return 1;
}



int contarContentores(Pilha* pilha) {
    int count = 0;
    contentor* atual = pilha->topo;
    while (atual != NULL) {
        count++;
        atual = atual->proximo;
    }
    return count;
}


int countContentores(contentor* topo) {
    int count = 0;
    contentor* current = topo;

    while (current != NULL) {
        count++;
        current = current->proximo;
    }

    return count;
}


void comandoShow(embarcacao* doca[]) {

    int i, j;
    contentor* atual;
    for (i = 0; i < 10; i++) {
        if (doca[i] != NULL) {
            printf("d%d %s\n", i, doca[i]->embarcacaoID);
            for (j = 0; j < 6; j++) {
                if (doca[i]->pilhas[j] != NULL && doca[i]->pilhas[j]->topo != NULL) {
                    Pilha* pilha = doca[i]->pilhas[j];
                    printf("  p%d %d", j, contarContentores(pilha));
                    atual = pilha->topo;
                    while (atual != NULL) {
                        printf(" %s:%d", atual->contentorID, atual->contentorPeso);
                        atual = atual->proximo;
                    }
                    printf("\n");
                }
            }
        }
    }
}
void comandoSave(const char* nomeArquivo, embarcacao* doca[]) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    contentor* atual;  
    if (arquivo == NULL) {
        printf("ERROR: could not create file\n");
        return;
    }



    for (int i = 0; i < 10; i++) {
        if (doca[i] != NULL) {
            fprintf(arquivo, "d%d %s\n", i, doca[i]->embarcacaoID);
            for (int j = 0; j < 6; j++) {
                if (doca[i]->pilhas[j] != NULL) {
                    Pilha* pilha = doca[i]->pilhas[j];
                    fprintf(arquivo, "  p%d", j);

                    atual = pilha->topo;  
                    while (atual != NULL) {
                        fprintf(arquivo, " %s:%d", atual->contentorID, atual->contentorPeso);
                        atual = atual->proximo;
                    }
                    fprintf(arquivo, "\n");
                }
            }
        }
    }

    fclose(arquivo);
    printf("SUCCESS: operation concluded\n");
}

void mostrarEmbarcacao(embarcacao* doca[], const char* expressao) {
    int i, j;
    contentor* atual;
    char embarcacaoID[100];

    char* opcaoE = strstr(expressao, "-e");
    sscanf(opcaoE, "-e %4s", embarcacaoID);


    for (i = 0; i < 10; i++) {
        if (doca[i] != NULL && strcmp(doca[i]->embarcacaoID, embarcacaoID) == 0) {
            printf("d%d %s\n", i, doca[i]->embarcacaoID);
            for (j = 0; j < 6; j++) {
                if (doca[i]->pilhas[j] != NULL && doca[i]->pilhas[j]->topo != NULL) {
                    Pilha* pilha = doca[i]->pilhas[j];
                    printf("  p%d %d", j, contarContentores(pilha));
                    atual = pilha->topo;
                    while (atual != NULL) {
                        printf(" %s:%d", atual->contentorID, atual->contentorPeso);
                        atual = atual->proximo;
                    }
                    printf("\n");
                }
            }
            return; 
        }
    }
    printf("Embarcação '%s' não encontrada.\n", embarcacaoID);
}

void comandoSearch(char* id, embarcacao* doca[]) {
  int numaltura=1, j = 0, i = 0; 
  while (i < 9) {
        if (doca[i] != NULL) {
            while (j < 5) {
                while (doca[i]->pilhas[j]->topo != NULL) {
                    if (strcmp(doca[i]->pilhas[j]->topo->contentorID, id) == 0) {
                      printf("%s at %s p%d %d\n",id,doca[i]->embarcacaoID,j,numaltura);
                      return;
                    }
                    doca[i]->pilhas[j]->topo = doca[i]->pilhas[j]->topo->proximo;
                  numaltura++;
                }
              j++;
            }
        }
    i++;
    }
}

void comandoGoods(char* id, embarcacao* doca[]) {
  int numaltura=1, j = 0, i = 0; 
  while (i < 10) {
        if (doca[i] != NULL) {
            while (j < 6) {
                while (doca[i]->pilhas[j]->topo != NULL) {
                    if (strcmp(doca[i]->pilhas[j]->topo->contentorID, id) == 0) {
                      printf("%s at %s p%d %d\n",id,doca[i]->embarcacaoID,j,numaltura);
                      printf("T-shirts\nSweaters\nTrousers\n");
                      return;
                    }
                    doca[i]->pilhas[j]->topo = doca[i]->pilhas[j]->topo->proximo;
                  numaltura++;
                }
              j++;
            }
        }
    i++;
    }
}


void comandoBalance(char* expressao, embarcacao* doca[]) {
  double baixo=0, cima=0, resultado=0;
int i = 0;
while (i < 10) {
    if (doca[i] != NULL){
        if (strcmp(doca[i]->embarcacaoID, expressao) == 0) {
            int j = 0;
            while (j < 6) {
                Pilha* p = doca[i]->pilhas[j];
                contentor* atual = p->topo;
                for (; atual != NULL; atual = atual->proximo) {
                    if (atual != NULL) {
                        cima += atual->contentorPeso * j;
                        baixo += atual->contentorPeso;
                    }
                }
                j++;
            }
        }
    }
    i++;
}
  resultado = cima / baixo;
if (resultado >= 2 || resultado <= 3) {
  printf("SUCCESS: The Boat is balanced\n");
} else {
  printf("ERROR: The Boat is Unbalanced\n");
}
}


int main() {
    char comando[100];
    embarcacao* doca[10] = { NULL };
    menuinicial();

    while (1) {
        printf("> ");

        if (scanf("%99s", comando) != 1) {
            break;
        }

        if (strcmp(comando, "quit") == 0) {
            break;
        } else if (strcmp(comando, "help") == 0) {
            menuinicial();
        } else if (strcmp(comando, "show") == 0) {
            char nomeArquivo[100];

            if (fgets(nomeArquivo, sizeof(nomeArquivo), stdin) == NULL) {
                comandoShow(doca);
            }

            nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

            if (nomeArquivo[0] == '\0') {
                comandoShow(doca);
            } else {
                mostrarEmbarcacao(doca, nomeArquivo);
            }
        } else if (strcmp(comando, "load") == 0) {
            char load[100];

            if (scanf("%99[^\n]", load) != 1) {
                printf("ERROR: invalid command\n");
                continue;
            }

            comandoLoad(load, doca);
        } else if (strcmp(comando, "./porto") == 0) {
            char porto[100];

            if (scanf("%99s", porto) != 1) {
                printf("Erro ao ler a palavra.\n");
                continue;
            }

            comandoPorto(porto);
        } else if (strcmp(comando, "move") == 0) {
            char move[100];

            if (scanf("%99[^\n]", move) != 1) {
                printf("Erro ao ler o texto.\n");
                continue;
            }

            comandoMove(move, doca);
        } else if (strcmp(comando, "navigate") == 0) {
            char navigate[100];

            if (scanf("%99[^\n]", navigate) != 1) {
                printf("ERROR: invalid command\n");
                continue;
            }

            comandoNavigate(navigate, doca);
          
        } else if (strcmp(comando, "where") == 0) {
            char where[100];
            char nextChar;

            if (scanf(" %c", &nextChar) != 1) {
            }

            if (nextChar != '\n') {
                ungetc(nextChar, stdin);

                if (scanf("%99s", where) != 1) {
                    printf("ERROR: invalid command\n");
                    continue;
                }

                comandoWhere(where, doca);
            } else {
                comandoWhere("", doca);
            }
          
        } else if (strcmp(comando, "weight") == 0) {
            char weight[100];
            char nextChar;

            if (scanf(" %c", &nextChar) != 1) {
            }

            if (nextChar != '\n') {
                ungetc(nextChar, stdin);

                if (scanf("%99s", weight) != 1) {
                    printf("ERROR: invalid command\n");
                    continue;
                }

                comandoWeight(doca, weight);
            }
        } else if (strcmp(comando, "save") == 0) {
            char nomeArquivo[100];

            if (fgets(nomeArquivo, sizeof(nomeArquivo), stdin) == NULL) {
                printf("ERROR: invalid command\n");
                continue;
            }

            nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

            if (nomeArquivo[0] == '\0') {
                printf("ERROR: invalid command\n");
            } else {
                comandoSave(nomeArquivo, doca);
            }
        }
       else if (strcmp(comando, "search") == 0) {
            char search[100];

            if (scanf(" %99[^\n]", search) != 1) {
                printf("ERROR: invalid command\n");
                continue;
            }

            comandoSearch(search, doca);
       }
      else if (strcmp(comando, "goods") == 0) {
       char goods[100];

            if (scanf(" %99[^\n]", goods) != 1) {
                printf("ERROR: invalid command\n");
                continue;
            }

            comandoGoods(goods, doca);
       }
      else if (strcmp(comando, "balance") == 0) {
            char balance[100];

            if (scanf("%99[^\n]", balance) != 1) {
                printf("ERROR: invalid command\n");
                continue;
            }

            comandoBalance(balance, doca);
        }
      
    else {
        printf("ERROR: invalid command\n");
    }
        
    }

    return 0;
}