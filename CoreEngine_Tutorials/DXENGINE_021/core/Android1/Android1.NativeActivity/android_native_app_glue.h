/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licenciado sob Apache License, Versão 2.0 (a "Licença");
 * você não pode usar esse arquivo, exceto se estiver em conformidade com a Licença.
 * Você pode obter uma cópia da Licença em
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * A menos que solicitado pela lei aplicável ou acordado por escrito, o software
 * distribuído sob a licença é distribuído em uma BASE ''NO ESTADO'',
 * SEM GARANTIAS OU CONDIÇÕES DE NENHUM TIPO, expressas ou implícitas.
 * Consulte a Licença para ver permissões de governo específicas e
 * limitações da Licença.
 *
 */

#ifndef _ANDROID_NATIVE_APP_GLUE_H
#define _ANDROID_NATIVE_APP_GLUE_H

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A interface da atividade nativa fornecida por <android/native_activity.h>
 * baseia-se em um conjunto de retornos de chamada fornecidos pelo aplicativo que será chamado
 * pelo thread principal da Atividade quando determinados eventos ocorrerem.
 *
 * Isso significa que cada um desses retornos de chamada não deve bloquear ou
 * arriscam-se a ter o fechamento forçado pelo sistema do aplicativo. Este modelo de programação
 * é direto e leve, mas possui restrições.
 *
 * A biblioteca estática 'threaded_native_app' é usada para fornecer um modelo de execução
 * diferente em que o aplicativo pode implementar seu próprio loop de evento principal
 * em um thread diferente. Veja aqui como funciona:
 *
 * 1/ O aplicativo deve fornecer uma função nomeada "android_main()" que
 *    será chamada quando a atividade for criada, em um novo thread
 *    distinto do thread principal da atividade.
 *
 * 2/ android_main() recebe um ponteiro para uma estrutura "android_app" válida
 *    que contém referências a outros objetos importantes, por exemplo, a
 *    instância de objeto ANativeActivity na qual o aplicativo é executado.
 *
 * 3/ o objeto "android_app" retém uma instância do ALooper que já
 *    ouve dois itens importantes:
 *
 *      - eventos de ciclo de vida da atividade (ex.: "pausar", "retomar"). Consulte declarações de APP_CMD_XXX
 *        abaixo.
 *
 *      - eventos de entrada provenientes de AInputQueue anexados à atividade.
 *
 *    Cada um deles corresponde a um identificador de ALooper retornado por
 *    ALooper_pollOnce com valores de LOOPER_ID_MAIN e LOOPER_ID_INPUT,
 *    respectivamente.
 *
 *    Seu aplicativo pode usar o mesmo ALooper para ouvir os
 *    descritores de arquivo adicionais.  Eles podem fazer retorno de chamada com base ou com retorno em
 *    identificadores começando com LOOPER_ID_USER.
 *
 * 4/ Sempre que você receber um evento LOOPER_ID_MAIN ou LOOPER_ID_INPUT,
 *    os dados retornados apontarão para uma estrutura android_poll_source.  Você
 *    pode chamar a função do processo nele e preencher android_app->onAppCmd
 *    e android_app->onInputEvent para ser chamado para seu próprio processamento
 *    do evento.
 *
 *    Se preferir, é possível chamar funções de níveis inferiores para ler e processar
 *    os dados diretamente...  observe as implementações de processo process_cmd() e process_input()
 *    na associação para ver como fazer isso.
 *
 * Consulte o modelo nomeado ''atividade nativa'' fornecido com o NDK com um
 * exemplo de uso completo.  Procure também o JavaDoc de NativeActivity.
 */

struct android_app;

/**
 * Dados associados a um ALooper fd que serão retornados como o ''outData"
 * quando a fonte já tiver dados prontos.
 */
struct android_poll_source {
    // O identificador desta fonte. Pode ser LOOPER_ID_MAIN ou
    // LOOPER_ID_INPUT.
    int32_t id;

    // O android_app ao qual este ident está associado.
    struct android_app* app;

