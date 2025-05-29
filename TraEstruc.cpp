#include <iostream>
#include <string>
#include <stack>

using namespace std;

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

// Planificador de CPU con lista ordenada por prioridad
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

// Gestor de Memoria con pila
class GestorMemoria {
private:
    stack<int> pilaMemoria;
    int contador;
    static const int MAX_BLOQUES = 100; // Límite máximo de bloques

public:
    GestorMemoria() : contador(0) {}

    void asignarMemoria() {
        if (contador >= MAX_BLOQUES) {
            cout << "Error: Se ha alcanzado el límite máximo de bloques de memoria ("
                 << MAX_BLOQUES << ").\n";
            return;
        }
        
        ++contador;
        pilaMemoria.push(contador);
        cout << "Bloque de memoria #" << contador << " asignado exitosamente.\n";
    }

    void liberarMemoria() {
        if (pilaMemoria.empty()) {
            cout << "No hay bloques de memoria asignados para liberar.\n";
            return;
        }
        
        int bloqueLiberated = pilaMemoria.top();
        pilaMemoria.pop();
        cout << "Bloque de memoria #" << bloqueLiberated << " liberado exitosamente.\n";
    }

    void mostrarEstado() {
        if (pilaMemoria.empty()) {
            cout << "\nNo hay bloques de memoria asignados.\n";
            return;
        }
        
        stack<int> temp = pilaMemoria;
        cout << "\n=== Estado actual de la memoria ===\n";
        cout << "Bloques asignados (del más reciente al más antiguo):\n";
        while (!temp.empty()) {
            cout << "Bloque #" << temp.top() << endl;
            temp.pop();
        }
        cout << "Total de bloques asignados: " << pilaMemoria.size() << endl;
        cout << "==================================\n";
    }
    
    size_t bloquesAsignados() const {
        return pilaMemoria.size();
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
    cout << "6. Encolar proceso para ejecución\n";
    cout << "7. Ejecutar proceso\n";
    cout << "8. Mostrar cola de ejecución\n";
    cout << "9. Asignar memoria\n";
    cout << "10. Liberar memoria\n";
    cout << "11. Mostrar estado de memoria\n";
    cout << "0. Salir\n";
    cout << "===================================================\n";
    cout << "Seleccione una opción: ";
}

int main() {
    GestorProcesos gestor;
    PlanificadorCPU planificador;
    GestorMemoria memoria;
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch(opcion) {
            case 1: {
                int id, prioridad;
                string nombre;
                cout << "\n--- Insertar Proceso ---\n";
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Nombre del proceso: ";
                cin >> nombre;
                cout << "Prioridad (1-10): ";
                cin >> prioridad;
                gestor.insertar(id, nombre, prioridad);
                break;
            }
            case 2: {
                int id;
                cout << "\n--- Eliminar Proceso ---\n";
                cout << "ID del proceso a eliminar: ";
                cin >> id;
                gestor.eliminar(id);
                break;
            }
            case 3: {
                int opcionBusqueda;
                cout << "\n--- Buscar Proceso ---\n";
                cout << "1. Buscar por ID\n";
                cout << "2. Buscar por nombre\n";
                cout << "Seleccione opción: ";
                cin >> opcionBusqueda;
                
                if (opcionBusqueda == 1) {
                    int id;
                    cout << "ID del proceso: ";
                    cin >> id;
                    Proceso* p = gestor.buscar(id);
                    if (p != NULL) {
                        cout << "Proceso encontrado:\n";
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                } else if (opcionBusqueda == 2) {
                    string nombre;
                    cout << "Nombre del proceso: ";
                    cin >> nombre;
                    Proceso* p = gestor.buscar(nombre);
                    if (p != NULL) {
                        cout << "Proceso encontrado:\n";
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                } else {
                    cout << "Opción inválida.\n";
                }
                break;
            }
            case 4: {
                int id, nuevaPrioridad;
                cout << "\n--- Modificar Prioridad ---\n";
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Nueva prioridad (1-10): ";
                cin >> nuevaPrioridad;
                gestor.modificarPrioridad(id, nuevaPrioridad);
                break;
            }
            case 5: {
                cout << "\n--- Lista de Procesos ---\n";
                gestor.mostrarProcesos();
                break;
            }
            case 6: {
                int id;
                cout << "\n--- Encolar Proceso ---\n";
                cout << "ID del proceso a encolar: ";
                cin >> id;
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
                cout << "\n--- Cola de Ejecución ---\n";
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
            default: {
                cout << "\nOpción inválida. Por favor seleccione una opción del 0 al 11.\n";
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
