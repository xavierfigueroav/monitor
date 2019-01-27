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


