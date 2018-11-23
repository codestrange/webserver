Orientacion del Proyecto
========================

El objetivo del proyecto es hacer un pequeño servidor web que permita listar directorios y archivos en una computadora.

Las funcionalidades son las siguientes:

1. al ejecutar el programa deben pasarle 2 parámetros, el puerto por donde va a escuchar (1) y el directorio raíz de donde se va a mostrar información (2). 

~~~bash
$ gcc webserver.c -o server
$ ./server 4000 /home/pqr/
Listening in port 4000
Serving directory "/home/pqr"
~~~

2. Debe velar en todo momento que los clientes no puedan listar ninguna información que se encuentre fuera del directorio raiz que se especifica en la entrada.

3. Este servidor web debe mostrar la información de un directorio creando una página web html con el contenido. Vea el fichero [directory.html](directory.html) para que vea un ejemplo de como debe ser este archivo.

4. No es objetivo de este proyecto que el servidor implemente con lujo de detalle el protocolo http. Pero es necesario que sea capaz de entender las peticiones que recibirá de los navegadores web más comunes (Chrome, Firefox, etc.).

5. Para saber como está conformada la petición que recibirá el servidor, se recomienda, que en los primeros momentos del desarrollo del mismo, abran un navegador y pongan la dirección del servidor web desarrollado por ustedes e impriman la información que reciben.

6. El cliente en el navegador debe ser capaz de "entrar" a un nuevo directorio con solo dar un click encima del nombre del mismo.

7. El cliente en el navegador debe ser capaz de descargar un archivo con solo darle un click.

8. El servidor debe ser capaz de atender correctamente a un cliente y manejar un solo request a la vez.  

* NOTA: para leer el contenido de un directorio considere usar las funciones `opendir()` y `readdir()`

y hasta aqui tienen 3 puntos en el proyecto

9. Mostrar información adicional de los archivos además del nombre (fecha de modificación, tamaño, permisos, etc.) [1pt] 

* esta funcionalidad incluye la posibilidad de ordenar por cada uno de esos parámetros (nombre, fecha, tamaño, etc.)

10. Soportar multiples conexiones  (2pt)

* esta funcionalidad adicional debe permitir que el cliente pueda estar atendiendo varios clientes simultáneamente

* esta funcionalidad debe permitir que un cliente pueda empezar a descargar un archivo (grande) en el navegador y que el navegador no se bloquee y permita seguir navegando y dando clicks en otros directorios.

* eventualmente se debe poder estar descargando varios archivos a la misma vez. 

* NOTA: si hay mas de un archivo descargandose a la vez, debe velar porque ambos archivos *"se descarguen simultáneamente"*. Si uno de los 2 archivos termina antes de empezar el otro, no se considera terminada esta funcionalidad y obtienen solo 1pt en vez de 2pts.

* NOTA: para esta funcionalidad considere usar la función `select()`.

* NOTA: puede hacer uso tambien de funciones como `sendfile()` 

* NOTA: esta funcionalidad se puede hacer sin `select()` usando multiples procesos (`fork()`). si no utilizan select, solo obtienen 1pt en vez de 2pts.

Fecha de entrega: 30/11/2018

