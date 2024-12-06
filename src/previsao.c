#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <gtk/gtk.h>  // Incluir GTK para usar GtkWidget
#include "previsao.h" 

#define API_KEY "757b97253f0e0a914026b54068de55d7"  // Sua API Key
#define API_URL "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric"

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

// Função para pegar dados do clima e exibir a previsão
void get_weather(const char *cidade, GtkWidget *label_resultado) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // Codificar a cidade para URL
    char *cidade_codificada = curl_easy_escape(curl, cidade, 0);

    // Formatar a URL com o nome da cidade codificada e a chave da API
    char url[512];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric", cidade_codificada, API_KEY);

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
            struct json_object *main;
            struct json_object *temp;
            struct json_object *humidity;
            struct json_object *weather_array;
            struct json_object *weather_main;

            parsed_json = json_tokener_parse(chunk.memory);

            // Extrair os dados da resposta JSON
            json_object_object_get_ex(parsed_json, "main", &main);
            json_object_object_get_ex(main, "temp", &temp);
            json_object_object_get_ex(main, "humidity", &humidity);
            json_object_object_get_ex(parsed_json, "weather", &weather_array);
            weather_main = json_object_array_get_idx(weather_array, 0);  // Pega o primeiro item da lista de "weather"

            const char *weather_description = json_object_get_string(json_object_object_get(weather_main, "description"));

            // Exibir as informações no GtkLabel
            char resultado[256];
            snprintf(resultado, sizeof(resultado),
                     "Temperatura: %.2f °C\nUmidade: %d%%\nCondição climática: %s",
                     json_object_get_double(temp), json_object_get_int(humidity), weather_description);

            gtk_label_set_text(GTK_LABEL(label_resultado), resultado);

            // Verificar se está chovendo
            if (strstr(weather_description, "rain") != NULL) {
                gtk_label_set_text(GTK_LABEL(label_resultado), "Está chovendo na cidade.");
            }
        }

        // Limpar recursos
        curl_easy_cleanup(curl);
    }

    free(cidade_codificada);
    free(chunk.memory);
    curl_global_cleanup();
}