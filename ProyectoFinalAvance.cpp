#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>    // Para round()
#include <limits>   // Para validaciones de flujo de datos

using namespace std;

// =============================
// CONSTANTES GLOBALES
// =============================
const char* PATH_PRODUCTOS = "productos.dat";
const char* PATH_VENTAS = "ventas.dat";
const float IVA_FACTOR = 0.12f; // 12% de IVA

// =============================
// STRUCTS
// =============================
struct Producto {
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    int ventasAcumuladas;
    bool activo;
};

struct DetalleVenta {
    int codigoProducto;
    int cantidad;
    float precioUnitario;
    float subtotalDetalle;
};

struct Venta {
    int idVenta;
    DetalleVenta detalles[15]; // Máximo 15 productos por venta
    int cantidadDetalles;
    int totalArticulos;
    float subtotal;
    float iva;
    float descuento;
    float total;
    int mes; // Rango de 1 a 12
};

// =============================
// PROTOTIPOS DE FUNCIONES (Solo lo que está listo)
// =============================
void menu();
void gestionProducto();
void procesoVenta();

// MODULO 1: CRUD
void registrarProducto();
void listarProductos();
void buscarProductoCodigo();
void buscarProductoNombre(); 
void actualizarStock();
void actualizarPrecio();
void eliminarProducto();

// FUNCIONES AUXILIARES / VALIDACIONES
bool existeProducto(int codigo);
int generarIdVenta();

// =============================
// FUNCION PRINCIPAL
// =============================
int main() {
    menu();
    return 0;
}

// =============================
// MENU DEL SISTEMA
// =============================
void menu() {
    int opcion;
    do {
        cout << "\n===== SISTEMA DE GESTION DE VENTAS =====\n";
        cout << "1. Modulo de Gestion de Productos (CRUD)\n";
        cout << "2. Modulo de Proceso de Venta\n";
        cout << "3. [PROXIMA ENTREGA] Modulo de Reportes\n";
        cout << "4. [PROXIMA ENTREGA] Modulo de Utilidades\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";

        try {
            if (!(cin >> opcion)) throw runtime_error("Entrada no valida. Por favor ingrese un numero.");

            switch (opcion) {
                case 1: gestionProducto(); break;
                case 2: procesoVenta(); break;
                case 3: 
                case 4: 
                    cout << "Este modulo estara disponible en la entrega final del proyecto.\n"; 
                    break;
                case 5: cout << "Saliendo del sistema. Hasta luego...\n"; break;
                default: cout << "Opcion no valida. Intente nuevamente.\n"; break;
            }
        } catch (const runtime_error& e) {
            cout << "ERROR: " << e.what() << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            opcion = 0; 
        }
    } while (opcion != 5);
}

void gestionProducto() {
    int opcion;
    do {
        cout << "\n--- MODULO 1: GESTION DE PRODUCTOS ---\n";
        cout << "1. Registrar Producto\n";
        cout << "2. Listar Productos Activos\n";
        cout << "3. Buscar Producto por Codigo\n";
        cout << "4. Buscar Producto por Nombre\n";
        cout << "5. Actualizar Stock\n";
        cout << "6. Modificar Precio\n";
        cout << "7. Eliminar / Desactivar Producto\n";
        cout << "8. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";

        try {
            if (!(cin >> opcion)) throw runtime_error("Entrada no valida. Por favor ingrese un numero.");

            switch (opcion) {
                case 1: registrarProducto(); break;
                case 2: listarProductos(); break;
                case 3: buscarProductoCodigo(); break;
                case 4: buscarProductoNombre(); break;
                case 5: actualizarStock(); break;
                case 6: actualizarPrecio(); break;
                case 7: eliminarProducto(); break;
                case 8: cout << "Volviendo al menu principal.\n"; break;
                default: cout << "Opcion no valida. Intente nuevamente.\n"; break;
            }
        } catch (...) {
            cout << "ERROR de lectura de opcion.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
        }
    } while (opcion != 8);
}

// =============================
// DESARROLLO DEL MODULO 1: CRUD DE PRODUCTOS
// =============================
void registrarProducto() {
    Producto p;
    cout << "\n--- REGISTRAR NUEVO PRODUCTO ---\n";
    cout << "Codigo: ";
    cin >> p.codigo;

    if (existeProducto(p.codigo)) {
        cout << "ERROR: Ya existe un producto con ese codigo. Registro cancelado.\n";
        return;
    }

    cin.ignore(); 
    cout << "Nombre: ";
    cin.getline(p.nombre, 50);
    cout << "Precio (Q): ";
    cin >> p.precio;
    cout << "Stock Inicial: ";
    cin >> p.stock;

    p.ventasAcumuladas = 0;
    p.activo = true;

    ofstream archivo(PATH_PRODUCTOS, ios::binary | ios::app);
    if (!archivo) {
        cout << "ERROR: No se pudo abrir el archivo de productos para escritura.\n";
        return;
    }
    archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    archivo.close();
    cout << "Producto registrado exitosamente.\n";
}

