#include <iostream>  // Incluye la biblioteca para entrada y salida estándar (cin, cout)
#include <string>     // Incluye la biblioteca para manejo de cadenas de texto

using namespace std;  // Permite usar elementos del espacio de nombres std sin el prefijo

// ==================== FUNCIONES DE VALIDACIÓN ====================

// Función para leer un entero válido con validación de rango
int leerEnteroValido(const string& mensaje, int min = INT_MIN, int max = INT_MAX) {
    int valor;                    // Variable para almacenar el valor ingresado
    while (true) {                // Ciclo infinito hasta obtener entrada válida
        cout << mensaje;          // Muestra el mensaje al usuario
        if (cin >> valor) {       // Intenta leer un entero
            // Verifica si el valor está dentro del rango permitido
            if (valor >= min && valor <= max) {
                return valor;     // Retorna el valor si es válido
            } else {
                // Mensaje de error si está fuera del rango
                cout << "Error: El valor debe estar entre " << min << " y " << max << ".\n";
            }
        } else {
            // Mensaje de error si no se ingresó un número válido
            cout << "Error: Ingrese un numero valido.\n";
            cin.clear();          // Limpia el estado de error de cin
            cin.ignore(10000, '\n'); // Ignora caracteres hasta encontrar nueva línea
        }
    }
}

// Función para leer una cadena que no esté vacía
string leerStringNoVacio(const string& mensaje) {
    string texto;                 // Variable para almacenar el texto ingresado
    while (true) {                // Ciclo hasta obtener texto válido
        cout << mensaje;          // Muestra el mensaje al usuario
        getline(cin, texto);      // Lee toda la línea incluyendo espacios
        // Verifica que no esté vacía y tenga al menos un carácter no blanco
        if (!texto.empty() && texto.find_first_not_of(" \t\n\r") != string::npos) {
            return texto;         // Retorna el texto si es válido
        }
        cout << "Error: El texto no puede estar vacio.\n"; // Mensaje de error
    }
}

// Función específica para leer opciones del menú
int leerOpcionMenu(int min, int max) {
    return leerEnteroValido("", min, max); // Llama a leerEnteroValido sin mensaje
}

// ==================== ESTRUCTURA DE PROCESO ====================

// Estructura que representa un proceso del sistema
struct Proceso {
    int id;                       // Identificador único del proceso
    string nombre;                // Nombre descriptivo del proceso
    int prioridad;                // Prioridad del proceso (1-10)
    Proceso* siguiente;           // Puntero al siguiente proceso en la lista
    
    // Constructor para facilitar la creación de procesos
    Proceso(int _id, string _nombre, int _prioridad) 
        : id(_id), nombre(_nombre), prioridad(_prioridad), siguiente(NULL) {}
        // Inicializa los miembros y pone siguiente en NULL
};

// ==================== GESTOR DE PROCESOS ====================

// Clase que maneja una lista enlazada de procesos
class GestorProcesos {
private:
    Proceso* cabeza;              // Puntero al primer proceso de la lista

public:
    // Constructor: inicializa la lista vacía
    GestorProcesos() : cabeza(NULL) {}
    
    // Destructor: libera toda la memoria utilizada
    ~GestorProcesos() {
        while (cabeza != NULL) {  // Mientras haya procesos en la lista
            Proceso* temp = cabeza;    // Guarda referencia al proceso actual
            cabeza = cabeza->siguiente; // Avanza al siguiente proceso
            delete temp;               // Libera la memoria del proceso actual
        }
    }

    // Método para insertar un nuevo proceso al inicio de la lista
    void insertar(int id, string nombre, int prioridad) {
        // Verifica si ya existe un proceso con el mismo ID
        if (buscar(id) != NULL) {
            cout << "Error: Ya existe un proceso con ID " << id << ".\n";
            return;               // Sale del método si el ID ya existe
        }
        
        // Crea un nuevo proceso con los datos proporcionados
        Proceso* nuevo = new Proceso(id, nombre, prioridad);
        nuevo->siguiente = cabeza; // El nuevo proceso apunta al anterior primero
        cabeza = nuevo;           // El nuevo proceso se convierte en el primero
        cout << "Proceso insertado exitosamente.\n"; // Confirmación
    }

