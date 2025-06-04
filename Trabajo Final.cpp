#include <iostream>
#include <string>

using namespace std;

// Funciones de validación
int leerEnteroValido(const string& mensaje, int min = INT_MIN, int max = INT_MAX) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                return valor;
            } else {
                cout << "Error: El valor debe estar entre " << min << " y " << max << ".\n";
            }
        } else {
            cout << "Error: Ingrese un numero valido.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

string leerStringNoVacio(const string& mensaje) {
    string texto;
    while (true) {
        cout << mensaje;
        getline(cin, texto);
        if (!texto.empty() && texto.find_first_not_of(" \t\n\r") != string::npos) {
            return texto;
        }
        cout << "Error: El texto no puede estar vacio.\n";
    }
}

int leerOpcionMenu(int min, int max) {
    return leerEnteroValido("", min, max);
}

// Estructura de Proceso
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* siguiente;
    
    // Constructor para facilitar la creación
    Proceso(int _id, string _nombre, int _prioridad) 
        : id(_id), nombre(_nombre), prioridad(_prioridad), siguiente(NULL) {}
};

// Lista enlazada para el Gestor de Procesos
class GestorProcesos {
private:
    Proceso* cabeza;

public:
    // Constructor
    GestorProcesos() : cabeza(NULL) {}
    