void listarProductos() {
    Producto p;
    ifstream archivo(PATH_PRODUCTOS, ios::binary);
    if (!archivo) {
        cout << "ERROR: No se pudo abrir el archivo de productos para lectura.\n";
        return;
    }

    cout << "\n" << left << setw(10) << "CODIGO" <<
    setw(25) << "NOMBRE" <<
    setw(12) << "PRECIO" <<
    setw(10) << "STOCK" <<
    setw(15) << "VENTAS" << endl;
    cout << "-------------------------------------------------------------\n";
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.activo) {
            cout << left << setw(10) << p.codigo
            << setw(25) << p.nombre
            << "Q" << setw(11) << p.precio
            << setw(10) << p.stock
            << p.ventasAcumuladas << endl;
        }
    }
    archivo.close();
}

void buscarProductoCodigo() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;

    cout << "\nIngrese el codigo a buscar: ";
    cin >> codigoBuscado;

    ifstream archivo(PATH_PRODUCTOS, ios::binary);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Producto Encontrado: " << p.nombre << " | Precio: Q" <<
            p.precio << " | Stock: " << p.stock << endl;
            encontrado = true;
            break;
        }
    }
    archivo.close();
    if (!encontrado) {
        cout << "No se encontro un producto activo con ese codigo.\n";
    }
}

void buscarProductoNombre() { 
    Producto p;
    string nombreBuscado;
    bool encontrado = false;

    cout << "\nIngrese el nombre a buscar: ";
    cin.ignore();
    getline(cin, nombreBuscado);

    ifstream archivo(PATH_PRODUCTOS, ios::binary);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.nombre == nombreBuscado && p.activo) {
            cout << "Producto Encontrado: Codigo " << p.codigo
            << " | " << p.nombre << " | Q" << p.precio << endl;
            encontrado = true;
            break;
        }
    }
    archivo.close();
    if (!encontrado) {
        cout << "No se encontro un producto activo con ese nombre.\n";
    }
}

void actualizarStock() {
    Producto p;
    int cod, nuevoStock;
    bool modificado = false;

    cout << "\nIngrese el codigo del producto para actualizar stock: ";
    cin >> cod;

    fstream archivo(PATH_PRODUCTOS, ios::binary | ios::in | ios::out);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == cod && p.activo) {
            cout << "Stock actual de " << p.nombre << ": " << p.stock << endl;
            cout << "Ingrese el nuevo stock: ";
            cin >> nuevoStock;
            p.stock = nuevoStock;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            modificado = true;
            cout << "Stock actualizado exitosamente.\n";
            break;
        }
    }
    archivo.close();
    if (!modificado) {
        cout << "No se encontro un producto activo con ese codigo para actualizar.\n";
    }
}

void actualizarPrecio() {
    Producto p;
    int cod;
    float nuevoPrecio;
    bool modificado = false;

    cout << "\nIngrese el codigo del producto para actualizar precio: ";
    cin >> cod;

    fstream archivo(PATH_PRODUCTOS, ios::binary | ios::in | ios::out);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == cod && p.activo) {
            cout << "Precio actual: Q" << p.precio << endl;
            cout << "Ingrese el nuevo precio (Q): ";
            cin >> nuevoPrecio;
            p.precio = nuevoPrecio;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            modificado = true;
            cout << "Precio actualizado exitosamente.\n";
            break;
        }
    }
    archivo.close();
    if (!modificado) {
        cout << "No se encontro un producto activo con ese codigo para actualizar.\n";
    }
}

void eliminarProducto() {
    Producto p;
    int cod;
    bool eliminado = false;

    cout << "\nIngrese el codigo del producto para eliminar/desactivar: ";
    cin >> cod;

    fstream archivo(PATH_PRODUCTOS, ios::binary | ios::in | ios::out);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == cod && p.activo) {
            p.activo = false; 

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            eliminado = true;
            cout << "Producto eliminado/desactivado exitosamente.\n";
            break;
        }
    }
    archivo.close();
    if (!eliminado) {
        cout << "No se encontro un producto activo con ese codigo para eliminar.\n";
    }
}

