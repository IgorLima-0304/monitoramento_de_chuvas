// # Código da função "enviar_mensagens_apoio"
#include "mensagens.h"
#include <gtk/gtk.h>

void enviar_mensagens_apoio(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *label, *entry;
    GtkWidget *button_ok, *button_cancel;
    GtkWidget *idade_spin;
    GtkDialogFlags flags = GTK_DIALOG_MODAL;

    // Criação do diálogo
    dialog = gtk_dialog_new_with_buttons("Enviar Mensagem de Apoio",
                                         NULL,
                                         flags,
                                         NULL,
                                         NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Layout em grid
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Campo para idade
    label = gtk_label_new("Digite a idade do destinatário:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    idade_spin = gtk_spin_button_new_with_range(1, 120, 1);  // Definindo intervalo de 1 a 120 anos
    gtk_grid_attach(GTK_GRID(grid), idade_spin, 1, 0, 1, 1);

    // Passar o widget idade_spin para o callback
    g_object_set_data(G_OBJECT(dialog), "idade_spin", idade_spin);

    // Botões
    button_ok = gtk_button_new_with_label("OK");
    button_cancel = gtk_button_new_with_label("Cancelar");
    gtk_grid_attach(GTK_GRID(grid), button_ok, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_cancel, 1, 1, 1, 1);

    // Mostrar elementos
    gtk_widget_show_all(dialog);

    // Conectar sinais aos botões
    g_signal_connect_swapped(button_cancel, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);
    g_signal_connect(button_ok, "clicked", G_CALLBACK(on_ok_clicked_mensagem), dialog);

    gtk_main();
}

// Função para tratar a mensagem quando o botão OK for pressionado
void on_ok_clicked_mensagem(GtkWidget *button, gpointer dialog) {
    GtkWidget *idade_spin = g_object_get_data(G_OBJECT(dialog), "idade_spin");  // Obter o widget de idade_spin

    int idade = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(idade_spin));  // Obter o valor da idade

    char mensagem[256];

    if (idade < 18) {
        snprintf(mensagem, sizeof(mensagem), "Mensagem para criança: 'Você é forte! Estamos juntos para superar esse momento.'");
    } else if (idade < 60) {
        snprintf(mensagem, sizeof(mensagem), "Mensagem para jovem/adulto: 'Mantenha a esperança! Sua resiliência nos inspira.'");
    } else {
        snprintf(mensagem, sizeof(mensagem), "Mensagem para idoso: 'Você é um exemplo de coragem. Estamos aqui para ajudar.'");
    }

    // Exibir a mensagem de apoio
    GtkWidget *resultado = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_INFO,
                                                  GTK_BUTTONS_OK,
                                                  "Sua mensagem foi enviada: \n%s", mensagem);
    gtk_dialog_run(GTK_DIALOG(resultado));
    gtk_widget_destroy(resultado);

    // Fechar o diálogo principal
    gtk_widget_destroy(dialog);
}
