#ifndef PREVISAO_H
#define PREVISAO_H
#include <gtk/gtk.h> 
// Declaração da função que será definida em previsao.c
void get_weather(const char *cidade, GtkWidget *label_resultado);

#endif // PREVISAO_H
