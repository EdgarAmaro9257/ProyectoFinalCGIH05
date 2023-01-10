<div align="center">

# **ProyectoFinalCGIH05-Manual de Usuario**

<div align="justify">

### **Autor** :
 - Amaro Cantoral Edgar
 - Cta: 316625368
</div>
  
#### **Semestre 2023-1 | Grupo 05**
</div>

### **Requerimentos**

* Para ejecución con Visual Studio 2019 con instalación de C++ o con el ejecutable .exe:
    - Windows 7 (Última actualización), Windows 8 (2919355), Windows 10 (version 1703 o superior)
    - Procesador: 1.8 GHz (recomendado 4 núcleos o más)
    - 2.5 GB RAM mínimo, 8 GB recomendado
    - 2 GB en disco, recomendado
    - Chip gráfico de mínimo 720p de resolución

### **Apertura y ejecución del proyecto**

Descargar el proyecto desde GitHub dando click al boton verde "Code" y seleccionando una de las opciones
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_1.jpg" alt="Descarga GitHub 1" ></img>
<div align="justify">
 
Si selecciona "Open with GitHub Desktop" requiere tener esta aplicación instalada en su equipo y seguir los pasos para clonar el repositorio de manera local, una vez 
clonado saltar hasta ["Ejecución el archivo .exe"](#item1).

La selección "Download ZIP" descargará un archivo .zip del proyecto, habrá que descomprimirlo en su equipo, regresar a este manual y saltarse hasta ["Ejecución el archivo .exe"](#item1).

Una vez descargado y descomprimido el proyecto en su equipo, deberá descomprimir 3 archivos importantes en su equipo debido a que en el repositorio está limitada la cantidad de alamcenamiento de datos. Las siguientes rutas en donde descomprimirá estos archivos con las siguientes:

CGIH_prueba1/lib
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_3.jpg" alt="Descomprimir archivos faltantes 1" ></img>
<div align="justify">
 

CGIH_prueba1/resources/objects/FurnitureBath
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_4.jpg" alt="Descomprimir archivos faltantes 2" ></img>
<div align="justify">


 ### ***Ejecución con Visual Studio*** 
 
 Si se desea y tiene instalado el IDE Visual Studio puede ejecutar la aplicación siguiendo los siguientes pasos.
 Tener descargado el proyecto de git-hub y haber descomprimdo la carpeta zip. Una vez teniendo esto abrir Visual Studio y seleccionar la opción "Abrir un proyecto o una solución"
 
  <div align="Center">
  <img src="ImagenesEvidencia/EvidenciaPF_2.jpg" alt="Abrir proyecto"></img>
<div align="justify">
 
Buscar el archivo "CGIH_prueba1.sln" dentro de la carpeta descomprimida y dar en el botón de abrir. 

<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_5.jpg" alt="Abrir solución" ></img>
<div align="justify">

Antes de ejectutar el programa comprobamos que todas las bibliotecas y configuraciones esten cargadas correctamente. Nos dirigimos al explorador de soluciones, hacemos click derecho en el nombre del proyecto y seleccionamos la opción de propiedades.

<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_6.jpg" alt="Propiedades de solución" ></img>
<div align="justify">

En el apartado de "General" verificamos que el Conjunto de herramienras de la plataforma sea uno que tengamos descargado puede ser v142 o v143.

<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_7.jpg" alt="Version de Visual Studio" ></img>
<div align="justify">
 
 Dentro de C/C++ -> Gneral verificar que la palabra include se encuentra en directorios de inclusión adicionales. En el Vincluador -> General la palabra lib en Directorios de bibliotecas adicionales.
 
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_8.jpg" alt="include" ></img>
<div align="justify">

<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_9.jpg" alt="lib" ></img>
<div align="justify">

Finalmente, en Vinculador -> Entrada verificar que las dependencias se encuentren ahí: SDL2.lib;SDL2main.lib;assimp-vc141-mtd.lib;opengl32.lib;glew32.lib;glfw3.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

Si ya se encuentran todas las dependencias, presionar "Aceptar"
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_10.jpg" alt="dependencias" ></img>
<div align="justify">

Para poner en ejecución el proyecto se debe seleccionar la opción "x64" en la barra superior y verificar que esté seleccionada la opción "Debug" como se muestra en la siguiente imagen.
 
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_11.jpg" alt="Verificando opcion" ></img>
<div align="justify"> 
 
Ya puede ejecutarse el proyecto dando click en el boton "Depurador local de Windows", esto abrirá la ventana del programa. 
 
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_12.jpg" alt="Depurando proyecto" ></img>
<div align="justify"> 
 
### ***Ejecución con archivo .exe***
 
Una vez clonado el proyecto o descomprimido el ZIP: abrir la carpeta generada o ubicación del repositorio en su equipo y dirigirse a "\ProyectoFinalCGIH05\Ejecutable", descomprimir el archivo "CGIH_prueba1.pdb", ubicar el archivo "CGIH_prueba1.exe" y dar doble click para abrir. Lo anterior abrirá una ventana con el espacio virtual ejecutandose.
 
<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_13.jpg" alt="Descomprimiendo archivo" ></img>
<div align="justify"> 

<div align="Center">
    <img src="ImagenesEvidencia/EvidenciaPF_14.jpg" alt="Ejecutando proyecto" ></img>
<div align="justify"> 
 
### ***Interacción con el ambiente*** 
 
Una vez abierta la ventana; estando dentro del espacio virtual se captura el mouse para funcionar unicamente dentro de este, por lo que no se observa el cursos y el movimiento del mouse solo funciona dentro de la ventana. La interacción se da mediante el teclado y mouse de la computadora.
 
 
 
 
 
 
 
 