    // Método para eliminar un proceso por su ID
    void eliminar(int id) {
        if (cabeza == NULL) {     // Verifica si la lista está vacía
            cout << "No hay procesos para eliminar.\n";
            return;
        }
        
        Proceso* actual = cabeza;  // Puntero para recorrer la lista
        Proceso* anterior = NULL;  // Puntero al proceso anterior
        
        // Recorre la lista buscando el proceso con el ID especificado
        while (actual != NULL) {
            if (actual->id == id) { // Si encuentra el proceso
                if (anterior == NULL) { // Si es el primer proceso
                    cabeza = actual->siguiente; // La cabeza apunta al siguiente
                } else {                        // Si no es el primer proceso
                    anterior->siguiente = actual->siguiente; // Salta el proceso actual
                }
                delete actual;      // Libera la memoria del proceso
                cout << "Proceso con ID " << id << " eliminado exitosamente.\n";
                return;             // Sale del método
            }
            anterior = actual;      // Actualiza el puntero anterior
            actual = actual->siguiente; // Avanza al siguiente proceso
        }
        cout << "Proceso con ID " << id << " no encontrado.\n"; // Si no lo encuentra
    }

    // Método para buscar un proceso por ID
    Proceso* buscar(int id) {
        Proceso* actual = cabeza;  // Comienza desde el primer proceso
        while (actual != NULL) {   // Recorre toda la lista
            if (actual->id == id)  // Si encuentra el ID buscado
                return actual;     // Retorna el puntero al proceso
            actual = actual->siguiente; // Avanza al siguiente proceso
        }
        return NULL;               // Retorna NULL si no lo encuentra
    }

    // Sobrecarga del método buscar para buscar por nombre
    Proceso* buscar(const string& nombre) {
        Proceso* actual = cabeza;  // Comienza desde el primer proceso
        while (actual != NULL) {   // Recorre toda la lista
            if (actual->nombre == nombre) // Si encuentra el nombre buscado
                return actual;     // Retorna el puntero al proceso
            actual = actual->siguiente; // Avanza al siguiente proceso
        }
        return NULL;               // Retorna NULL si no lo encuentra
    }

    // Método para modificar la prioridad de un proceso
    void modificarPrioridad(int id, int nuevaPrioridad) {
        // Valida que la prioridad esté en el rango correcto
        if (nuevaPrioridad < 1 || nuevaPrioridad > 10) {
            cout << "Error: La prioridad debe estar entre 1 y 10.\n";
            return;
        }
        
        Proceso* p = buscar(id);   // Busca el proceso por ID
        if (p != NULL) {           // Si encuentra el proceso
            int prioridadAnterior = p->prioridad; // Guarda la prioridad anterior
            p->prioridad = nuevaPrioridad;        // Asigna la nueva prioridad
            // Muestra confirmación con valores anterior y nuevo
            cout << "Prioridad del proceso ID " << id << " modificada de " 
                 << prioridadAnterior << " a " << nuevaPrioridad << ".\n";
        } else {
            cout << "Proceso con ID " << id << " no encontrado.\n";
        }
    }

    // Método para mostrar todos los procesos
    void mostrarProcesos() {
        if (cabeza == NULL) {      // Verifica si la lista está vacía
            cout << "\nNo hay procesos registrados.\n";
            return;
        }
        
        Proceso* actual = cabeza;  // Comienza desde el primer proceso
        cout << "\n=== Lista de procesos ===\n"; // Encabezado
        while (actual != NULL) {   // Recorre toda la lista
            // Muestra la información de cada proceso
            cout << "ID: " << actual->id 
                 << ", Nombre: " << actual->nombre 
                 << ", Prioridad: " << actual->prioridad << endl;
            actual = actual->siguiente; // Avanza al siguiente proceso
        }
        cout << "========================\n"; // Pie de la lista
    }
    
    // Método para verificar si la lista está vacía
    bool estaVacio() const {
        return cabeza == NULL;     // Retorna true si cabeza es NULL
    }
};

