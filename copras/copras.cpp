#include <iostream>
#include <fstream>
#include <iomanip>
#include <cfloat>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string.h>
using namespace std;


//Dos metodos, uno de ellos es el de leer el archivo y el otro es con valores ya definidos

extern "C" {
    __declspec(dllexport) float* resolverText(const char* ruta);
    __declspec(dllexport) float* copras(float** tabla, const char** beneficio, float* pesos, int filas, int columnas);
}

float* resolverText(const char* ruta) {
    vector<string> beneficio;
    vector<float> pesos;
    vector<vector<float>> tabla;
    string linea;
    ifstream archivo(ruta);
    if (archivo.is_open()) {
        // Leer la primera línea (cabecera)
        if (getline(archivo, linea)) {
            stringstream ss(linea);
            string valor;

            // Leer cada valor separado por comas
            while (getline(ss, valor, ',')) {
                beneficio.push_back(valor);
            }

            getline(archivo, linea);
            cout << linea << endl;
            stringstream dd(linea);
            while (getline(dd, valor, ',')) {
                pesos.push_back(stof(valor));
            }
        }

        while (getline(archivo, linea)) {
            vector<float> fila_valores;
            stringstream ss(linea);
            string valor;

            // Leer cada valor separado por comas
            while (getline(ss, valor, ',')) {
                fila_valores.push_back(stof(valor));
            }
            tabla.push_back(fila_valores);
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    archivo.close();

    // Convertir los datos a los tipos adecuados para la función copras
    int filas = tabla.size();
    int columnas = tabla[0].size();
    float** tabla_array = new float*[filas];
    for (int i = 0; i < filas; ++i) {
        tabla_array[i] = new float[columnas];
        for (int j = 0; j < columnas; ++j) {
            tabla_array[i][j] = tabla[i][j];
        }
    }

    const char** beneficio_array = new const char*[beneficio.size()];
    for (size_t i = 0; i < beneficio.size(); ++i) {
        beneficio_array[i] = beneficio[i].c_str();
    }

    float* pesos_array = new float[pesos.size()];
    for (size_t i = 0; i < pesos.size(); ++i) {
        pesos_array[i] = pesos[i];
    }

    float* result = copras(tabla_array, beneficio_array, pesos_array, filas, columnas);

    // Liberar memoria
    for (int i = 0; i < filas; ++i) {
        delete[] tabla_array[i];
    }
    delete[] tabla_array;
    delete[] beneficio_array;
    delete[] pesos_array;
    delete[] result;
    return result;
}

float* copras(float** tabla, const char** beneficio, float* pesos, int filas, int columnas) {
    vector<float> sPlus(filas);
    vector<float> sMinus(filas);
    vector<float> sDiv(filas);
    vector<float> qi(filas);
    vector<float> ui(filas);
    float sMin = FLT_MAX;
    vector<vector<float>> normalizada(filas, vector<float>(columnas));

    // Llenar la tabla normalizada
    for (int i = 0; i < columnas; i++) {
        float sum = 0;
        for (int j = 0; j < filas; j++) {
            sum += tabla[j][i];
        }
        for (int j = 0; j < filas; j++) {
            normalizada[j][i] = tabla[j][i] / sum;
        }
    }

    // Multiplicar por los pesos
    for (int i = 0; i < columnas; i++) {
        for (int j = 0; j < filas; j++) {
            normalizada[j][i] = normalizada[j][i] * pesos[i];
        }
    }

    // Calcular S+i y S-i
    for (int i = 0; i < columnas; i++) {
        if (strcmp(beneficio[i], "B")) {
            for (int k = 0; k < filas; k++) {
                
                sPlus[k] += normalizada[k][i];
            }
        } else {
            for (int k = 0; k < filas; k++) {
                sMinus[k] += normalizada[k][i];
            }
        }
    }

    // Obtener S-Min
    for (int i = 0; i < filas; i++) {
        if (sMinus[i] < sMin) {
            sMin = sMinus[i];
        }
    }

    // Obtener S -min/S -i
    for (int i = 0; i < filas; i++) {
        sDiv[i] = sMin / sMinus[i];
    }

    // Obtener Qi
    for (int i = 0; i < filas; i++) {
        float sum = 0;
        for (int j = 0; j < filas; j++) {
            sum += sMinus[j];
        }
        float arribadiv = sMin * sum;

        sum = 0;
        for (int j = 0; j < filas; j++) {
            sum += sDiv[j];
        }
        float abajodiv = sMinus[i] * sum;

        qi[i] = sPlus[i] + (arribadiv / abajodiv);
        cout << qi[i] << " ";
    }

    // Obtener Ui
    float maxQi = FLT_MIN;
    for (int i = 0; i < filas; i++) {
        if (qi[i] > maxQi) {
            maxQi = qi[i];
        }
    }
    cout << maxQi << "\n";
    for (int i = 0; i < filas; i++) {
        ui[i] = (qi[i] / maxQi) * 100;
    }
    
    // Convertir el resultado a un array de floats
    float* result = new float[ui.size()];
    for (size_t i = 0; i < ui.size(); ++i) {
        result[i] = ui[i];
    }

    return result;
}
int main(){

//Leer archivo
    char* ruta = "C:\\Users\\rober\\Desktop\\COPRAS\\ejemplo.txt";
        
    
/*
    beneficio[0] = "B";
    beneficio[1] = "B";
    beneficio[2] = "B";
    beneficio[3] = "N";
    beneficio[4] = "N";
    beneficio[5] = "N";
    beneficio[6] = "N";

    pesos[0] = 0.28;
    pesos[1] = 0.14;
    pesos[2] = 0.05;
    pesos[3] = 0.24;
    pesos[4] = 0.19;
    pesos[5] = 0.05;
    pesos[6] = 0.05;
    
    tabla[0][0] = 75.5;
    tabla[0][1] = 420;
    tabla[0][2] = 74.2;
    tabla[0][3] = 2.8;
    tabla[0][4] = 21.4;
    tabla[0][5] = 0.37;
    tabla[0][6] = 0.16;

    tabla[1][0] = 95;
    tabla[1][1] = 91;
    tabla[1][2] = 70;
    tabla[1][3] = 2.68;
    tabla[1][4] = 22.1;
    tabla[1][5] = 0.33;
    tabla[1][6] = 0.16;
    
    tabla[2][0] = 770;
    tabla[2][1] = 1365;
    tabla[2][2] = 189;
    tabla[2][3] = 7.9;
    tabla[2][4] = 16.9;
    tabla[2][5] = 0.04;
    tabla[2][6] = 0.08;

    tabla[3][0] = 187;
    tabla[3][1] = 1120;
    tabla[3][2] = 210;
    tabla[3][3] = 7.9;
    tabla[3][4] = 14.4;
    tabla[3][5] = 0.03;
    tabla[3][6] = 0.08;

    tabla[4][0] = 179;
    tabla[4][1] = 875;
    tabla[4][2] = 112;
    tabla[4][3] = 4.43;
    tabla[4][4] = 9.4;
    tabla[4][5] = 0.016;
    tabla[4][6] = 0.09;

    tabla[5][0] = 239;
    tabla[5][1] = 1190;
    tabla[5][2] = 217;
    tabla[5][3] = 8.51;
    tabla[5][4] = 11.5;
    tabla[5][5] = 0.31;
    tabla[5][6] = 0.07;

    tabla[6][0] = 273;
    tabla[6][1] = 200;
    tabla[6][2] = 112;
    tabla[6][3] = 8.53;
    tabla[6][4] = 19.9;
    tabla[6][5] = 0.29;
    tabla[6][6] = 0.06;
*/
    resolverText(ruta);
    return 0;
    
}