// =============================
// DESARROLLO DEL MODULO 2: PROCESO DE VENTA
// =============================
void procesoVenta() {
    Venta nuevaVenta;
    nuevaVenta.idVenta = generarIdVenta();
    nuevaVenta.cantidadDetalles = 0;
    nuevaVenta.totalArticulos = 0;
    nuevaVenta.subtotal = 0.0f;

    cout << "\nIngrese el numero de mes para la transaccion (1-12): ";
    cin >> nuevaVenta.mes;
    if (nuevaVenta.mes < 1 || nuevaVenta.mes > 12) nuevaVenta.mes = 1; 

    char responder;
    do {
        try {
            int codProd, cant;
            cout << "\nCodigo del producto para anadir al carrito: ";
            if (!(cin >> codProd)) throw runtime_error("Entrada no valida. Por favor ingrese un numero.");

            fstream archivoP(PATH_PRODUCTOS, ios::binary | ios::in | ios::out);
            if (!archivoP) throw runtime_error("ERROR: No se pudo abrir el archivo de productos para lectura/escritura.");

            Producto p;
            bool hallado = false;
            int posicion = 0;

            while (archivoP.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
                if (p.codigo == codProd && p.activo) {
                    hallado = true;
                    cout << ">> " << p.nombre << " | Precio: Q" << p.precio << " | Disponibles: " << p.stock << endl;
                    cout << "Cantidad a comprar: ";
                    if (!(cin >> cant)) throw runtime_error("Entrada no valida. Por favor ingrese un numero.");

                    if (cant <= 0) {
                        cout << "La cantidad debe ser mayor a cero. Intente nuevamente.\n";
                        break;
                    }

                    if (cant > p.stock) {
                        cout << "Stock insuficiente. Solo hay " << p.stock << " unidades disponibles.\n";
                    } else {
                        DetalleVenta &d = nuevaVenta.detalles[nuevaVenta.cantidadDetalles];
                        d.codigoProducto = p.codigo;
                        d.cantidad = cant;
                        d.precioUnitario = p.precio;
                        d.subtotalDetalle = cant * p.precio;

                        nuevaVenta.subtotal += d.subtotalDetalle;
                        nuevaVenta.totalArticulos += cant;
                        nuevaVenta.cantidadDetalles++;

                        p.stock -= cant;
                        p.ventasAcumuladas += cant;

                        archivoP.seekp(posicion * sizeof(Producto), ios::beg);
                        archivoP.write(reinterpret_cast<char*>(&p), sizeof(Producto));
                        cout << "Producto anadido al carrito exitosamente.\n";
                    }
                    break;
                }
                posicion++;
            }
            archivoP.close();
            if (!hallado) {
                cout << "No se encontro un producto activo con ese codigo.\n";
            }
        } catch (const runtime_error& e) {
            cout << "EXCEPION CONTROLADA: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (nuevaVenta.cantidadDetalles >= 15) {
            cout << "Se ha alcanzado el maximo de productos por venta (15). Procediendo al pago.\n";
            break;
        }

        cout << "Desea agregar otro producto a la misma venta? (S/N): ";
        cin >> responder;
    } while (responder == 'S' || responder == 's');

    if (nuevaVenta.cantidadDetalles == 0) {
        cout << "No se agregaron productos a la venta. Transaccion cancelada.\n";
        return;
    }

    nuevaVenta.descuento = 0.0f;
    if (nuevaVenta.subtotal > 500.0f) {
        nuevaVenta.descuento += nuevaVenta.subtotal * 0.05f;
    }
    if (nuevaVenta.totalArticulos % 5 == 0) {
        nuevaVenta.descuento += 10.0f;
    }

    nuevaVenta.iva = (nuevaVenta.subtotal - nuevaVenta.descuento) * IVA_FACTOR;

    float tempTotal = (nuevaVenta.subtotal - nuevaVenta.descuento) + nuevaVenta.iva;
    nuevaVenta.total = std::round(tempTotal * 100.0f) / 100.0f;

    ofstream archivoV(PATH_VENTAS, ios::binary | ios::app);
    if (archivoV) { 
        archivoV.write(reinterpret_cast<char*>(&nuevaVenta), sizeof(Venta));
        archivoV.close();

        cout << fixed << setprecision(2);
        cout << "\n======================================" << endl;
        cout << "            TICKET FACTURA #00" << nuevaVenta.idVenta << endl;
        cout << "======================================" << endl;
        cout << "Articulos Totales: " << nuevaVenta.totalArticulos << endl;
        cout << "Subtotal:          Q" << nuevaVenta.subtotal << endl;
        cout << "Descuento Total:  -Q" << nuevaVenta.descuento << endl;
        cout << "IVA Calculado:     Q" << nuevaVenta.iva << endl;
        cout << "---------------------------------------" << endl;
        cout << "TOTAL A PAGAR:     Q" << nuevaVenta.total << endl;
        cout << "======================================" << endl;
    } else {
        cout << "ERROR: No se pudo guardar la venta en el archivo.\n";
    }
}

// =============================
// FUNCIONES AUXILIARES
// =============================
bool existeProducto(int codigo) {
    Producto p;
    bool flag = false;
    ifstream archivo(PATH_PRODUCTOS, ios::binary);
    if (!archivo) return false;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            flag = true;
            break;
        }
    }
    archivo.close();
    return flag;
}

int generarIdVenta() {
    Venta v;
    int id = 1;
    ifstream archivo(PATH_VENTAS, ios::binary);
    if (!archivo) return id;

    while (archivo.read(reinterpret_cast<char*>(&v), sizeof(Venta))) {
        id = v.idVenta + 1;
    }
    archivo.close();
    return id;
}