    // Destructor para liberar memoria
    ~GestorProcesos() {
        while (cabeza != NULL) {
            Proceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void insertar(int id, string nombre, int prioridad) {
        // Verificar si el ID ya existe
        if (buscar(id) != NULL) {
            cout << "Error: Ya existe un proceso con ID " << id << ".\n";
            return;
        }
        
        Proceso* nuevo = new Proceso(id, nombre, prioridad);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cout << "Proceso insertado exitosamente.\n";
    }

    void eliminar(int id) {
        if (cabeza == NULL) {
            cout << "No hay procesos para eliminar.\n";
            return;
        }
        
        Proceso* actual = cabeza;
        Proceso* anterior = NULL;
        
        while (actual != NULL) {
            if (actual->id == id) {
                if (anterior == NULL) {
                    cabeza = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                cout << "Proceso con ID " << id << " eliminado exitosamente.\n";
                return;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        cout << "Proceso con ID " << id << " no encontrado.\n";
    }

    Proceso* buscar(int id) {
        Proceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->id == id)
                return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

    Proceso* buscar(const string& nombre) {
        Proceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->nombre == nombre)
                return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        if (nuevaPrioridad < 1 || nuevaPrioridad > 10) {
            cout << "Error: La prioridad debe estar entre 1 y 10.\n";
            return;
        }
        
        Proceso* p = buscar(id);
        if (p != NULL) {
            int prioridadAnterior = p->prioridad;
            p->prioridad = nuevaPrioridad;
            cout << "Prioridad del proceso ID " << id << " modificada de " 
                 << prioridadAnterior << " a " << nuevaPrioridad << ".\n";
        } else {
            cout << "Proceso con ID " << id << " no encontrado.\n";
        }
    }

    void mostrarProcesos() {
        if (cabeza == NULL) {
            cout << "\nNo hay procesos registrados.\n";
            return;
        }
        
        Proceso* actual = cabeza;
        cout << "\n=== Lista de procesos ===\n";
        while (actual != NULL) {
            cout << "ID: " << actual->id 
                 << ", Nombre: " << actual->nombre 
                 << ", Prioridad: " << actual->prioridad << endl;
            actual = actual->siguiente;
        }
        cout << "========================\n";
    }
    
    bool estaVacio() const {
        return cabeza == NULL;
    }
};

// Planificador de CPU con cola personalizada ordenada por prioridad
class PlanificadorCPU {
private:
    struct NodoCola {
        Proceso* proceso;
        NodoCola* siguiente;
        
        NodoCola(Proceso* p) : proceso(p), siguiente(NULL) {}
    };
    
    NodoCola* cabeza;
    int size;

public:
    PlanificadorCPU() : cabeza(NULL), size(0) {}
    
    ~PlanificadorCPU() {
        while (cabeza != NULL) {
            NodoCola* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void encolar(Proceso* p) {
        if (p == NULL) {
            cout << "Error: No se puede encolar un proceso nulo.\n";
            return;
        }
        
        NodoCola* nuevo = new NodoCola(p);
        
        // Insertar ordenado por prioridad (mayor prioridad primero)
        if (cabeza == NULL || p->prioridad > cabeza->proceso->prioridad) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        } else {
            NodoCola* actual = cabeza;
            while (actual->siguiente != NULL && 
                   actual->siguiente->proceso->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        
        size++;
        cout << "Proceso '" << p->nombre << "' (ID: " << p->id 
             << ") encolado para ejecución.\n";
    }

    void ejecutar() {
        if (cabeza == NULL) {
            cout << "No hay procesos en cola para ejecutar.\n";
            return;
        }
        
        NodoCola* temp = cabeza;
        Proceso* p = temp->proceso;
        cabeza = cabeza->siguiente;
        delete temp;
        size--;
        
        cout << ">>> Ejecutando proceso: ID " << p->id 
             << ", Nombre: '" << p->nombre 
             << "', Prioridad: " << p->prioridad << " <<<\n";
    }

    void mostrarCola() {
        if (cabeza == NULL) {
            cout << "\nLa cola de procesos está vacía.\n";
            return;
        }
        
        NodoCola* actual = cabeza;
        cout << "\n=== Cola de procesos (por prioridad) ===\n";
        int posicion = 1;
        while (actual != NULL) {
            cout << posicion++ << ". ID: " << actual->proceso->id 
                 << ", Nombre: '" << actual->proceso->nombre 
                 << "', Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
        }
        cout << "=======================================\n";
    }
    
    bool estaVacia() const {
        return cabeza == NULL;
    }
    
    int tamanio() const {
        return size;
    }
};

// Gestor de Memoria simplificado usando lista enlazada
class GestorMemoria {
private:
    struct BloqueMemoria {
        int id;
        BloqueMemoria* siguiente;
        
        BloqueMemoria(int _id) : id(_id), siguiente(NULL) {}
    };
    
    BloqueMemoria* cabeza;
    int contador;
    int totalBloques;
    static const int MAX_BLOQUES = 100;

public:
    GestorMemoria() : cabeza(NULL), contador(0), totalBloques(0) {}
    
    ~GestorMemoria() {
        while (cabeza != NULL) {
            BloqueMemoria* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void asignarMemoria() {
        if (totalBloques >= MAX_BLOQUES) {
            cout << "Error: Se ha alcanzado el límite máximo de bloques de memoria ("
                 << MAX_BLOQUES << ").\n";
            return;
        }
        
        ++contador;
        BloqueMemoria* nuevo = new BloqueMemoria(contador);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        totalBloques++;
        
        cout << "Bloque de memoria #" << contador << " asignado exitosamente.\n";
    }

    void liberarMemoria() {
        if (cabeza == NULL) {
            cout << "No hay bloques de memoria asignados para liberar.\n";
            return;
        }
        
        BloqueMemoria* temp = cabeza;
        int bloqueId = temp->id;
        cabeza = cabeza->siguiente;
        delete temp;
        totalBloques--;
        
        cout << "Bloque de memoria #" << bloqueId << " liberado exitosamente.\n";
    }

    void mostrarEstado() {
        if (cabeza == NULL) {
            cout << "\nNo hay bloques de memoria asignados.\n";
            return;
        }
        
        cout << "\n=== Estado actual de la memoria ===\n";
        cout << "Bloques asignados (del más reciente al más antiguo):\n";
        
        BloqueMemoria* actual = cabeza;
        while (actual != NULL) {
            cout << "Bloque #" << actual->id << endl;
            actual = actual->siguiente;
        }
        
        cout << "Total de bloques asignados: " << totalBloques << endl;
        cout << "==================================\n";
    }
    
    int bloquesAsignados() const {
        return totalBloques;
    }
};

// Función para mostrar menú
void mostrarMenu() {
    cout << "\n========== SIMULADOR DE SISTEMA OPERATIVO ==========\n";
    cout << "1. Insertar proceso\n";
    cout << "2. Eliminar proceso\n";
    cout << "3. Buscar proceso\n";
    cout << "4. Modificar prioridad\n";
    cout << "5. Mostrar todos los procesos\n";
    cout << "6. Encolar proceso para ejecucion\n";
    cout << "7. Ejecutar proceso\n";
    cout << "8. Mostrar cola de ejecucion\n";
    cout << "9. Asignar memoria\n";
    cout << "10. Liberar memoria\n";
    cout << "11. Mostrar estado de memoria\n";
    cout << "0. Salir\n";
    cout << "===================================================\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    GestorProcesos gestor;
    PlanificadorCPU planificador;
    GestorMemoria memoria;
    
    int opcion;
    do {
        mostrarMenu();
        opcion = leerOpcionMenu(0, 11);
        
        switch(opcion) {
            case 1: {
                cout << "\n--- Insertar Proceso ---\n";
                int id = leerEnteroValido("ID del proceso (numero positivo): ", 1, INT_MAX);
                cin.ignore(); // Limpiar el buffer
                string nombre = leerStringNoVacio("Nombre del proceso: ");
                int prioridad = leerEnteroValido("Prioridad (1-10): ", 1, 10);
                gestor.insertar(id, nombre, prioridad);
                break;
            }
            case 2: {
                cout << "\n--- Eliminar Proceso ---\n";
                if (gestor.estaVacio()) {
                    cout << "No hay procesos para eliminar.\n";
                    break;
                }
                int id = leerEnteroValido("ID del proceso a eliminar: ", 1, INT_MAX);
                gestor.eliminar(id);
                break;
            }
            case 3: {
                cout << "\n--- Buscar Proceso ---\n";
                if (gestor.estaVacio()) {
                    cout << "No hay procesos registrados para buscar.\n";
                    break;
                }
                int opcionBusqueda = leerEnteroValido("1. Buscar por ID\n2. Buscar por nombre\nSeleccione opcion: ", 1, 2);
                
                if (opcionBusqueda == 1) {
                    int id = leerEnteroValido("ID del proceso: ", 1, INT_MAX);
                    Proceso* p = gestor.buscar(id);
                    if (p != NULL) {
                        cout << "Proceso encontrado:\n";
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                } else {
                    cin.ignore(); // Limpiar el buffer
                    string nombre = leerStringNoVacio("Nombre del proceso: ");
                    Proceso* p = gestor.buscar(nombre);
                    if (p != NULL) {
                        cout << "Proceso encontrado:\n";
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                }
                break;
            }
            case 4: {
                cout << "\n--- Modificar Prioridad ---\n";
                if (gestor.estaVacio()) {
                    cout << "No hay procesos registrados para modificar.\n";
                    break;
                }
                int id = leerEnteroValido("ID del proceso: ", 1, INT_MAX);
                int nuevaPrioridad = leerEnteroValido("Nueva prioridad (1-10): ", 1, 10);
                gestor.modificarPrioridad(id, nuevaPrioridad);
                break;
            }
            case 5: {
                cout << "\n--- Lista de Procesos ---\n";
                gestor.mostrarProcesos();
                break;
            }
            case 6: {
                cout << "\n--- Encolar Proceso ---\n";
                if (gestor.estaVacio()) {
                    cout << "No hay procesos registrados para encolar.\n";
                    break;
                }
                int id = leerEnteroValido("ID del proceso a encolar: ", 1, INT_MAX);
                Proceso* p = gestor.buscar(id);
                if (p != NULL) {
                    planificador.encolar(p);
                } else {
                    cout << "Proceso con ID " << id << " no encontrado.\n";
                }
                break;
            }
            case 7: {
                cout << "\n--- Ejecutar Proceso ---\n";
                planificador.ejecutar();
                break;
            }
            case 8: {
                cout << "\n--- Cola de Ejecucion ---\n";
                planificador.mostrarCola();
                break;
            }
            case 9: {
                cout << "\n--- Asignar Memoria ---\n";
                memoria.asignarMemoria();
                break;
            }
            case 10: {
                cout << "\n--- Liberar Memoria ---\n";
                memoria.liberarMemoria();
                break;
            }
            case 11: {
                cout << "\n--- Estado de Memoria ---\n";
                memoria.mostrarEstado();
                break;
            }
            case 0: {
                cout << "\n¡Gracias por usar el Simulador de Sistema Operativo!\n";
                cout << "Saliendo del programa...\n";
                break;
            }
        }
        
        // Pausa para que el usuario pueda ver el resultado
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 0);

    return 0;
}
