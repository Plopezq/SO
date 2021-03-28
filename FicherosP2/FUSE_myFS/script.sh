#SCRIPT DE COMPROBACION

# a) Copie dos ﬁcheros de texto 
# que ocupen más de un bloque (por ejemplo fuseLib.c y myFS.h) a 
# nuestro SF y a un directorio temporal, por ejemplo ./temp

cp src/fuseLib.c ./mount-point/
cp src/myFS.h ./mount-point/

mkdir temp 
cp src/fuseLib.c ./temp/
cp src/myFS.h ./temp/

# b) Audite el disco y haga un diff entre los ﬁcheros originales y los copiados 
# en el SF
./my-fsck virtual-disk
diff mount-point/fuseLib.c temp/fuseLib.c
diff mount-point/myFS.h temp/myFS.h
# Trunque el primer ﬁchero (man truncate) en copiasTemporales y en nuestro SF 
# de manera que ocupe ocupe un bloque de datos menos. Los bloques son de 4096 byets
truncate -s -4096 temp/fuseLib.c
truncate -s -4096 mount-point/fuseLib.c
#c) Audite el disco y haga un diff entre el ﬁchero original y el truncado.
#Original --> src/fuseLib.c
./my-fsck virtual-disk
diff temp/fuseLib.c src/fuseLib.c
diff mount-point/fuseLib.c src/fuseLib.c
#d) Copie un tercer ﬁchero de texto a nuestro SF.
cp text.txt ./mount-point/
#e) Audite el disco y haga un diff entre el ﬁchero original y el copiado en el SF
./my-fsck virtual-disk
diff text.txt ./mount-point/text.txt
#f) Trunque el segundo ﬁchero en copiasTemporales y en nuestro SF haciendo que ocupe algún bloque de datos más.
#un bloque --> 4096
truncate -s +8192 temp/myFS.h
truncate -s +8192 mount-point/myFS.h
#g) Audite el disco y haga un diff entre el ﬁchero original y el truncado.
./my-fsck virtual-disk
diff temp/myFS.h src/myFS.h
diff mount-point/myFS.h src/myFS.h




















