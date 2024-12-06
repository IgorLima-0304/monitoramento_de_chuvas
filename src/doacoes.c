// # Código da função "doar_para_vitimas"
#include "doacoes.h"

void doar_para_vitimas(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(
        NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "=== Informações de Doação ===\n"
        "1) Dinheiro:\n"
        "   - Doe via PIX para 12345678900 (Defesa Civil RS).\n\n"
        "2) Alimentos:\n"
        "   - Entregue em postos de arrecadação das prefeituras locais.\n\n"
        "3) Roupas e Cobertores:\n"
        "   - Doações podem ser feitas nas ONGs locais ou na Cruz Vermelha.\n\n"
        "4) Voluntariado:\n"
        "   - Cadastre-se no site oficial da Defesa Civil RS.\n\n"
        "Obrigado por sua ajuda!"
    );

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
