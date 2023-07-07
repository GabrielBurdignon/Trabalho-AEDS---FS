/*H**********************************************************************
* FILENAME :        TrabalhoFinal.c
*
* DESCRIPTION :
*     Programa que implementa o jogo Flappy Bird.
*
* PUBLIC FUNCTIONS :
*     void fim_jogo(int pontuacao)
*     int colisao(Passaro passaro, Obstaculo obstaculo)
*
*
* AUTHORS :
*     João Pedro Silva Braga
*     Gabriel Figueiredo Cayres Burdignon
*
* START DATE :  05 Jun 23
*
*H*/

/// Bibliotecas
#include <iostream>                       // Biblioteca de entrada e saída padrão.
#include <cstdlib>                        // Biblioteca que contém funções para gerar números aleatórios.
#include <ctime>                          // Biblioteca que fornece funções relacionadas ao tempo.
#include <allegro5/allegro.h>             // Biblioteca principal do Allegro.
#include <allegro5/allegro_primitives.h>  // Biblioteca que fornece funções para desenho primitivo.
#include <allegro5/allegro_font.h>        // Biblioteca que fornece funções relacionadas a fontes de texto.
#include <allegro5/allegro_ttf.h>         // Biblioteca que fornece funções relacionadas a arquivos de fonte TTF.
#include <allegro5/allegro_audio.h>       // Biblioteca que fornece funções relacionadas a arquivos de audio
#include <allegro5/allegro_acodec.h>      // Biblioteca que fornece funções relacionadas a arquivos de audio

#define LARGURA_TELA 800                  // Define a largura da tela como 800 pixels.
#define ALTURA_TELA 600                   // Define a altura da tela como 600 pixels.
#define TAMANHO_FRESTA 200                // Define o tamanho da fresta como 200 pixels.
#define LARGURA_OBSTACULO 80              // Define a largura do obstáculo como 80 pixels.
#define VELOCIDADE_OBSTACULO 3            // Define a velocidade do obstáculo como 3 pixels por frame.
#define GRAVIDADE 1                       // Define a gravidade como 1 pixel por frame.

using namespace std;                                            // Usa o namespace std para facilitar o uso de elementos padrão do C++.

ALLEGRO_FONT* fonte = nullptr;                                  // Ponteiro para a fonte do texto.
ALLEGRO_SAMPLE* som_music = nullptr;                          // Ponteiro para armazenar o som.

/// Define uma estrutura para representar o pássaro.
struct Passaro {
    float x, y;                                                 // Coordenadas x e y do pássaro.
    float velocidade;                                           // Velocidade do pássaro.
};

/// Define uma estrutura para representar os obstáculos.
struct Obstaculo {
    float x, y;                                                 // Coordenadas x e y do obstáculo.
};


/*******************************************************************
* NAME :            void fim_jogo(int pontuacao)
*
* DESCRIPTION :     Função responsável por exibir a tela de fim de jogo
*
* INPUTS :
*       PARAMETERS:
*           int pontuacao : valor da pontuação
*       GLOBALS :
*           None
*
* OUTPUTS :
*       PARAMETERS:
*           None
*       GLOBALS :
*           None
*       RETURN :
*           Type: void
*
* PROCESS :
*       [1] Limpar a tela e definir a cor de fundo como preto.
*       [2] Desenhar o texto "Game Over" na tela.
*       [3] Desenhar a pontuação na tela.
*       [4] Atualizar o display.
*       [5] Aguardar 2 segundos antes de fechar o display.
*/
void fim_jogo(int pontuacao)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                 // Limpa a tela e define a cor de fundo como preto.
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 10, ALLEGRO_ALIGN_CENTER, "Game Over");             // Desenha o texto "Game Over" na tela.
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 10, ALLEGRO_ALIGN_CENTER, "Score: %d", pontuacao);  // Desenha a pontuação na tela.
    al_flip_display();                                                                                                                      // Atualiza a tela.
    al_rest(2.0);                                                                                                                           // Delay de 2 segundos antes de fechar o display
}