    // Função a ser chamada para executar o processamento padrão de dados
    // dessa fonte.
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

/**
 * Essa é a interface para o código de associação de um aplicativo do
 * thread. Nesse modelo, o código do aplicativo é executado em seu próprio
 * thread separado do thread principal do processo.
 * Não é necessário que esse thread seja associado ao VM do Java,
 * apesar de precisar para fazer chamadas de JNI para quaisquer
 * objetos Java.
 */
struct android_app {
    // O aplicativo pode inserir um ponto a seu próprio projeto de estado
    // aqui, se desejar.
    void* userData;

    // Preencha com a função para processar comandos do aplicativo principais (APP_CMD_*)
    void (*onAppCmd)(struct android_app* app, int32_t cmd);

    // Preencha com a função para processar os eventos de entrada. Nesse ponto,
    // o evento já foi pré-expedido e será concluído após
    // o retorno.  Retorne 1 se você manuseou o evento, 0 para qualquer expedição
    // padrão.
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);

    // A instância do objeto ANativeActivity na qual este aplicativo está em execução.
    ANativeActivity* activity;

    // A configuração atual na qual o aplicativo está em execução.
    AConfiguration* config;

    // Este é o último estado de salvamento da instância, como fornecido no momento da criação.
    // É NULO se não houver nenhum estado.  Você pode usá-lo conforme necessário; a
    // memória permanecerá até que você chame android_app_exec_cmd() para
    // APP_CMD_RESUME, no ponto em que será liberado e savedState definido para NULO.
    // Essas variáveis devem ser alteradas somente ao processar um APP_CMD_SAVE_STATE,
    // no ponto em que serão inicializadas para NULO e você pode fazer malloc de seu
    // estado e colocar as informações aqui. Nesse caso, a memória será
    // liberada posteriormente.
    void* savedState;
    size_t savedStateSize;

    // O ALooper associado ao thread desse aplicativo.
    ALooper* looper;

    // Quando não for NULO, esta é a fila de entrada da qual o aplicativo
    // receberá eventos de entrada do usuário.
    AInputQueue* inputQueue;

    // Quando for não NULO, essa é a superfície da janela na qual o aplicativo pode desenhar.
    ANativeWindow* window;

    // Retângulo de conteúdo atual da janela. Esta é a área em que o
    // conteúdo da janela deve ser colocado para ser visto pelo usuário.
    ARect contentRect;

    // Estado atual da atividade do aplicativo. Pode ser APP_CMD_START,
    // APP_CMD_RESUME, APP_CMD_PAUSE ou APP_CMD_STOP; consulte abaixo.
    int activityState;

    // Isso é diferente de zero quando o NativeActivity do aplicativo é
    // destruído e aguarda o thread de aplicativos ser concluído.
    int destroyRequested;

    // -------------------------------------------------
    // Abaixo está implementação ''particular'' do código de associação.

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int msgread;
    int msgwrite;

    pthread_t thread;

    struct android_poll_source cmdPollSource;
    struct android_poll_source inputPollSource;

    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
    ARect pendingContentRect;
};

enum {
    /**
     * ID de dados do Looper de comandos provenientes do thread principal do aplicativo, que
     * é retornado como um identificador de ALooper_pollOnce().  Os dados para este
     * identificador são um ponteiro para uma estrutura android_poll_source.
     * Eles podem ser recuperados e processados com android_app_read_cmd()
     * e android_app_exec_cmd().
     */
    LOOPER_ID_MAIN = 1,

    /**
     * ID de dados do Looper provenientes da AInputQueue da
     * janela de aplicativos, que retorna como um identificador de
     * ALooper_pollOnce().  Os dados para este identificador são um ponteiro para uma estrutura
     * android_poll_source.  Eles podem ser lidos pelo objeto inputQueue
     * de android_app.
     */
    LOOPER_ID_INPUT = 2,

    /**
     * Início de identificadores de ALooper definidos pelo usuário.
     */
    LOOPER_ID_USER = 3,
};

enum {
    /**
     * Comando do thread principal: o AInputQueue foi alterado.  Após processar
     * esse comando, android_app->inputQueue será atualizado para a nova fila
     * (ou NULO).
     */
    APP_CMD_INPUT_CHANGED,

