#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10
#define MAX_INVENTARIO 5
#define MAX_NOME 30

typedef struct {
    char nome[30];
    int poder;
    int preco;
    char tipo[10]; // "arma", "cura"
} Item;

typedef struct {
    char nome[MAX_NOME];
    char classe[MAX_NOME];
    int nivel;
    int xp;
    int vida;
    int vida_max;
    int mana;
    int mana_max;
    int ataque;
    int defesa;
    int ouro;
    Item inventario[MAX_INVENTARIO];
    int num_itens;
} Jogador;

typedef struct {
    char nome[30];
    int vida;
    int ataque;
    int defesa;
    int comportamento; // 0: normal, 1: agressivo, 2: curandeiro
} Inimigo;

// Protótipos
void salvar_jogo(Jogador *jogador);
int carregar_jogo(Jogador *jogador);
void criar_personagem(Jogador *jogador);
void mostrar_status(Jogador *j);
void loja(Jogador *j);
void mapa(Jogador *j);
void batalha(Jogador *j, Inimigo *i);
void usar_habilidade(Jogador *j, Inimigo *i);
void mostrar_inventario(Jogador *j);
void adicionar_item(Jogador *j, Item item);

// Lista de itens da loja
Item loja_itens[] = {
    {"Espada de Ferro", 10, 20, "arma"},
    {"Poção de Cura", 30, 15, "cura"},
    {"Cajado Mágico", 15, 25, "arma"},
    {"Poção de Mana", 20, 15, "cura"}
};
int total_itens_loja = 4;

// Funções
void salvar_jogo(Jogador *j) {
    FILE *f = fopen("save.txt", "w");
    if (!f) return;
    fprintf(f, "%s\n%s\n%d %d %d %d %d %d %d %d %d\n", j->nome, j->classe, j->nivel, j->xp, j->vida, j->vida_max, j->mana, j->mana_max, j->ataque, j->defesa, j->ouro);
    fprintf(f, "%d\n", j->num_itens);
    for (int i = 0; i < j->num_itens; i++) {
        fprintf(f, "%s %d %d %s\n", j->inventario[i].nome, j->inventario[i].poder, j->inventario[i].preco, j->inventario[i].tipo);
    }
    fclose(f);
    printf("Jogo salvo!\n");
}

int carregar_jogo(Jogador *j) {
    FILE *f = fopen("save.txt", "r");
    if (!f) return 0;
    fscanf(f, " %[^\n]", j->nome);
    fscanf(f, " %[^\n]", j->classe);
    fscanf(f, "%d %d %d %d %d %d %d %d %d", &j->nivel, &j->xp, &j->vida, &j->vida_max, &j->mana, &j->mana_max, &j->ataque, &j->defesa, &j->ouro);
    fscanf(f, "%d", &j->num_itens);
    for (int i = 0; i < j->num_itens; i++) {
        fscanf(f, " %[^\n] %d %d %s", j->inventario[i].nome, &j->inventario[i].poder, &j->inventario[i].preco, j->inventario[i].tipo);
    }
    fclose(f);
    printf("Jogo carregado!\n");
    return 1;
}

void criar_personagem(Jogador *j) {
    printf("Nome do personagem: ");
    scanf(" %[^\n]", j->nome);

    printf("Escolha sua classe:\n1. Guerreiro\n2. Mago\n3. Arqueiro\n> ");
    int esc;
    scanf("%d", &esc);

    j->nivel = 1;
    j->xp = 0;
    j->ouro = 50;
    j->num_itens = 0;

    switch (esc) {
        case 1:
            strcpy(j->classe, "Guerreiro");
            j->vida = j->vida_max = 120;
            j->mana = j->mana_max = 30;
            j->ataque = 15;
            j->defesa = 10;
            break;
        case 2:
            strcpy(j->classe, "Mago");
            j->vida = j->vida_max = 80;
            j->mana = j->mana_max = 80;
            j->ataque = 20;
            j->defesa = 5;
            break;
        case 3:
            strcpy(j->classe, "Arqueiro");
            j->vida = j->vida_max = 100;
            j->mana = j->mana_max = 50;
            j->ataque = 17;
            j->defesa = 7;
            break;
    }
}

void mostrar_status(Jogador *j) {
    printf("\n[%s - %s] Nível: %d XP: %d\n", j->nome, j->classe, j->nivel, j->xp);
    printf("HP: %d/%d | MP: %d/%d | Atk: %d | Def: %d | Ouro: %d\n", j->vida, j->vida_max, j->mana, j->mana_max, j->ataque, j->defesa, j->ouro);
}