// ==================== PLANIFICADOR DE CPU ====================

// Clase que implementa una cola de prioridad para procesos
class PlanificadorCPU {
private:
    // Estructura interna para los nodos de la cola
    struct NodoCola {
        Proceso* proceso;          // Puntero al proceso
        NodoCola* siguiente;       // Puntero al siguiente nodo en la cola
        
        // Constructor del nodo
        NodoCola(Proceso* p) : proceso(p), siguiente(NULL) {}
    };
    
    NodoCola* cabeza;             // Puntero al primer nodo de la cola
    int size;                     // Contador del tamaño de la cola

public:
    // Constructor: inicializa la cola vacía
    PlanificadorCPU() : cabeza(NULL), size(0) {}
    
    // Destructor: libera toda la memoria de la cola
    ~PlanificadorCPU() {
        while (cabeza != NULL) {   // Mientras haya nodos en la cola
            NodoCola* temp = cabeza;    // Guarda referencia al nodo actual
            cabeza = cabeza->siguiente; // Avanza al siguiente nodo
            delete temp;               // Libera la memoria del nodo actual
        }
    }

    // Método para encolar un proceso manteniendo orden por prioridad
    void encolar(Proceso* p) {
        if (p == NULL) {          // Valida que el proceso no sea nulo
            cout << "Error: No se puede encolar un proceso nulo.\n";
            return;
        }
        
        NodoCola* nuevo = new NodoCola(p); // Crea un nuevo nodo
        
        // Insertar ordenado por prioridad (mayor prioridad primero)
        if (cabeza == NULL || p->prioridad > cabeza->proceso->prioridad) {
            // Si la cola está vacía o tiene mayor prioridad que el primero
            nuevo->siguiente = cabeza; // El nuevo nodo apunta al anterior primero
            cabeza = nuevo;           // El nuevo nodo se convierte en el primero
        } else {
            // Busca la posición correcta para insertar
            NodoCola* actual = cabeza;
            // Avanza hasta encontrar un proceso de menor prioridad
            while (actual->siguiente != NULL && 
                   actual->siguiente->proceso->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            // Inserta el nuevo nodo en la posición encontrada
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        
        size++;                   // Incrementa el contador de tamaño
        // Confirmación de encolado
        cout << "Proceso '" << p->nombre << "' (ID: " << p->id 
             << ") encolado para ejecución.\n";
    }

    // Método para ejecutar (desencoler) el proceso de mayor prioridad
    void ejecutar() {
        if (cabeza == NULL) {     // Verifica si la cola está vacía
            cout << "No hay procesos en cola para ejecutar.\n";
            return;
        }
        
        NodoCola* temp = cabeza;  // Guarda referencia al primer nodo
        Proceso* p = temp->proceso; // Obtiene el proceso del primer nodo
        cabeza = cabeza->siguiente; // La cabeza apunta al siguiente nodo
        delete temp;              // Libera la memoria del nodo
        size--;                   // Decrementa el contador de tamaño
        
        // Muestra información del proceso ejecutado
        cout << ">>> Ejecutando proceso: ID " << p->id 
             << ", Nombre: '" << p->nombre 
             << "', Prioridad: " << p->prioridad << " <<<\n";
    }

    // Método para mostrar todos los procesos en la cola
    void mostrarCola() {
        if (cabeza == NULL) {     // Verifica si la cola está vacía
            cout << "\nLa cola de procesos está vacía.\n";
            return;
        }
        
        NodoCola* actual = cabeza; // Comienza desde el primer nodo
        cout << "\n=== Cola de procesos (por prioridad) ===\n"; // Encabezado
        int posicion = 1;         // Contador de posición
        while (actual != NULL) {  // Recorre toda la cola
            // Muestra información de cada proceso con su posición
            cout << posicion++ << ". ID: " << actual->proceso->id 
                 << ", Nombre: '" << actual->proceso->nombre 
                 << "', Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente; // Avanza al siguiente nodo
        }
        cout << "=======================================\n"; // Pie de la lista
    }
    
    // Método para verificar si la cola está vacía
    bool estaVacia() const {
        return cabeza == NULL;    // Retorna true si cabeza es NULL
    }
    
    // Método para obtener el tamaño de la cola
    int tamanio() const {
        return size;              // Retorna el contador de tamaño
    }
};

// ==================== GESTOR DE MEMORIA ====================

// Clase que simula la gestión de memoria del sistema
class GestorMemoria {
private:
    // Estructura para representar un bloque de memoria
    struct BloqueMemoria {
        int id;                   // Identificador del bloque
        BloqueMemoria* siguiente; // Puntero al siguiente bloque
        
        // Constructor del bloque
        BloqueMemoria(int _id) : id(_id), siguiente(NULL) {}
    };
    
    BloqueMemoria* cabeza;        // Puntero al primer bloque asignado
    int contador;                 // Contador para generar IDs únicos
    int totalBloques;             // Contador total de bloques asignados
    static const int MAX_BLOQUES = 100; // Límite máximo de bloques

public:
    // Constructor: inicializa el gestor vacío
    GestorMemoria() : cabeza(NULL), contador(0), totalBloques(0) {}
    
    // Destructor: libera todos los bloques de memoria
    ~GestorMemoria() {
        while (cabeza != NULL) {  // Mientras haya bloques asignados
            BloqueMemoria* temp = cabeza;    // Guarda referencia al bloque actual
            cabeza = cabeza->siguiente;      // Avanza al siguiente bloque
            delete temp;                     // Libera la memoria del bloque actual
        }
    }

    // Método para asignar un nuevo bloque de memoria
    void asignarMemoria() {
        // Verifica que no se haya alcanzado el límite máximo
        if (totalBloques >= MAX_BLOQUES) {
            cout << "Error: Se ha alcanzado el límite máximo de bloques de memoria ("
                 << MAX_BLOQUES << ").\n";
            return;
        }
        
        ++contador;               // Incrementa el contador para el nuevo ID
        // Crea un nuevo bloque con el ID generado
        BloqueMemoria* nuevo = new BloqueMemoria(contador);
        nuevo->siguiente = cabeza; // El nuevo bloque apunta al anterior primero
        cabeza = nuevo;           // El nuevo bloque se convierte en el primero
        totalBloques++;           // Incrementa el contador total
        
        // Confirmación de asignación
        cout << "Bloque de memoria #" << contador << " asignado exitosamente.\n";
    }

    // Método para liberar el bloque más reciente (LIFO - Last In, First Out)
    void liberarMemoria() {
        if (cabeza == NULL) {     // Verifica si hay bloques para liberar
            cout << "No hay bloques de memoria asignados para liberar.\n";
            return;
        }
        
        BloqueMemoria* temp = cabeza; // Guarda referencia al primer bloque
        int bloqueId = temp->id;      // Obtiene el ID del bloque a liberar
        cabeza = cabeza->siguiente;   // La cabeza apunta al siguiente bloque
        delete temp;                  // Libera la memoria del bloque
        totalBloques--;               // Decrementa el contador total
        
        // Confirmación de liberación
        cout << "Bloque de memoria #" << bloqueId << " liberado exitosamente.\n";
    }

    // Método para mostrar el estado actual de la memoria
    void mostrarEstado() {
        if (cabeza == NULL) {     // Verifica si hay bloques asignados
            cout << "\nNo hay bloques de memoria asignados.\n";
            return;
        }
        
        cout << "\n=== Estado actual de la memoria ===\n"; // Encabezado
        cout << "Bloques asignados (del más reciente al más antiguo):\n";
        
        BloqueMemoria* actual = cabeza; // Comienza desde el primer bloque
        while (actual != NULL) {        // Recorre todos los bloques
            cout << "Bloque #" << actual->id << endl; // Muestra el ID del bloque
            actual = actual->siguiente; // Avanza al siguiente bloque
        }
        
        // Muestra el total de bloques asignados
        cout << "Total de bloques asignados: " << totalBloques << endl;
        cout << "==================================\n"; // Pie del estado
    }
    
    // Método para obtener el número de bloques asignados
    int bloquesAsignados() const {
        return totalBloques;      // Retorna el contador total
    }
};

// ==================== FUNCIÓN DEL MENÚ ====================

// Función para mostrar el menú principal del simulador
void mostrarMenu() {
    cout << "\n========== SIMULADOR DE SISTEMA OPERATIVO ==========\n";
    cout << "1. Insertar proceso\n";              // Opción para crear nuevo proceso
    cout << "2. Eliminar proceso\n";              // Opción para eliminar proceso
    cout << "3. Buscar proceso\n";                // Opción para buscar proceso
    cout << "4. Modificar prioridad\n";           // Opción para cambiar prioridad
    cout << "5. Mostrar todos los procesos\n";    // Opción para listar procesos
    cout << "6. Encolar proceso para ejecucion\n"; // Opción para añadir a cola
    cout << "7. Ejecutar proceso\n";              // Opción para ejecutar proceso
    cout << "8. Mostrar cola de ejecucion\n";     // Opción para ver cola
    cout << "9. Asignar memoria\n";               // Opción para asignar memoria
    cout << "10. Liberar memoria\n";              // Opción para liberar memoria
    cout << "11. Mostrar estado de memoria\n";    // Opción para ver estado memoria
    cout << "0. Salir\n";                         // Opción para salir del programa
    cout << "===================================================\n";
    cout << "Seleccione una opcion: ";            // Prompt para selección
}

// ==================== FUNCIÓN PRINCIPAL ====================

int main() {
    // Declaración de objetos principales del simulador
    GestorProcesos gestor;        // Gestor para manejar la lista de procesos
    PlanificadorCPU planificador; // Planificador para la cola de ejecución
    GestorMemoria memoria;        // Gestor para simular la memoria
    
    int opcion;                   // Variable para almacenar la opción del usuario
    
    // Bucle principal del programa
    do {
        mostrarMenu();            // Muestra el menú de opciones
        opcion = leerOpcionMenu(0, 11); // Lee la opción del usuario (0-11)
        
        // Switch para manejar cada opción del menú
        switch(opcion) {
            case 1: {             // INSERTAR PROCESO
                cout << "\n--- Insertar Proceso ---\n";
                // Lee y valida el ID del proceso (números positivos)
                int id = leerEnteroValido("ID del proceso (numero positivo): ", 1, INT_MAX);
                cin.ignore();     // Limpia el buffer de entrada
                // Lee el nombre del proceso (no puede estar vacío)
                string nombre = leerStringNoVacio("Nombre del proceso: ");
                // Lee y valida la prioridad (1-10)
                int prioridad = leerEnteroValido("Prioridad (1-10): ", 1, 10);
                // Inserta el proceso en el gestor
                gestor.insertar(id, nombre, prioridad);
                break;
            }
            case 2: {             // ELIMINAR PROCESO
                cout << "\n--- Eliminar Proceso ---\n";
                if (gestor.estaVacio()) { // Verifica si hay procesos
                    cout << "No hay procesos para eliminar.\n";
                    break;
                }
                // Lee el ID del proceso a eliminar
                int id = leerEnteroValido("ID del proceso a eliminar: ", 1, INT_MAX);
                gestor.eliminar(id); // Elimina el proceso
                break;
            }
            case 3: {             // BUSCAR PROCESO
                cout << "\n--- Buscar Proceso ---\n";
                if (gestor.estaVacio()) { // Verifica si hay procesos
                    cout << "No hay procesos registrados para buscar.\n";
                    break;
                }
                // Pregunta el tipo de búsqueda (por ID o por nombre)
                int opcionBusqueda = leerEnteroValido("1. Buscar por ID\n2. Buscar por nombre\nSeleccione opcion: ", 1, 2);
                
                if (opcionBusqueda == 1) { // BÚSQUEDA POR ID
                    int id = leerEnteroValido("ID del proceso: ", 1, INT_MAX);
                    Proceso* p = gestor.buscar(id); // Busca el proceso
                    if (p != NULL) {        // Si encuentra el proceso
                        cout << "Proceso encontrado:\n";
                        // Muestra la información del proceso encontrado
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                } else {                   // BÚSQUEDA POR NOMBRE
                    cin.ignore();          // Limpia el buffer de entrada
                    string nombre = leerStringNoVacio("Nombre del proceso: ");
                    Proceso* p = gestor.buscar(nombre); // Busca el proceso
                    if (p != NULL) {       // Si encuentra el proceso
                        cout << "Proceso encontrado:\n";
                        // Muestra la información del proceso encontrado
                        cout << "ID: " << p->id << ", Nombre: " << p->nombre 
                             << ", Prioridad: " << p->prioridad << endl;
                    } else {
                        cout << "Proceso no encontrado.\n";
                    }
                }
                break;
            }
            case 4: {             // MODIFICAR PRIORIDAD
                cout << "\n--- Modificar Prioridad ---\n";
                if (gestor.estaVacio()) { // Verifica si hay procesos
                    cout << "No hay procesos registrados para modificar.\n";
                    break;
                }
                // Lee el ID del proceso a modificar
                int id = leerEnteroValido("ID del proceso: ", 1, INT_MAX);
                // Lee la nueva prioridad
                int nuevaPrioridad = leerEnteroValido("Nueva prioridad (1-10): ", 1, 10);
                // Modifica la prioridad del proceso
                gestor.modificarPrioridad(id, nuevaPrioridad);
                break;
            }
            case 5: {             // MOSTRAR TODOS LOS PROCESOS
                cout << "\n--- Lista de Procesos ---\n";
                gestor.mostrarProcesos(); // Muestra todos los procesos registrados
                break;
            }
            case 6: {             // ENCOLAR PROCESO
                cout << "\n--- Encolar Proceso ---\n";
                if (gestor.estaVacio()) { // Verifica si hay procesos
                    cout << "No hay procesos registrados para encolar.\n";
                    break;
                }
                // Lee el ID del proceso a encolar
                int id = leerEnteroValido("ID del proceso a encolar: ", 1, INT_MAX);
                Proceso* p = gestor.buscar(id); // Busca el proceso
                if (p != NULL) {          // Si encuentra el proceso
                    planificador.encolar(p); // Lo encola para ejecución
                } else {
                    cout << "Proceso con ID " << id << " no encontrado.\n";
                }
                break;
            }
            case 7: {             // EJECUTAR PROCESO
                cout << "\n--- Ejecutar Proceso ---\n";
                planificador.ejecutar();  // Ejecuta el proceso de mayor prioridad
                break;
            }
            case 8: {             // MOSTRAR COLA DE EJECUCIÓN
                cout << "\n--- Cola de Ejecucion ---\n";
                planificador.mostrarCola(); // Muestra la cola de procesos
                break;
            }
            case 9: {             // ASIGNAR MEMORIA
                cout << "\n--- Asignar Memoria ---\n";
                memoria.asignarMemoria(); // Asigna un nuevo bloque de memoria
                break;
            }
            case 10: {            // LIBERAR MEMORIA
                cout << "\n--- Liberar Memoria ---\n";
                memoria.liberarMemoria(); // Libera el bloque más reciente
                break;
            }
            case 11: {            // MOSTRAR ESTADO DE MEMORIA
                cout << "\n--- Estado de Memoria ---\n";
                memoria.mostrarEstado();  // Muestra el estado actual de la memoria
                break;
            }
            case 0: {             // SALIR DEL PROGRAMA
                cout << "\n¡Gracias por usar el Simulador de Sistema Operativo!\n";
                cout << "Saliendo del programa...\n";
                break;
            }
        }
        
        // Pausa para que el usuario pueda ver el resultado
        if (opcion != 0) {        // Si no es la opción de salir
            cout << "\nPresione Enter para continuar..."; // Mensaje de pausa
            cin.ignore();         // Ignora cualquier carácter pendiente
            cin.get();            // Espera a que el usuario presione Enter
        }
        
    } while(opcion != 0);         // Continúa hasta que el usuario elija salir

    return 0;                     // Retorna 0 indicando ejecución exitosa
}
