#include <gtk/gtk.h>
#include "previsao.h"  // Incluir o arquivo de previsão
#include "doacoes.h"
#include "mensagens.h"

// Função para sair do programa
void sair_do_programa(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// Função para verificar a previsão do tempo (chama a função get_weather)
void verificar_previsao_tempo(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);  // Obtém o widget de entrada (cidade)
    const char *cidade = gtk_entry_get_text(GTK_ENTRY(entry));  // Pega o texto da entrada

    if (cidade == NULL || strlen(cidade) == 0) {
        // Se a cidade estiver vazia, exibe uma mensagem de erro
        GtkWidget *erro_dialog = gtk_message_dialog_new(
            NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Por favor, insira o nome de uma cidade válida."
        );
        gtk_dialog_run(GTK_DIALOG(erro_dialog));
        gtk_widget_destroy(erro_dialog);
        return;  // Retorna para não fazer a requisição
    }

    // Obtém o dia e o mês selecionado
    GtkWidget *spin_day = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "spin_day"));
    GtkWidget *spin_month = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "spin_month"));
    
    int dia = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_day));
    int mes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_month));

    // Passa o GtkLabel para mostrar a previsão
    GtkWidget *label_resultado = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "resultado_label"));
    
    // Chama a função para pegar a previsão
    get_weather(cidade, dia, mes, label_resultado);  
}


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button1, *button2, *button3, *button4;
    GtkWidget *cidade_entry;  // Campo de entrada para cidade
    GtkWidget *label_resultado;  // Label para exibir a previsão
    GtkWidget *spin_day, *spin_month;  // Campos de entrada para o dia e mês

    // Inicialização do GTK
    gtk_init(&argc, &argv);

    // Criação da janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Monitoramento de Chuvas no RS");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Criação do layout em grade
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Campo de entrada para a cidade
    cidade_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cidade_entry), "Digite o nome da cidade");
    gtk_grid_attach(GTK_GRID(grid), cidade_entry, 0, 0, 1, 1);

    // Campo de entrada para o dia
    GtkWidget *label_day = gtk_label_new("Dia:");
    spin_day = gtk_spin_button_new_with_range(1, 31, 1);  // Permitindo dias entre 1 e 31
    gtk_grid_attach(GTK_GRID(grid), label_day, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spin_day, 1, 1, 1, 1);

    // Campo de entrada para o mês
    GtkWidget *label_month = gtk_label_new("Mês:");
    spin_month = gtk_spin_button_new_with_range(1, 12, 1);  // Permitindo meses entre 1 e 12
    gtk_grid_attach(GTK_GRID(grid), label_month, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spin_month, 1, 2, 1, 1);

    // Label para exibir os resultados da previsão
    label_resultado = gtk_label_new("Resultado da previsão será exibido aqui.");
    gtk_grid_attach(GTK_GRID(grid), label_resultado, 0, 3, 2, 1);

    // Botões do menu
    button1 = gtk_button_new_with_label("1) Verificar a Previsão do Tempo");
    button2 = gtk_button_new_with_label("2) Doar para Vítimas das Chuvas");
    button3 = gtk_button_new_with_label("3) Enviar Mensagens de Apoio");
    button4 = gtk_button_new_with_label("4) Sair");

    // Adicionando os botões à grid
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 7, 1, 1);

    // Passando os dados necessários para as funções
    g_object_set_data(G_OBJECT(button1), "spin_day", spin_day);
    g_object_set_data(G_OBJECT(button1), "spin_month", spin_month);
    g_object_set_data(G_OBJECT(button1), "resultado_label", label_resultado);

    // Conectando sinais às funções
    g_signal_connect(button1, "clicked", G_CALLBACK(verificar_previsao_tempo), cidade_entry);
    g_signal_connect(button2, "clicked", G_CALLBACK(doar_para_vitimas), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(enviar_mensagens_apoio), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(sair_do_programa), NULL);

    // Mostrar janela e seus elementos
    gtk_widget_show_all(window);

    // Loop principal do GTK
    gtk_main();

    return 0;
}