/**************************************************************************
* NAME :            int colisao(Passaro passaro, Obstaculo obstaculo)
*
* DESCRIPTION :     Função responsável por verificar a colisão entre o pássaro e o obstáculo
*
* INPUTS :
*       PARAMETERS:
*           Passaro passaro             Estrutura que representa o pássaro com coordenadas (x, y)
*           Obstaculo obstaculo         Estrutura que representa o obstáculo com coordenadas (x, y)
*
*       GLOBALS :
*           None
*
* OUTPUTS :
*       PARAMETERS:
*           None
*       GLOBALS :
*           None
*       RETURN :
*           Type:   int
*           Values: 0             Não ocorreu colisão
*                   1             Ocorreu colisão
*
* PROCESS :
*       [1] Verifica se o pássaro ultrapassou os limites da tela
*       [3] Verifica se ocorreu colisão entre o pássaro e o obstáculo.
*       [4] Retorna se ocorreu ou não colisão
*/
int colisao(Passaro passaro, Obstaculo obstaculo)
{
    // Verifica se o pássaro ultrapassou os limites da tela.
    if (passaro.y <= 0 || passaro.y >= ALTURA_TELA)
        return 1;                                               // Retorna 1 para indicar colisão.

    // Verifica se ocorreu colisão entre o pássaro e o obstáculo.
    if (passaro.x + 50 >= obstaculo.x && passaro.x <= obstaculo.x + LARGURA_OBSTACULO && (passaro.y <= obstaculo.y || passaro.y + 50 >= obstaculo.y + TAMANHO_FRESTA))
        return 1;                                               // Retorna 1 para indicar colisão.

    return 0;                                                   // Retorna 0 para indicar ausência de colisão.
}

/************************************************************************************
* NAME :            test_colisao
*
* DESCRIPTION :     Realiza testes para verificar o funcionamento correto da função colisao.
*
* INPUTS :
*       None
*
* OUTPUTS :
*       None
*
* PROCESS :
*       [1] Cria um objeto Passaro e um objeto Obstaculo.
*       [2] Testa se o passaro está acima da tela. Espera-se que a função colisao retorne 1.
*       [3] Testa se o passaro está abaixo da tela. Espera-se que a função colisao retorne 1.
*       [4] Testa se o passaro está sem colisão com o obstaculo. Espera-se que a função colisao retorne 0.
*       [5] Testa se o passaro colide com a parte superior do obstaculo. Espera-se que a função colisao retorne 1.
*       [6] Testa se o passaro colide com a parte inferior do obstaculo. Espera-se que a função colisao retorne 1.
*       [7] Exibe a mensagem "Todos os testes passaram!".
*
* NOTES :
*       Esta função assume que já existe a função colisao implementada corretamente.
*       A função assert() é usada para verificar se os resultados esperados são retornados pela função colisao.
*/
void test_colisao()
{
    Passaro passaro;
    Obstaculo obstaculo;

    // Teste: passaro acima da tela
    passaro.y = -10;
    assert(colisao(passaro, obstaculo) == 1);

    // Teste: passaro abaixo da tela
    passaro.y = 600;
    assert(colisao(passaro, obstaculo) == 1);

    // Teste: passaro sem colisao
    passaro.y = 300;
    obstaculo.x = 400;
    obstaculo.y = 200;
    assert(colisao(passaro, obstaculo) == 0);

    // Teste: passaro colide com a parte superior do obstaculo
    passaro.y = 200;
    obstaculo.x = 400;
    obstaculo.y = 150;
    assert(colisao(passaro, obstaculo) == 1);

    // Teste: passaro colide com a parte inferior do obstaculo
    passaro.y = 250;
    obstaculo.x = 400;
    obstaculo.y = 300;
    assert(colisao(passaro, obstaculo) == 1);

    cout << "Todos os testes passaram!" << endl;
}

/************************************************************************************
* NAME :            test_fim_jogo
*
* DESCRIPTION :     Realiza testes para verificar o funcionamento correto da função fim_jogo.
*
* INPUTS :
*       None
*
* OUTPUTS :
*       None
*
* PROCESS :
*       [1] Configuração inicial do Allegro.
*       [2] Inicialização dos addons necessários (primitives, font e ttf).
*       [3] Carrega a fonte "arial.ttf" para uso nos testes.
*       [4] Verifica se a fonte foi carregada corretamente. Caso contrário, exibe uma mensagem de erro.
*       [5] Teste: pontuação igual a 0.
*       [6] Teste: pontuação positiva.
*       [7] Teste: pontuação negativa.
*       [8] Finalizações do Allegro.
*       [9] Liberação da memória alocada para a fonte.
*       [10] Desligamento dos addons do Allegro.
*       [11] Exibe a mensagem "Todos os testes passaram!".
*
* NOTES :
*       Esta função assume que o Allegro já está configurado corretamente.
*       A função fim_jogo() será testada com diferentes valores de pontuação.
*       A função assert() é usada para verificar se a fonte foi carregada corretamente.
*/
void test_fim_jogo()
{
    // Configuração inicial do Allegro
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Carrega a fonte para uso nos testes
    fonte = al_load_ttf_font("arial.ttf", 24, 0);
    assert(fonte != nullptr && "Erro ao carregar a fonte!");

    // Teste: pontuação igual a 0
    fim_jogo(0);

    // Teste: pontuação positiva
    fim_jogo(100);

    // Teste: pontuação negativa
    fim_jogo(-50);

    // Finalizações do Allegro
    al_destroy_font(fonte);
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();

    cout << "Todos os testes passaram!" << endl;
}


