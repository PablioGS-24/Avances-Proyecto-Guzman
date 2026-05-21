🛒 Sistema de Ventas en C++ (Avance de Proyecto)
Este proyecto implementa la administración de inventario y facturación utilizando archivos binarios para la persistencia de datos.

📊 Estado de los Módulos
Módulo 1: Gestión de Productos (CRUD) ➔ ¡100% COMPLETADO Y LISTO!

* Registro de nuevos productos con códigos únicos.

* Listado completo del inventario activo en formato de tabla.

* Búsqueda de productos por código o por nombre exacto.

* Modificación directa de stock y precios en el archivo binario.

* Eliminación por "borrado lógico" (desactivación).

Módulo 2: Proceso de Venta ➔ ¡100% COMPLETADO Y LISTO!

* Carrito de compras virtual (soporta hasta 15 productos por venta).

* Control automático de stock (no permite vender más de lo existente).

* Aplicación de descuentos (5% por compras mayores a Q500 y bono de Q10 por artículos múltiplos de 5).

* Desglose financiero con cálculo del 12% de IVA y redondeo con round().

* Generación del Ticket de Factura y descarga del stock en tiempo real.

Módulo 3: Reportes Estadísticos ➔ ⏳ EN PROCESO

Estatus: Diseñado en el menú principal. Su implementación técnica (ordenamientos y matriz anual) se entregará en la fase final.

Módulo 4: Utilidades del Sistema ➔ ⏳ EN PROCESO

Estatus: Diseñado en el menú principal. La exportación a archivos .txt y el formateo del sistema se incluirán en la entrega final.

🛠️ Tecnologías Evaluadas en este Avance
Archivos Binarios: Lectura y escritura exacta de registros mediante fstream.

Estructuras Fijas: Uso de struct para el control estricto de bytes.

Manejo de Excepciones: Bloques try/catch para proteger el flujo de datos contra ingresos inválidos del usuario.