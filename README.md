# Importante

En caso de que no se cree sola, agregar una carpeta vacía con nombre "outputs" para que el programa funcione correctamente.

Según la versión de C++ que se use, se encontró que "filesystem", que se utiliza con la finalidad de borrar los archivos de las ejecuciones anteriores, da error. Se necesita C++17 o superior. Comentar, si no.

En caso de que los links en el informe no funcionen:

- Seguimiento - Servicio 1: <https://docs.google.com/presentation/d/1LQJFxBL7h5mbrUrlEm-RDgIJAQC-yR-vB9CQsiyWiOI/edit?usp=sharing>

- Seguimiento - Servicio 2: <https://docs.google.com/presentation/d/1IXWunOe_m3T9Lvf_4W-sZdWgksMkz5d6hLv3Xg_Vndw/edit?usp=sharing>

- Seguimiento - Servicio 3: <https://docs.google.com/presentation/d/1sNRtLrm3kM0cDVOK-p-C-jmVMNFK_qOvCOgCYWWumuo/edit?usp=sharing>

## Compilación del Programa

`g++ -Iinclude *.cpp -o Main`

Explicación:

- `-Iinclude` le indica al compilador dónde buscar los .h.
- `*.cpp` compila todos los archivos .cpp de la carpeta actual y los linkea juntos.
- `-o Main` genera el ejecutable llamado Main.

## Drive

Material originalmente entregado en 2023: <https://drive.google.com/drive/folders/1lkFPNAm_Ghp0EjDVmCpnAqnPXeSShcED?usp=drive_link>.

En 2025, se modificó el código del proyecto para dejarlo funcional tanto en sistemas operativos Windows como Linux.
