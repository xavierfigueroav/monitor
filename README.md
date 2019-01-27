#MONITOR#

Este proyecto pretende ser una aplicación cliente - servidor para monitorear remotamente el estado de una computador. La comunicacion entre el servidor y el cliente es a traves de un socket utilizando el formato Protobuf (Protocol Buffes) de Google el cual debe ser interpretado correctamente por el cliente.

##Empezar el monitor##

*monitor* esta escrito en C y por ende necesitas tener instaladas en tu computadora las herramientas de desarrollo de C. Ademas, para poder trabajar con el formato Protobuf se necesita tener las versiones actualizadas de Protobuf, Protobuf-c, pkg-config, un compilador de C y un compilador de C++.

###Instalacion###

El primer paso antes de poder empezar a usar *monitor* es obtener su código fuente, lo cual puedes hacer clonando este repositorio.

Una vez hecho lo anterior, podemos compilar el codigo fuente por medio del uso del archivo Makefile, el cual viene incluido en el proyecto. Para utilizarlo basta con ubicarnos en el directorio principal e ingresar el siguiente comando:
```
make
```
Al momento de realizar el paso anterior se generaran dos archivos ejecutables, monitorc que es el cliente y monitord que es el servidor

##Funciones##

Por el momento, *monitor* brinda las siguientes funciones:

Para monitorc, permite a un usuario el ingresar su ip y el puerto de donde desea obtener la respuesta del servidor, en caso de que no se ingresen, estos por defecto seran 127.0.0.1 (localhost) y 8000.

Para monitord, permite enviar la informacion del servidor a un cliente, este por defecto trabaja como un daemon a menos que el usuario le pida que trabaje por consola o almacenando la informacion en un archivo.

##Uso##

Para usar la aplicacion de *monitor* primero se debe ejecutar el servidor, es decir, debemos ejecutar monitord de la siguiente forma:
```
./monitord [option] [puerto]
```
entre las opciones de monitord tenemos:

* -h: muestra un mensaje de ayuda sobre el funcionamiento de monitord
* -c: hace que monitord deje de trabajar como deamon y muestre la informacion por consola
* -l [log_file]: almacena la informacion del cliente, el ip y los procesos que este haciendo, en el archivo log_file, por defecto se almacena en log.txt

Despues de escoger una de la opciones, se debe ingresar el puerto por donde se desea que el cliente se conecte. Por ejemplo:
```
./monitord -c -l 8000
```
Una vez que monitord este en ejecucion, procedemos a ejecutar monitorc, el cual es el cliente,de la siguiente forma:
```
./monitorc [option] [<ip>] [<puerto>]
```
entre las opciones tenemos:

* -h: muestra un mensaje de ayuda sobre el funcionamiento de monitorc

Despues de escoger una de la opciones, se debe ingresar la ip del cliente y el puerto por donde desea recibir la informacion, por defecto estos son 127.0.0.1 y 8000. Por ejemplo:
```
./monitorc 198.0.0.1 8000
```
##Autores##
[Xavier Figueroa](https://bitbucket.org/xavierfigueroav),[Eduardo Tigse](https://bitbucket.org/EduardTigLar)
 
