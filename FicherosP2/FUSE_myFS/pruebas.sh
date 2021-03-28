#SCRIPT DE COMPROBACION

# a) Copie dos ﬁcheros de texto 
# que ocupen más de un bloque (por ejemplo fuseLib.c y myFS.h) a 
# nuestro SF y a un directorio temporal, por ejemplo ./temp

cp src/fuseLib.c ./mount-point/
cp src/myFS.h ./mount-point/

#rm -R /temp

mkdir temp 
cp src/fuseLib.c ./temp/
cp src/myFS.h ./temp/

# b) Audite el disco y haga un diff entre los ﬁcheros originales y los copiados 
# en el SF
./my-fsck virtual-disk
diff mount-point/fuseLib.c temp/fuseLib.c #NO hay diferencias
diff mount-point/myFS.h temp/myFS.h