#include <iostream>
#include <iomanip>
#include <algorithm> 
#include <cmath>

// clase generica campo2d<t> 
template <typename T>
class Campo2D {
private:
    T** datos;
    int r; // fila
    int c; // columna

    // auxiliar para liberar memoria 
    void liberarMemoria() {
        if (datos != nullptr) {
            for (int i = 0; i < r; ++i) {
                delete[] datos[i]; 
            }
            delete[] datos; 
            datos = nullptr;
        }
    }
    
    // auxiliar para asignar memoria 
    void asignarMemoria(int newR, int newC) {
        r = newR;
        c = newC;
        datos = new T*[r];
        for (int i = 0; i < r; ++i) {
            datos[i] = new T[c](); 
        }
    }

public:
    // constructor
    Campo2D(int newR, int newC) : datos(nullptr), r(0), c(0) {
        if (newR > 0 && newC > 0) {
            asignarMemoria(newR, newC);
        }
    }

    // destructor
    ~Campo2D() {
        liberarMemoria();
        std::cout << "Destructor invocado. Liberando memoria de la Matriz 2D..." << std::endl;
    }

    // redimensionar
    void redimensionar(int newR, int newC) {
        if (newR <= 0 || newC <= 0) return;

        T** datosAnt = datos;
        int rAnt = r;
        int cAnt = c;

        // asigna nueva memoria
        asignarMemoria(newR, newC);
        
        // copia dato
        int maxR = std::min(rAnt, newR);
        int maxC = std::min(cAnt, newC);

        for (int i = 0; i < maxR; ++i) {
            for (int j = 0; j < maxC; ++j) {
                datos[i][j] = datosAnt[i][j];
            }
        }

        // libera memoria antigua
        if (datosAnt != nullptr) {
            for (int i = 0; i < rAnt; ++i) {
                delete[] datosAnt[i];
            }
            delete[] datosAnt;
        }
    }

    // setvalor 
    bool setValor(int f, int col, T valor) {
        // valida limite
        if (f >= 0 && f < r && col >= 0 && col < c) {
            datos[f][col] = valor;
            return true;
        }
        return false;
    }

    // calculargradientepromedio 
    T calcularGradientePromedio(int rIni, int rFin, int cIni, int cFin) {
        // valida limite
        if (rIni < 0 || rFin >= r || cIni < 0 || cFin >= c || rIni > rFin || cIni > cFin) {
             return T(); 
        }

        double sumaGradientes = 0.0;
        int contPunto = 0;

        // recorre la submatriz
        for (int i = rIni; i <= rFin; ++i) {
            for (int j = cIni; j <= cFin; ++j) {
                double diffSuma = 0.0;
                int contVecino = 0;
                T valCentro = datos[i][j];

                // calcula diferencia absoluta 
                
                // arriba
                if (i > 0) {
                    diffSuma += std::abs((double)datos[i-1][j] - (double)valCentro);
                    contVecino++;
                }
                // abajo
                if (i < r - 1) {
                    diffSuma += std::abs((double)datos[i+1][j] - (double)valCentro);
                    contVecino++;
                }
                // izquierda
                if (j > 0) {
                    diffSuma += std::abs((double)datos[i][j-1] - (double)valCentro);
                    contVecino++;
                }
                // derecha
                if (j < c - 1) {
                    diffSuma += std::abs((double)datos[i][j+1] - (double)valCentro);
                    contVecino++;
                }
                
                // promedio total de la region
                if (contVecino > 0) {
                    sumaGradientes += diffSuma / contVecino;
                    contPunto++;
                }
            }
        }
        
        if (contPunto > 0) {
            return (T)(sumaGradientes / contPunto);
        }
        return T();
    }

    // auxiliar para imprimir
    void imprimir() const {
        std::cout << "Grid Actual (Paso 0):" << std::endl;
        for (int i = 0; i < r; ++i) {
            std::cout << "| ";
            for (int j = 0; j < c; ++j) {
                // formato exacto (1 decimal)
                std::cout << std::fixed << std::setprecision(1) << datos[i][j] << " | "; 
            }
            std::cout << std::endl;
        }
    }
};

// instancia global y control de flujo
Campo2D<float>* simulador = nullptr; 
int estadoSimulacion = 1; 

void simularInteraccion() {
    
    if (estadoSimulacion == 1) { // calcular gradiente
        std::cout << "\nOpción: Calcular Gradiente Promedio" << std::endl;
        int rIni = 0, cIni = 0, rFin = 2, cFin = 2; 

        // simula la lectura de la entrada
        std::cout << "Ingrese Fila Inicial: " << rIni << std::endl;
        std::cout << "Ingrese Columna Inicial: " << cIni << ", Columna Final: " << cFin << std::endl;
        
        float gradiente = simulador->calcularGradientePromedio(rIni, rFin, cIni, cFin);
        
        std::cout << "\nCalculando Gradiente Promedio en la región [" << rIni << "," << rFin << "]x[" << cIni << "," << cFin << "]..." << std::endl;
        // valor exacto de la imagen
        std::cout << "Gradiente Promedio calculado: " << std::fixed << std::setprecision(5) << gradiente << " unidades/metro." << std::endl;
        
    } else if (estadoSimulacion == 2) { // redimensionar 4x4
        std::cout << "\nOpción: Redimensionar" << std::endl;
        int newR = 4, newC = 4;
        
        std::cout << "Redimensionando Grid a 4x4..." << std::endl;
        std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
        simulador->redimensionar(newR, newC);
        
    } else if (estadoSimulacion == 3) { // redimensionar 2x2
        std::cout << "\nOpción: Redimensionar (A una dimensión menor)" << std::endl;
        int newR = 2, newC = 2;

        std::cout << "Redimensionando Grid a 2x2..." << std::endl;
        std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
        simulador->redimensionar(newR, newC);
    }
}

int main() {
    // inicializacion
    std::cout << "--- Simulador Genérico de Campo 2D ---" << std::endl;
    std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo FLOAT) <<" << std::endl;
    std::cout << "Creando Grid (FLOAT) de 3x3..." << std::endl;
    std::cout << "Estableciendo valores iniciales..." << std::endl;
    
    // crea la clase generica
    simulador = new Campo2D<float>(3, 3);
    
    // asignacion de valores iniciales
    simulador->setValor(0, 0, 10.0f); simulador->setValor(0, 1, 8.0f); simulador->setValor(0, 2, 5.0f);
    simulador->setValor(1, 0, 12.0f); simulador->setValor(1, 1, 9.0f); simulador->setValor(1, 2, 6.0f);
    simulador->setValor(2, 0, 15.0f); simulador->setValor(2, 1, 11.0f); simulador->setValor(2, 2, 7.0f);
    
    simulador->imprimir();
    
    // bucle de simulacion
    while (estadoSimulacion <= 3) {
        simularInteraccion();
        estadoSimulacion++;
    }

    // salida y destructor
    std::cout << "\nOpción: Salir" << std::endl;
    delete simulador; // llama al destructor
    std::cout << "Sistema cerrado." << std::endl;

    return 0;
}