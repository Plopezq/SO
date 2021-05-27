#Script de comprobacion

#1. Comprobamos que el programa mytar esta en el directorio actual y es ejecutable
if ! test -x ./mytar ; then
    echo "Error: mytar no existe en el directorio actual"
    exit 1
fi

#2. Comprobamos si existe un directorio tmp, si es asi, lo borramos recursivamente
if test -d tmp; then
    rm -rf tmp
fi

#3. Creamos un irectorio temporal y nos movemos a ese directorio
mkdir tmp
cd tmp

#4. Creamos 3 ficheros dentro del directorio, con las caracteristicas pedidas
echo "Hello world!" > file1.txt

head -n 10 /etc/passwd > file2.txt

head -c 1024 /dev/urandom > file3.dat


#5. Invocamos al programa mytar pasando los ficheros anteriores

../mytar -cf filetar.mtar file1.txt file2.txt file3.dat

#6. Cremos un directorio "out" y copiamos el fichero creado a ese directorio

mkdir out

cp filetar.mtar out

#7. Cambiamos al directorio out y ejecutamos el programa mytar para extraer el contenido del taarball

cd out

../../mytar -xf filetar.mtar

#8. Ejecutamos la orden diff para comprobar si son los dos archivos iguales
diff file1.txt ../file1.txt
    if [ $? -ne 0 ] 
    then
        cd ..
        cd ..
        echo "Los ficheros file1.txt NO son iguales"
        exit 1
    fi
diff file2.txt ../file2.txt
    if [ $? -ne 0 ]
    then 
        cd ..
        cd ..
        echo "Los ficheros file2.txt NO son iguales"
        exit 1
    fi
diff file3.dat ../file3.dat
    if [ $? -ne 0 ]
    then
        cd ..
        cd ..
        echo "Los ficheros file3.dat NO son iguales"
        exit 1
    fi
#8. todo con exito
    echo "Correct"
exit 0







