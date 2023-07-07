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
*     Jo�o Pedro Silva Braga
*     Gabriel Figueiredo Cayres Burdignon
*
* START DATE :  05 Jun 23
*
*H*/

/// Bibliotecas
#include <iostream>                       // Biblioteca de entrada e sa�da padr�o.
#include <cstdlib>                        // Biblioteca que cont�m fun��es para gerar n�meros aleat�rios.
#include <ctime>                          // Biblioteca que fornece fun��es relacionadas ao tempo.
#include <allegro5/allegro.h>             // Biblioteca principal do Allegro.
#include <allegro5/allegro_primitives.h>  // Biblioteca que fornece fun��es para desenho primitivo.
#include <allegro5/allegro_font.h>        // Biblioteca que fornece fun��es relacionadas a fontes de texto.
#include <allegro5/allegro_ttf.h>         // Biblioteca que fornece fun��es relacionadas a arquivos de fonte TTF.
#include <allegro5/allegro_audio.h>       // Biblioteca que fornece fun��es relacionadas a arquivos de audio
#include <allegro5/allegro_acodec.h>      // Biblioteca que fornece fun��es relacionadas a arquivos de audio

#define LARGURA_TELA 800                  // Define a largura da tela como 800 pixels.
#define ALTURA_TELA 600                   // Define a altura da tela como 600 pixels.
#define TAMANHO_FRESTA 200                // Define o tamanho da fresta como 200 pixels.
#define LARGURA_OBSTACULO 80              // Define a largura do obst�culo como 80 pixels.
#define VELOCIDADE_OBSTACULO 3            // Define a velocidade do obst�culo como 3 pixels por frame.
#define GRAVIDADE 1                       // Define a gravidade como 1 pixel por frame.

using namespace std;                                            // Usa o namespace std para facilitar o uso de elementos padr�o do C++.

ALLEGRO_FONT* fonte = nullptr;                                  // Ponteiro para a fonte do texto.
ALLEGRO_SAMPLE* som_music = nullptr;                          // Ponteiro para armazenar o som.

/// Define uma estrutura para representar o p�ssaro.
struct Passaro {
    float x, y;                                                 // Coordenadas x e y do p�ssaro.
    float velocidade;                                           // Velocidade do p�ssaro.
};

/// Define uma estrutura para representar os obst�culos.
struct Obstaculo {
    float x, y;                                                 // Coordenadas x e y do obst�culo.
};


/*******************************************************************
* NAME :            void fim_jogo(int pontuacao)
*
* DESCRIPTION :     Fun��o respons�vel por exibir a tela de fim de jogo
*
* INPUTS :
*       PARAMETERS:
*           int pontuacao : valor da pontua��o
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
*       [3] Desenhar a pontua��o na tela.
*       [4] Atualizar o display.
*       [5] Aguardar 2 segundos antes de fechar o display.
*/
void fim_jogo(int pontuacao)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                 // Limpa a tela e define a cor de fundo como preto.
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 10, ALLEGRO_ALIGN_CENTER, "Game Over");             // Desenha o texto "Game Over" na tela.
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 10, ALLEGRO_ALIGN_CENTER, "Score: %d", pontuacao);  // Desenha a pontua��o na tela.
    al_flip_display();                                                                                                                      // Atualiza a tela.
    al_rest(2.0);                                                                                                                           // Delay de 2 segundos antes de fechar o display
}


/**************************************************************************
* NAME :            int colisao(Passaro passaro, Obstaculo obstaculo)
*
* DESCRIPTION :     Fun��o respons�vel por verificar a colis�o entre o p�ssaro e o obst�culo
*
* INPUTS :
*       PARAMETERS:
*           Passaro passaro             Estrutura que representa o p�ssaro com coordenadas (x, y)
*           Obstaculo obstaculo         Estrutura que representa o obst�culo com coordenadas (x, y)
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
*           Values: 0             N�o ocorreu colis�o
*                   1             Ocorreu colis�o
*
* PROCESS :
*       [1] Verifica se o p�ssaro ultrapassou os limites da tela
*       [3] Verifica se ocorreu colis�o entre o p�ssaro e o obst�culo.
*       [4] Retorna se ocorreu ou n�o colis�o
*/
int colisao(Passaro passaro, Obstaculo obstaculo)
{
    // Verifica se o p�ssaro ultrapassou os limites da tela.
    if (passaro.y <= 0 || passaro.y >= ALTURA_TELA)
        return 1;                                               // Retorna 1 para indicar colis�o.

    // Verifica se ocorreu colis�o entre o p�ssaro e o obst�culo.
    if (passaro.x + 50 >= obstaculo.x && passaro.x <= obstaculo.x + LARGURA_OBSTACULO && (passaro.y <= obstaculo.y || passaro.y + 50 >= obstaculo.y + TAMANHO_FRESTA))
        return 1;                                               // Retorna 1 para indicar colis�o.

    return 0;                                                   // Retorna 0 para indicar aus�ncia de colis�o.
}

