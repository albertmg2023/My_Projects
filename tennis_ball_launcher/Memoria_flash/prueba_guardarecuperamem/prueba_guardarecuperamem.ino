#include <Preferences.h>

Preferences preferences;

#define MAX_FILAS 20
#define MAX_COLUMNAS 5

void guardarMatriz(int matriz [MAX_FILAS][MAX_COLUMNAS], int filas, int columnas, String lugar) {
    preferences.begin("storage", false);

    String data = "";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            data += String(matriz[i][j]); 
            if (j < columnas - 1) data += ","; 
        }
        if (i < filas - 1) data += ";"; 
    }

    preferences.putString(lugar.c_str(), data);
    Serial.println("Matriz guardada: " + data);
    
    preferences.end();
}

void contarFilasColumnas(String data, int &filas, int &columnas) {
    filas = 1; 
    columnas = 1;

    if (data.length() == 0) {
        filas = 0;
        columnas = 0;
        return;
    }

    for (char c : data) {
        if (c == ';') filas++;
    }

    columnas = 1;
    for (char c : data) {
        if (c == ',') columnas++;
        if (c == ';') break;
    }
}

void recuperarLista(int listaDeListas[MAX_FILAS][MAX_COLUMNAS], String lugar) {
    preferences.begin("storage", true);
    String data = preferences.getString(lugar.c_str(), ""); 
    preferences.end();

    if (data == "") {
        Serial.println("No hay datos en memoria.");
        return;
    }

    int filas, columnas;
    contarFilasColumnas(data, filas, columnas);

    Serial.print("Filas detectadas: ");
    Serial.print(filas);
    Serial.print(", Columnas detectadas: ");
    Serial.println(columnas);
    
    int fila = 0;
    char buffer[data.length() + 1];
    data.toCharArray(buffer, data.length() + 1);

    char *saveptrFila;
    char *saveptrColumna;
    char *tokenFila = strtok_r(buffer, ";", &saveptrFila);

    while (tokenFila != NULL && fila < filas) {
        int columna = 0;
        char *tokenColumna = strtok_r(tokenFila, ",", &saveptrColumna);
        while (tokenColumna != NULL && columna < columnas) {
            listaDeListas[fila][columna] = atoi(tokenColumna);
            tokenColumna = strtok_r(NULL, ",", &saveptrColumna);
            columna++;
        }
        tokenFila = strtok_r(NULL, ";", &saveptrFila);
        fila++;
    }

    Serial.println("Lista reconstruida:");
    for (int i = 0; i < filas; i++) {
        Serial.print("Fila ");
        Serial.print(i);
        Serial.print(": ");
        for (int j = 0; j < columnas; j++) {
            Serial.print(listaDeListas[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    int matrizPrueba[MAX_FILAS][MAX_COLUMNAS] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15}
    };

    String memoriaLugar = "entrenamiento1";

    Serial.println("Guardando matriz en memoria...");
    guardarMatriz(matrizPrueba, 3, 5, memoriaLugar);

    delay(2000);
}

void loop() {
    int matrizRecuperada[MAX_FILAS][MAX_COLUMNAS] = {0};
    Serial.println("Recuperando matriz desde memoria...");
    recuperarLista(matrizRecuperada, "entrenamiento1");

    delay(5000); // Espera 5 segundos antes de recuperar nuevamente
}