/******** Função principal do programa  *******/
int main()
{
    srand(static_cast<unsigned>(time(NULL)));                   // Inicializa o gerador de números aleatórios com base no tempo atual.

    ALLEGRO_DISPLAY* display = nullptr;                         // Ponteiro para a tela do jogo.
    ALLEGRO_EVENT_QUEUE* fila_eventos = nullptr;                // Ponteiro para a fila de eventos.
    ALLEGRO_TIMER* timer = nullptr;                             // Ponteiro para o temporizador.
    ALLEGRO_KEYBOARD_STATE estado_teclado;                      // Variável para armazenar o estado do teclado.

    test_colisao();

    // Inicializa a biblioteca Allegro.
    if (!al_init()) {
        cerr << "Falha ao inicializar o Allegro." << endl;      // Exibe uma mensagem de erro caso a inicialização falhe.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    display = al_create_display(LARGURA_TELA, ALTURA_TELA);     // Cria a tela do jogo com as dimensões especificadas.

    // Verifica se a criação da tela falhou.
    if (!display) {
        cerr << "Falha ao criar a tela." << endl;               // Exibe uma mensagem de erro caso a criação falhe.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    fila_eventos = al_create_event_queue();                     // Cria a fila de eventos.

    // Verifica se a criação da fila de eventos falhou.
    if (!fila_eventos) {
        cerr << "Falha ao criar a fila de eventos." << endl;    // Exibe uma mensagem de erro caso a criação falhe.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    timer = al_create_timer(1.0 / 60);                          // Cria o temporizador com uma frequência de 60 vezes por segundo.

    // Verifica se a criação do temporizador falhou.
    if (!timer) {
        cerr << "Falha ao criar o temporizador." << endl;       // Exibe uma mensagem de erro caso a criação falhe.
        al_destroy_event_queue(fila_eventos);                   // Destroi a fila de eventos.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    al_init_primitives_addon();                                 // Inicializa o addon de desenho primitivo do Allegro.
    al_install_keyboard();                                      // Instala o teclado.
    al_init_font_addon();                                       // Inicializa o addon de fontes do Allegro.
    al_init_ttf_addon();                                        // Inicializa o addon TrueType Font do Allegro.
    al_install_audio();                                         // Instala o módulo de áudio do Allegro.
    al_init_acodec_addon();                                     // Inicializa o addon de decodificação de áudio do Allegro.
    al_reserve_samples(2);                                      // Reserva 1 voz (canal) para reprodução de amostras de áudio.

    fonte = al_create_builtin_font();                           // Cria a fonte padrão do Allegro.

    // Verifica se a criação da fonte falhou.
    if (!fonte) {
        cerr << "Failed to create fonte." << endl;              // Exibe uma mensagem de erro caso a criação falhe.
        al_destroy_timer(timer);                                // Destroi o temporizador.
        al_destroy_event_queue(fila_eventos);                   // Destroi a fila de eventos.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    // Verifica se a instalação do mouse falhou
    if (!al_install_mouse()) {
        cerr << "Falha ao inicializar o mouse." << endl;        // Exibe mensagem de erro
        return -1;                                              // Retorna -1 para indicar erro.
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(display));       // Registra a fonte de eventos da tela na fila de eventos.
    al_register_event_source(fila_eventos, al_get_mouse_event_source());                // Registra a fonte de eventos do mouse de eventos.
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));           // Registra a fonte de eventos do temporizador na fila de eventos.
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());             // Registra a fonte de eventos do teclado na fila de eventos.

    al_clear_to_color(al_map_rgb(0, 0, 0));                     // Preenche a tela com a cor preta.
    al_flip_display();                                          // Atualiza a tela exibindo o conteúdo desenhado.
    al_start_timer(timer);                                      // Inicia o temporizador.

    Passaro passaro;                                            // Declara uma variável do tipo Passaro.
    passaro.x = 100;                                            // Define a posição inicial do pássaro no eixo x.
    passaro.y = ALTURA_TELA / 2;                                // Define a posição inicial do pássaro no eixo y.
    passaro.velocidade = 0;                                     // Define a velocidade inicial do pássaro como zero.

    Obstaculo obstaculo;                                        // Declara uma variável do tipo Obstaculo.
    obstaculo.x = LARGURA_TELA;                                 // Define a posição inicial do obstáculo no eixo x.
    obstaculo.y = rand() % (ALTURA_TELA - TAMANHO_FRESTA);      // Define a posição inicial do obstáculo no eixo y de forma aleatória.

    bool executando = true;                                     // Variável de controle do loop principal do jogo.
    bool gameover = false;                                      // Variável que indica se o jogo terminou.
    int pontuacao = 0;                                          // Variável para armazenar a pontuação do jogador.
    bool iniciar_jogo = false;                                  // Variável que indica se o jogo deve ser iniciado.

    // Loop principal do jogo.
    while (executando) {
        ALLEGRO_EVENT ev;                                       // Variável para armazenar o evento atual.
        al_wait_for_event(fila_eventos, &ev);                   // Aguarda a chegada de um evento na fila de eventos.

        // Verifica se o evento é o fechamento da tela.
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                executando = false;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                // Verifica se o botão do mouse foi pressionado
                if (!iniciar_jogo && ev.mouse.button == 1)    // Verifica se foi o botão esquerdo do mouse
                {
                    iniciar_jogo = true;  // Inicia o jogo quando o botão do mouse é pressionado
                }
            }
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {

            al_get_keyboard_state(&estado_teclado);             // Obtém o estado atual do teclado.

            // Verifica se a tecla de espaço foi pressionada e o jogo ainda não foi iniciado.
            if (al_key_down(&estado_teclado, ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && !iniciar_jogo) {
                iniciar_jogo = true;                            // Marca o jogo para ser iniciado.
            }

            // Verifica se o jogo foi iniciado.
            if (iniciar_jogo) {

                som_music = al_load_sample("music.wav");
                al_play_sample(som_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);     // Ativa efeito sonoro de fim de jogo

                // Verifica se a tecla de espaço foi pressionada e o jogo não está em modo gameover.
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_SPACE) && !gameover) {
                    passaro.velocidade = -10;                   // Define a velocidade do pássaro para cima.
                }

                passaro.velocidade += GRAVIDADE;                // Aplica a gravidade ao pássaro atualizando a velocidade.
                passaro.y += passaro.velocidade;                // Atualiza a posição vertical do pássaro de acordo com a velocidade.

                // Verifica se o obstáculo saiu da tela.
                if (obstaculo.x + LARGURA_OBSTACULO <= 0) {
                    obstaculo.x = LARGURA_TELA;                                 // Reposiciona o obstáculo no lado direito da tela.
                    obstaculo.y = rand() % (ALTURA_TELA - TAMANHO_FRESTA);      // Define uma nova posição aleatória para o obstáculo no eixo y.
                    pontuacao++;                                                // Incrementa a pontuação do jogador.
                }

                obstaculo.x -= VELOCIDADE_OBSTACULO;            // Move o obstáculo para a esquerda de acordo com a velocidade.

                // Verifica se ocorreu colisão entre o pássaro e o obstáculo.
                if (colisao(passaro, obstaculo)) {
                    al_stop_samples();                                                                                // Para todas as amostras de áudio, incluindo a música atual.
                    al_play_sample(al_load_sample("game_over.wav"), 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);   // Ativa efeito sonoro de fim de jogo
                    gameover = true;                                                                                  // Marca o jogo como gameover.
                    fim_jogo(pontuacao);                                                                              // Exibe a tela de fim de jogo.
                    break;                                                                                            // Break para sair do loop enquanto game over é exibido
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                       // Preenche a tela com a cor preta.
                al_draw_filled_circle(passaro.x + 25, passaro.y + 25, 25, al_map_rgb(255, 255, 0));                                                             // Desenha o pássaro na tela.
                al_draw_filled_rectangle(obstaculo.x, 0, obstaculo.x + LARGURA_OBSTACULO, obstaculo.y, al_map_rgb(0, 255, 0));                                // Desenha o obstáculo superior na tela.
                al_draw_filled_rectangle(obstaculo.x, obstaculo.y + TAMANHO_FRESTA, obstaculo.x + LARGURA_OBSTACULO, ALTURA_TELA, al_map_rgb(0, 255, 0));     // Desenha o obstáculo inferior na tela.
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 100, 20, 0, "Score: %d", pontuacao);
                al_flip_display();
            }

            // Caso o jogo não tenha sido iniciado.
            else {
                al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                       // Preenche a tela com a cor preta.
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Clique na tela para iniciar");    // Desenha a mensagem de instrução na tela.
                al_flip_display();                                                                                                                            // Atualiza a tela exibindo o conteúdo desenhado.
            }
        }
    }

    al_destroy_timer(timer);                                    // Destroi o temporizador.
    al_destroy_event_queue(fila_eventos);                       // Destroi a fila de eventos.
    al_destroy_display(display);                                // Destroi a tela do jogo.
    al_destroy_sample(som_music);                               // Destroi a trilha do jogo.

    return 0;                                                   // Retorna 0 para indicar sucesso.
}