/************************************************************************************
* NAME :            test_colisao
*
* DESCRIPTION :     Realiza testes para verificar o funcionamento correto da fun��o colisao.
*
* INPUTS :
*       None
*
* OUTPUTS :
*       None
*
* PROCESS :
*       [1] Cria um objeto Passaro e um objeto Obstaculo.
*       [2] Testa se o passaro est� acima da tela. Espera-se que a fun��o colisao retorne 1.
*       [3] Testa se o passaro est� abaixo da tela. Espera-se que a fun��o colisao retorne 1.
*       [4] Testa se o passaro est� sem colis�o com o obstaculo. Espera-se que a fun��o colisao retorne 0.
*       [5] Testa se o passaro colide com a parte superior do obstaculo. Espera-se que a fun��o colisao retorne 1.
*       [6] Testa se o passaro colide com a parte inferior do obstaculo. Espera-se que a fun��o colisao retorne 1.
*       [7] Exibe a mensagem "Todos os testes passaram!".
*
* NOTES :
*       Esta fun��o assume que j� existe a fun��o colisao implementada corretamente.
*       A fun��o assert() � usada para verificar se os resultados esperados s�o retornados pela fun��o colisao.
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
* DESCRIPTION :     Realiza testes para verificar o funcionamento correto da fun��o fim_jogo.
*
* INPUTS :
*       None
*
* OUTPUTS :
*       None
*
* PROCESS :
*       [1] Configura��o inicial do Allegro.
*       [2] Inicializa��o dos addons necess�rios (primitives, font e ttf).
*       [3] Carrega a fonte "arial.ttf" para uso nos testes.
*       [4] Verifica se a fonte foi carregada corretamente. Caso contr�rio, exibe uma mensagem de erro.
*       [5] Teste: pontua��o igual a 0.
*       [6] Teste: pontua��o positiva.
*       [7] Teste: pontua��o negativa.
*       [8] Finaliza��es do Allegro.
*       [9] Libera��o da mem�ria alocada para a fonte.
*       [10] Desligamento dos addons do Allegro.
*       [11] Exibe a mensagem "Todos os testes passaram!".
*
* NOTES :
*       Esta fun��o assume que o Allegro j� est� configurado corretamente.
*       A fun��o fim_jogo() ser� testada com diferentes valores de pontua��o.
*       A fun��o assert() � usada para verificar se a fonte foi carregada corretamente.
*/
void test_fim_jogo()
{
    // Configura��o inicial do Allegro
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Carrega a fonte para uso nos testes
    fonte = al_load_ttf_font("arial.ttf", 24, 0);
    assert(fonte != nullptr && "Erro ao carregar a fonte!");

    // Teste: pontua��o igual a 0
    fim_jogo(0);

    // Teste: pontua��o positiva
    fim_jogo(100);

    // Teste: pontua��o negativa
    fim_jogo(-50);

    // Finaliza��es do Allegro
    al_destroy_font(fonte);
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();

    cout << "Todos os testes passaram!" << endl;
}


