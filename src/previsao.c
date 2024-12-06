#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <gtk/gtk.h>
#include <time.h>  // Incluir para usar strptime

#include "previsao.h"

#define API_KEY "757b97253f0e0a914026b54068de55d7"  // Sua API Key

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Função para armazenar a resposta da API
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Não foi possível alocar memória!\n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para pegar dados do clima e exibir a previsão para o dia selecionado
void get_weather(const char *cidade, int dia, int mes, GtkWidget *label_resultado) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // Codificar a cidade para URL
    char *cidade_codificada = curl_easy_escape(curl, cidade, 0);

    // Formatar a URL para obter previsão de 5 dias
    char url[512];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/forecast?q=%s&appid=%s&units=metric", cidade_codificada, API_KEY);

    // Imprimir a URL gerada para depuração
    printf("URL Gerada: %s\n", url);

    // Inicializar a biblioteca curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Configurar a URL para fazer a requisição
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Fazer a requisição
        res = curl_easy_perform(curl);

        // Verificar se houve erro na requisição
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(res));
        } else {
            // Processar a resposta JSON
            struct json_object *parsed_json;
            struct json_object *list;
            struct json_object *item;
            struct json_object *main;
            struct json_object *temp;
            struct json_object *humidity;
            struct json_object *weather_array;
            struct json_object *weather_main;

            parsed_json = json_tokener_parse(chunk.memory);
            json_object_object_get_ex(parsed_json, "list", &list);

            // Procurar pela previsão do dia selecionado (dia e mês)
            int found = 0;
            for (int i = 0; i < json_object_array_length(list); i++) {
                item = json_object_array_get_idx(list, i);
                json_object_object_get_ex(item, "main", &main);
                json_object_object_get_ex(main, "temp", &temp);
                json_object_object_get_ex(main, "humidity", &humidity);
                json_object_object_get_ex(item, "weather", &weather_array);
                weather_main = json_object_array_get_idx(weather_array, 0);  // Pega o primeiro item da lista de "weather"

                // Verificar a data
                const char *dt_txt = json_object_get_string(json_object_object_get(item, "dt_txt"));
                struct tm tm;
                // Usando sscanf em vez de strptime para fazer o parse da data
                if (sscanf(dt_txt, "%4d-%2d-%2d %2d:%2d:%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
                    tm.tm_year -= 1900;  // Ajuste para o ano
                    tm.tm_mon -= 1;      // Ajuste para o mês

                    if (tm.tm_mday == dia && tm.tm_mon + 1 == mes) {
                        // Encontramos a previsão para o dia selecionado
                        const char *weather_description = json_object_get_string(json_object_object_get(weather_main, "description"));
                        char resultado[256];
                        snprintf(resultado, sizeof(resultado),
                                 "Temperatura: %.2f °C\nUmidade: %d%%\nCondição climática: %s",
                                 json_object_get_double(temp), json_object_get_int(humidity), weather_description);

                        gtk_label_set_text(GTK_LABEL(label_resultado), resultado);
                        found = 1;
                        break;
                    }
                }
            }

            if (!found) {
                gtk_label_set_text(GTK_LABEL(label_resultado), "Previsão não disponível para este dia.");
            }
        }

        // Limpar recursos
        curl_easy_cleanup(curl);
    }

    free(cidade_codificada);
    free(chunk.memory);
    curl_global_cleanup();
}
