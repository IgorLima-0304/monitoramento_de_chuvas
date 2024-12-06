#ifndef PREVISAO_H
#define PREVISAO_H

#include <gtk/gtk.h>  // Incluir GTK para usar GtkWidget

// Modificado para incluir os parâmetros dia e mês
void get_weather(const char *cidade, int dia, int mes, GtkWidget *label_resultado);

#endif // PREVISAO_H
