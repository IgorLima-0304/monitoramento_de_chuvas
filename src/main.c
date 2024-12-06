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

    // Passa o GtkLabel para mostrar a previsão
    GtkWidget *label_resultado = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "resultado_label"));
    
    get_weather(cidade, label_resultado);  // Chama a função do arquivo previsao.c
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button1, *button2, *button3, *button4;
    GtkWidget *cidade_entry;  // Campo de entrada para cidade
    GtkWidget *label_resultado;  // Label para exibir a previsão

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

    // Label para exibir os resultados da previsão
    label_resultado = gtk_label_new("Resultado da previsão será exibido aqui.");
    gtk_grid_attach(GTK_GRID(grid), label_resultado, 0, 1, 1, 1);

    // Botões do menu
    button1 = gtk_button_new_with_label("1) Verificar a Previsão do Tempo");
    button2 = gtk_button_new_with_label("2) Doar para Vítimas das Chuvas");
    button3 = gtk_button_new_with_label("3) Enviar Mensagens de Apoio");
    button4 = gtk_button_new_with_label("4) Sair");

    // Adicionando os botões à grid
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 5, 1, 1);

    // Passando o GtkLabel para a função verificar_previsao_tempo
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