    /**
     * Comando do thread principal: um novo ANativeWindow está pronto para uso.  Após
     * receber este comando, android_app->window conterá a nova superfície de
     * janela.
     */
    APP_CMD_INIT_WINDOW,

    /**
     * Comando do thread principal: a ANativeWindow existente precisa ser
     * terminada.  Ao receber esse comando, android_app->window ainda
     * contém a janela existente; após chamar android_app_exec_cmd
 * ele será definido como NULO.
     */
    APP_CMD_TERM_WINDOW,

    /**
     * Comando do thread principal: o ANativeWindow atual foi redimensionado.
     * Redesenhe com esse novo tamanho.
     */
    APP_CMD_WINDOW_RESIZED,

    /**
     * Comando do thread principal: o sistema precisa que o ANativeWindow atual
     * seja redesenhado.  Você deve redesenhar a janela antes de passar para
     * android_app_exec_cmd() para evitar falhas de desenho transitórias.
     */
    APP_CMD_WINDOW_REDRAW_NEEDED,

    /**
     * Comando do thread principal: a área de conteúdo da janela foi alterada,
     * como a partir da janela de entrada simples sendo exibida ou ocultada.  Você pode
     * encontrar o novo rect. de conteúdo em android_app::contentRect.
     */
    APP_CMD_CONTENT_RECT_CHANGED,

    /**
     * Comando do thread principal: a janela de atividade do aplicativo ganhou
     * foco de entrada.
     */
    APP_CMD_GAINED_FOCUS,

    /**
     * Comando do thread principal: a janela de atividade do aplicativo perdeu
     * foco de entrada.
     */
    APP_CMD_LOST_FOCUS,

    /**
     * Comando do thread principal: a configuração do dispositivo atual foi alterada.
     */
    APP_CMD_CONFIG_CHANGED,

    /**
     * Comando do thread principal: o sistema está com memória baixa.
     * Tente reduzir o uso da memória.
     */
    APP_CMD_LOW_MEMORY,

    /**
     * Comando do thread principal: a atividade do aplicativo foi iniciada.
     */
    APP_CMD_START,

    /**
     * Comando do thread principal: a atividade do aplicativo foi retomada.
     */
    APP_CMD_RESUME,

    /**
     * Comando do thread principal: o aplicativo deve gerar um novo estado salvo
     * para si, a fim de restaurar posteriormente, se necessário.  Se você tiver o estado salvo,
     * aloque com o malloc e coloque no android_app.savedState com
     * o tamanho em android_app.savedStateSize.  Ele será liberado para você
     * posteriormente.
     */
    APP_CMD_SAVE_STATE,

    /**
     * Comando do thread principal: a atividade do aplicativo foi pausada.
     */
    APP_CMD_PAUSE,

    /**
     * Comando do thread principal: a atividade do aplicativo foi interrompida.
     */
    APP_CMD_STOP,

    /**
     * Comando do thread principal: a atividade do aplicativo está sendo destruída,
     * e aguardando que o thread do aplicativo faça a limpeza e saia antes de prosseguir.
     */
    APP_CMD_DESTROY,
};

/**
 * Chamar quando ALooper_pollAll() retornar LOOPER_ID_MAIN, lendo a próxima
 * mensagem de comando do aplicativo.
 */
int8_t android_app_read_cmd(struct android_app* android_app);

/**
 * Chamada com o comando retornado pelo android_app_read_cmd() para fazer o
 * pré-processamento inicial do comando fornecido.  Você pode executar suas próprias
 * ações para o comando após chamar esta função.
 */
void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * A chamada com o comando retornado pelo android_app_read_cmd() para fazer o
 * pós-processamento final do comando fornecido.  Você deve ter feito suas próprias
 * ações para o comando antes de chamar essa função.
 */
void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * Esta é a função que o código do aplicativo deve implementar, representando
 * a entrada principal para o aplicativo.
 */
extern void android_main(struct android_app* app);

#ifdef __cplusplus
}
#endif

#endif /* _ANDROID_NATIVE_APP_GLUE_H */
