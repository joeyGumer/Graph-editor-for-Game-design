# Graph-editor-for-Game-design
## Manual de uso del editor de grafos
En este manual se explica cómo navegar y usar las funcionalidades del editor de grafos.

### IMPORTANTE: Cuando se descomprime el archivo con las carpetas del programa y se ejecuta el editor de grafos por primera vez, puede surgir un error imperceptible. Cuando se pasa el cursor por encima de un vértice en el editor, este debería resaltar su silueta de un color rosa saturado, si esto no ocurre, cierra el programa y vuélvelo a abrir, entonces se debería haber solucionado. Aunque parece una nimiedad, el sistema para seleccionar los vértices es el mismo que se usa para mostrar los resultados, por tanto si ocurre este error, en esa sesión del programa no se podrá mostrar los resultados de las evaluaciones de propiedades, que es una funcionalidad importante. Este error parece ocurrir debido a la intervención de Windows o por el proceso de descompresión del archivo, ya que nunca ha sucedido cuando se ejecuta la build directamente desde la carpeta original que crea Unity, pero no se ha logrado encontrar una solución.

### Camara
Estos son los controles principales de la cámara del escenario de edición de grafos, que se pueden usar cuando el cursor está sobre este.
•	**Zoom in/out**: al usar la rueda central del ratón se acerca o aleja la cámara.

•	**Desplazar**: al mantener pulsada la rueda central del ratón, se permite mover la cámara siguiendo el movimiento del cursor.

•	**Centrar**: pulsar la tecla “F” del teclado para centrar la cama en el origen del escenario.


### Herramientas de edición
Las herramientas de edición son las distintas opciones que se presentan en la barra de tareas a la izquierda del editor. Para seleccionar cualquiera de ellas pulsar con el botón izquierdo del ratón. Al seleccionarse se mantendrá ese modo de edición activo hasta que se seleccione otro o se pulse el botón derecho del ratón sobre la escena de grafos.

Este es el modo de uso de las distintas herramientas que ofrece el editor:
•	**Add Vertex** : crea un nuevo vértice en el escenario clicando con el botón izquierdo del ratón sobre la posición en la que se quiera crear.

•	**Add Connected Vertex**: crea un nuevo vértice conectado a uno existente. Para usar, clicar con el botón izquierdo del ratón sobre un vértice existente para crear un vértice nuevo, conectado al que se ha pulsado. Mantener el botón pulsado y mover el cursor para desplazar el nuevo vértice y soltar el botón para colocarlo en el sitio.

•	**Add Edge**: crea una arista conectando dos vértices especificados. Para usar, seleccionar un primer vértice con el botón izquierdo del ratón. Al mantener pulsado se generará una línea rosa que simula la arista a crear, arrastrar el cursor hasta un segundo vértice y soltar el botón para conectar el primer y segundo vértice con una nueva arista. Si se suelta el botón antes de conectar los vértices, la línea desaparece y no se crea ninguna arista.

•	**Delete Vertex y Delete Edge**: eliminan los vértices y aristas seleccionados respectivamente. Pulsar con el botón izquierdo del ratón sobre el vértice o arista que se quiera eliminar. Si se elimina un vértice se eliminarán todas las aristas a las que estaba conectado.

•	**Duplicate Vertex y Duplicate Connect Vertex**: si se selecciona con el botón izquierdo del ratón sobre un vértice, se crea un nuevo vértice conectado mediante aristas a todos los vértices a los cual estaba conectado el vértice original. Si se selecciona la versión Duplicate Connect Vertex, el nuevo vértice también se conecta al vértice original. Mantener pulsado el botón para desplazar el nuevo vértice y soltar para colocarlo similarmente a Add Connected Vertex.

•	**Evaluate**: abre la ventana de evaluación del grafo para evaluar sus propiedades. 

•	**Reset**: elimina el grafo en pantalla.

También hay una checkbox con el título “Show Weights” que al marcar muestra los pesos asignados a las aristas, que por defecto es 1. Si se pulsa con el botón derecho del ratón sobre una arista se abre el panel de información de dicha arista y se puede modificar el peso asignado.
Ventana de evaluación

Ventana que se abre al activar la herramienta “Evaluation”, muestra los paneles de distintas propiedades a evaluar del grafo presente en la escena. Cada panel consiste principalmente de dos botones: “Evaluate” y “Show”, el primero evalúa la propiedad del grafo y el segundo muestra los resultados.

En general, el proceso de evaluación de cada escena es el siguiente, el botón “Show” se encuentra bloqueado por defecto, si se pulsa el botón “Evaluate” se obtienen los resultados. En caso de que haya resultados que mostrar se desbloquea el botón “Show”, que al pulsarse muestra sobre le grafo en escena el resultado.
Algunas propiedades requieren alguna operación antes de evaluarse, por lo que el botón “Evaluate” se mantendrá bloqueado hasta que se cumplan los requisitos.

La información y operaciones específicas de cada propiedad a evaluar son las siguientes:
•	**Dijkstra**: calcula el camino más corto en el grafo desde un vértice de origen seleccionado hasta todos los demás vértices, si es que existe tal camino. Para evaluarlo, se requiere seleccionar primero el vértice de origen, primero pulsando el botón “Select Vertex”, y luego seleccionando un vértice del grafo pulsándolo con el botón izquierdo. El índice del vértice seleccionado se muestra en el texto “Source vertex”

•	**Minimal Path**: calcula el camino más corto entre dos vértices seleccionados, que puede no existir. Para evaluarse se tienen que seleccionar primer el vértice de salida y el de llegada pulsando el botón “Select Vertices” y luego seleccionando con el botón izquierdo del ratón, primero el vértice de salida y luego el de llegada, cuyos indices se muestran respectivamente en los textos “Start Vertex” y “End Vertex”. Al evaluarse, si existe o no un camino se muestra en el texto “Exists”, y en caso positivo se muestra la distancia en “Distance” , y solo en caso positivo se permite mostrar el resultado.

•	**Hamiltonian Cycle (TPS)**: calcula el ciclo hamiltoniano de menor peso total del grafo, que puede no existir. Se puede evaluar directamente, y solo se puede mostrar el resultado si existe tal solución, lo cual se muestra en el texto “Exists”. Si existe solución también se muestra el peso total en el texto “Weight”.

•	**Minimal Coloration Vertices/Edges**: calcula la coloración minimal de vértices y aristas respectivamente, y muestra un ejemplo de coloración como resultado. Se puede evaluar directamente, y al evaluarse muestra la cantidad de colores mínima de la coloración. Siempre existe una coloración minimal. Por limitaciones técnicas, se ha estipulado que si un grafo requiere 10 o más colores para alguna coloración, no se puede mostrar el resultado.

### ADVERTENCIA: algunas propiedades pueden empezar a tardar bastante tiempo en evaluarse si el grafo contiene muchos vértices, o sobre todo, si contiene muchas aristas. Concretamente las propiedades que se pueden alargar notablemente en función del tamaño son TPS y la coloración minimal de aristas. En caso de que ocurra se abrirá una pantalla de consola en negro con un cursor, correspondiente al programa de evaluación. Se puede cortar este proceso simplemente cerrando dicha ventana.