/******** Fun��o principal do programa  *******/
int main()
{
    srand(static_cast<unsigned>(time(NULL)));                   // Inicializa o gerador de n�meros aleat�rios com base no tempo atual.

    ALLEGRO_DISPLAY* display = nullptr;                         // Ponteiro para a tela do jogo.
    ALLEGRO_EVENT_QUEUE* fila_eventos = nullptr;                // Ponteiro para a fila de eventos.
    ALLEGRO_TIMER* timer = nullptr;                             // Ponteiro para o temporizador.
    ALLEGRO_KEYBOARD_STATE estado_teclado;                      // Vari�vel para armazenar o estado do teclado.

    test_colisao();

    // Inicializa a biblioteca Allegro.
    if (!al_init()) {
        cerr << "Falha ao inicializar o Allegro." << endl;      // Exibe uma mensagem de erro caso a inicializa��o falhe.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    display = al_create_display(LARGURA_TELA, ALTURA_TELA);     // Cria a tela do jogo com as dimens�es especificadas.

    // Verifica se a cria��o da tela falhou.
    if (!display) {
        cerr << "Falha ao criar a tela." << endl;               // Exibe uma mensagem de erro caso a cria��o falhe.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    fila_eventos = al_create_event_queue();                     // Cria a fila de eventos.

    // Verifica se a cria��o da fila de eventos falhou.
    if (!fila_eventos) {
        cerr << "Falha ao criar a fila de eventos." << endl;    // Exibe uma mensagem de erro caso a cria��o falhe.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    timer = al_create_timer(1.0 / 60);                          // Cria o temporizador com uma frequ�ncia de 60 vezes por segundo.

    // Verifica se a cria��o do temporizador falhou.
    if (!timer) {
        cerr << "Falha ao criar o temporizador." << endl;       // Exibe uma mensagem de erro caso a cria��o falhe.
        al_destroy_event_queue(fila_eventos);                   // Destroi a fila de eventos.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    al_init_primitives_addon();                                 // Inicializa o addon de desenho primitivo do Allegro.
    al_install_keyboard();                                      // Instala o teclado.
    al_init_font_addon();                                       // Inicializa o addon de fontes do Allegro.
    al_init_ttf_addon();                                        // Inicializa o addon TrueType Font do Allegro.
    al_install_audio();                                         // Instala o m�dulo de �udio do Allegro.
    al_init_acodec_addon();                                     // Inicializa o addon de decodifica��o de �udio do Allegro.
    al_reserve_samples(2);                                      // Reserva 1 voz (canal) para reprodu��o de amostras de �udio.

    fonte = al_create_builtin_font();                           // Cria a fonte padr�o do Allegro.

    // Verifica se a cria��o da fonte falhou.
    if (!fonte) {
        cerr << "Failed to create fonte." << endl;              // Exibe uma mensagem de erro caso a cria��o falhe.
        al_destroy_timer(timer);                                // Destroi o temporizador.
        al_destroy_event_queue(fila_eventos);                   // Destroi a fila de eventos.
        al_destroy_display(display);                            // Destroi a tela do jogo.
        return -1;                                              // Retorna -1 para indicar erro.
    }

    // Verifica se a instala��o do mouse falhou
    if (!al_install_mouse()) {
        cerr << "Falha ao inicializar o mouse." << endl;        // Exibe mensagem de erro
        return -1;                                              // Retorna -1 para indicar erro.
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(display));       // Registra a fonte de eventos da tela na fila de eventos.
    al_register_event_source(fila_eventos, al_get_mouse_event_source());                // Registra a fonte de eventos do mouse de eventos.
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));           // Registra a fonte de eventos do temporizador na fila de eventos.
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());             // Registra a fonte de eventos do teclado na fila de eventos.

    al_clear_to_color(al_map_rgb(0, 0, 0));                     // Preenche a tela com a cor preta.
    al_flip_display();                                          // Atualiza a tela exibindo o conte�do desenhado.
    al_start_timer(timer);                                      // Inicia o temporizador.

    Passaro passaro;                                            // Declara uma vari�vel do tipo Passaro.
    passaro.x = 100;                                            // Define a posi��o inicial do p�ssaro no eixo x.
    passaro.y = ALTURA_TELA / 2;                                // Define a posi��o inicial do p�ssaro no eixo y.
    passaro.velocidade = 0;                                     // Define a velocidade inicial do p�ssaro como zero.

    Obstaculo obstaculo;                                        // Declara uma vari�vel do tipo Obstaculo.
    obstaculo.x = LARGURA_TELA;                                 // Define a posi��o inicial do obst�culo no eixo x.
    obstaculo.y = rand() % (ALTURA_TELA - TAMANHO_FRESTA);      // Define a posi��o inicial do obst�culo no eixo y de forma aleat�ria.

    bool executando = true;                                     // Vari�vel de controle do loop principal do jogo.
    bool gameover = false;                                      // Vari�vel que indica se o jogo terminou.
    int pontuacao = 0;                                          // Vari�vel para armazenar a pontua��o do jogador.
    bool iniciar_jogo = false;                                  // Vari�vel que indica se o jogo deve ser iniciado.

    // Loop principal do jogo.
    while (executando) {
        ALLEGRO_EVENT ev;                                       // Vari�vel para armazenar o evento atual.
        al_wait_for_event(fila_eventos, &ev);                   // Aguarda a chegada de um evento na fila de eventos.

        // Verifica se o evento � o fechamento da tela.
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                executando = false;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                // Verifica se o bot�o do mouse foi pressionado
                if (!iniciar_jogo && ev.mouse.button == 1)    // Verifica se foi o bot�o esquerdo do mouse
                {
                    iniciar_jogo = true;  // Inicia o jogo quando o bot�o do mouse � pressionado
                }
            }
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {

            al_get_keyboard_state(&estado_teclado);             // Obt�m o estado atual do teclado.

            // Verifica se a tecla de espa�o foi pressionada e o jogo ainda n�o foi iniciado.
            if (al_key_down(&estado_teclado, ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && !iniciar_jogo) {
                iniciar_jogo = true;                            // Marca o jogo para ser iniciado.
            }

            // Verifica se o jogo foi iniciado.
            if (iniciar_jogo) {

                som_music = al_load_sample("music.wav");
                al_play_sample(som_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);     // Ativa efeito sonoro de fim de jogo

                // Verifica se a tecla de espa�o foi pressionada e o jogo n�o est� em modo gameover.
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_SPACE) && !gameover) {
                    passaro.velocidade = -10;                   // Define a velocidade do p�ssaro para cima.
                }

                passaro.velocidade += GRAVIDADE;                // Aplica a gravidade ao p�ssaro atualizando a velocidade.
                passaro.y += passaro.velocidade;                // Atualiza a posi��o vertical do p�ssaro de acordo com a velocidade.

                // Verifica se o obst�culo saiu da tela.
                if (obstaculo.x + LARGURA_OBSTACULO <= 0) {
                    obstaculo.x = LARGURA_TELA;                                 // Reposiciona o obst�culo no lado direito da tela.
                    obstaculo.y = rand() % (ALTURA_TELA - TAMANHO_FRESTA);      // Define uma nova posi��o aleat�ria para o obst�culo no eixo y.
                    pontuacao++;                                                // Incrementa a pontua��o do jogador.
                }

                obstaculo.x -= VELOCIDADE_OBSTACULO;            // Move o obst�culo para a esquerda de acordo com a velocidade.

                // Verifica se ocorreu colis�o entre o p�ssaro e o obst�culo.
                if (colisao(passaro, obstaculo)) {
                    al_stop_samples();                                                                                // Para todas as amostras de �udio, incluindo a m�sica atual.
                    al_play_sample(al_load_sample("game_over.wav"), 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);   // Ativa efeito sonoro de fim de jogo
                    gameover = true;                                                                                  // Marca o jogo como gameover.
                    fim_jogo(pontuacao);                                                                              // Exibe a tela de fim de jogo.
                    break;                                                                                            // Break para sair do loop enquanto game over � exibido
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                       // Preenche a tela com a cor preta.
                al_draw_filled_circle(passaro.x + 25, passaro.y + 25, 25, al_map_rgb(255, 255, 0));                                                             // Desenha o p�ssaro na tela.
                al_draw_filled_rectangle(obstaculo.x, 0, obstaculo.x + LARGURA_OBSTACULO, obstaculo.y, al_map_rgb(0, 255, 0));                                // Desenha o obst�culo superior na tela.
                al_draw_filled_rectangle(obstaculo.x, obstaculo.y + TAMANHO_FRESTA, obstaculo.x + LARGURA_OBSTACULO, ALTURA_TELA, al_map_rgb(0, 255, 0));     // Desenha o obst�culo inferior na tela.
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 100, 20, 0, "Score: %d", pontuacao);
                al_flip_display();
            }

            // Caso o jogo n�o tenha sido iniciado.
            else {
                al_clear_to_color(al_map_rgb(0, 0, 0));                                                                                                       // Preenche a tela com a cor preta.
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Clique na tela para iniciar");    // Desenha a mensagem de instru��o na tela.
                al_flip_display();                                                                                                                            // Atualiza a tela exibindo o conte�do desenhado.
            }
        }
    }

    al_destroy_timer(timer);                                    // Destroi o temporizador.
    al_destroy_event_queue(fila_eventos);                       // Destroi a fila de eventos.
    al_destroy_display(display);                                // Destroi a tela do jogo.
    al_destroy_sample(som_music);                               // Destroi a trilha do jogo.

    return 0;                                                   // Retorna 0 para indicar sucesso.
}