void mostrar_inventario(Jogador *j) {
    printf("\nInventário:\n");
    for (int i = 0; i < j->num_itens; i++) {
        printf("- %s (%s)\n", j->inventario[i].nome, j->inventario[i].tipo);
    }
}

void adicionar_item(Jogador *j, Item item) {
    if (j->num_itens < MAX_INVENTARIO) {
        j->inventario[j->num_itens++] = item;
    }
}

void loja(Jogador *j) {
    printf("\n--- Loja ---\n");
    for (int i = 0; i < total_itens_loja; i++) {
        printf("%d. %s - %d ouro\n", i + 1, loja_itens[i].nome, loja_itens[i].preco);
    }
    printf("Escolha um item para comprar (0 para sair): ");
    int op;
    scanf("%d", &op);
    if (op > 0 && op <= total_itens_loja) {
        Item escolhido = loja_itens[op - 1];
        if (j->ouro >= escolhido.preco) {
            j->ouro -= escolhido.preco;
            adicionar_item(j, escolhido);
            printf("Comprou %s!\n", escolhido.nome);
        } else {
            printf("Ouro insuficiente.\n");
        }
    }
}

void usar_habilidade(Jogador *j, Inimigo *i) {
    if (strcmp(j->classe, "Guerreiro") == 0 && j->mana >= 10) {
        printf("Você usa Golpe Duplo!\n");
        i->vida -= j->ataque * 2;
        j->mana -= 10;
    } else if (strcmp(j->classe, "Mago") == 0 && j->mana >= 20) {
        printf("Você lança Bola de Fogo!\n");
        i->vida -= j->ataque * 3;
        j->mana -= 20;
    } else if (strcmp(j->classe, "Arqueiro") == 0 && j->mana >= 15) {
        printf("Você dispara Flecha Precisa!\n");
        i->vida -= j->ataque * 2 + 5;
        j->mana -= 15;
    } else {
        printf("Mana insuficiente ou classe inválida!\n");
    }
}

void batalha(Jogador *j, Inimigo *i) {
    printf("\n Encontro com %s!\n", i->nome);
    while (j->vida > 0 && i->vida > 0) {
        mostrar_status(j);
        printf("1. Atacar\n2. Habilidade\n3. Inventário\n4. Fugir\n> ");
        int esc;
        scanf("%d", &esc);

        if (esc == 1) {
            int dano = j->ataque - i->defesa;
            if (dano < 0) dano = 0;
            i->vida -= dano;
            printf("Você causou %d de dano!\n", dano);
        } else if (esc == 2) {
            usar_habilidade(j, i);
        } else if (esc == 3) {
            mostrar_inventario(j);
        } else if (esc == 4) {
            printf("Você fugiu!\n");
            return;
        }

        if (i->vida > 0) {
            int dano = i->ataque - j->defesa;
            if (dano < 0) dano = 0;
            j->vida -= dano;
            printf("O %s atacou! Você perdeu %d de vida.\n", i->nome, dano);
        }
    }

    if (j->vida > 0) {
        printf("Você venceu %s!\n", i->nome);
        j->xp += 30;
        j->ouro += 20;
        if (j->xp >= j->nivel * 100) {
            j->nivel++;
            j->vida_max += 20;
            j->mana_max += 10;
            j->ataque += 5;
            j->defesa += 2;
            j->xp = 0;
            printf("Você subiu para o nível %d!\n", j->nivel);
        }
    } else {
        printf("Você foi derrotado...\n");
    }
}

void mapa(Jogador *j) {
    while (1) {
        printf("\n--- Mapa ---\n1. Floresta\n2. Loja\n3. Salvar\n4. Sair\n> ");
        int op;
        scanf("%d", &op);

        if (op == 1) {
            Inimigo inimigo = {"Goblin", 60, 10, 5, rand() % 3};
            batalha(j, &inimigo);
            if (j->vida <= 0) return;
        } else if (op == 2) {
            loja(j);
        } else if (op == 3) {
            salvar_jogo(j);
        } else if (op == 4) {
            break;
        }
    }
}

int main() {
    srand(time(NULL));
    Jogador jogador;
    int carregado = carregar_jogo(&jogador);
    if (!carregado) criar_personagem(&jogador);

    mapa(&jogador);

    printf("Obrigado por jogar, %s!\n", jogador.nome);
    return 0;
}
