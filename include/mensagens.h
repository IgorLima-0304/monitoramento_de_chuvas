#ifndef MENSAGENS_H
#define MENSAGENS_H

#include <gtk/gtk.h>

// Função para enviar mensagens de apoio
void enviar_mensagens_apoio(GtkWidget *widget, gpointer data);

// Função auxiliar para tratar a mensagem quando o botão OK for pressionado
void on_ok_clicked_mensagem(GtkWidget *button, gpointer dialog);

#endif
