1-Descomprimir el .zip en la carpeta donde se quiera correr el proyecto.
2-Abrir la terminal en dicha carpeta y escribir por línea de comandos:
 cd RowDaBoat-x64barebones-d4e1c147f975/
docker run -v ${PWD}:/root --security-opt seccomp:unconfined -ti agodio/itba-so:1.0
3-En docker, escribir por línea de comandos:
	cd root/
	cd Toolchain
	make all
	cd ..
“make all” para compilar con nuestra implementación propia del MMU o “make buddy” para compilar con la implementación del MMU Buddy 
4-exit (para correr en la misma ventana)
5-Para la ejecución, basta con escribir por línea de comandos  “./run.sh”.
