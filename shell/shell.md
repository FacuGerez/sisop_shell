# shell

### Búsqueda en $PATH

¿Cuáles son las diferencias entre la syscall execve(2) y 
la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

En principio la diferencia es que la familia de wrappers, valga la redundancia, wrappea la syscall execve(2), y dentro de cada una de las funciones que pertenecen a la familia exec(3) otorgan distintas formas de pasar los parámetros para la llamada al ejecutable.

¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?

Sí, la llamada puede fallar y en ese caso se "vuelve" de la llama de exec, es decir que si tengo

exec(binary)
// si llegué aca tengo un error

por lo cual el error debe ser manejado debajo de la llamada misma. 

En la implementación de la shell se libera la memoria reservada por el comando y se finaliza el proceso con código de error.

### Procesos en segundo plano

- ¿Por qué es necesario el uso de señales?

El uso de señales es necesario porque no se espera de forma bloqueante al proceso ejecutado en segundo plano. Por lo tanto es necesario tener un handler que espere a la señal sigchild para luego hacerle el wait y que no quedé en modo zombie.

### Flujo estándar

Investigar el significado de 2>&1, explicar cómo funciona su forma general

Lo que está sucediendo es que se está redirigiendo el fd stderr al stdout. En general se redirige el fd de la izquierda al de la derecha.

    - Mostrar qué sucede con la salida de cat out.txt en el ejemplo.

```
$ ls -C /home /noexiste >out.txt 2>&1
$ cat out.txt
ls: no se puede acceder a '/noexiste': No existe el fichero o el directorio
/home:
taiel
```


    - Luego repetirlo, invirtiendo el orden de las redirecciones (es decir, 2>&1 >out.txt). ¿Cambió algo? Compararlo con el comportamiento en bash(1).

```
$ ls -C /home /noexiste 2>&1 >out.txt
ls: no se puede acceder a '/noexiste': No existe el fichero o el directorio
$ cat out.txt
/home:
taiel
```

La diferencia que se observa es que en el primero al hacer el cat del out.txt vemos tanto el stderr seguido del stdout, porque primero se redirige lo que va al fd stdout al archivo, y luego se redirige el stderr al mismo archivo (ya que previamente redirigimos el stdout al archivo mismo).

En cambio en el segundo caso lo que sucede es que primero redirigimos el stderr a la salida, y luego redirigimos la salida al txt, por ello vemos el "stderr" en el stdout, por eso nos aparece el mensaje de ls: ..., y luego vemos en el archivo lo que sería realmente la salida del programa.

En nuestra implementación el comportamiento es idéntico al del bash(1).

### Tuberías múltiples

Investigar qué ocurre con el exit code reportado por la shell si se ejecuta un pipe

- ¿Cambia en algo?



- ¿Qué ocurre si, en un pipe, alguno de los comandos falla? Mostrar evidencia (e.g. salidas de terminal) de este comportamiento usando bash. Comparar con su implementación.

Ejecucion bash

$ cat noexiste.txt | wc
cat: noexiste.txt: No existe el fichero o el directorio
      0       0       0
$ echo $?
0

Ejecución tp shell

$ ./sh
 (/home/taiel) 
$ cat noexiste.txt | wc
cat: noexiste.txt: No existe el fichero o el directorio
      0       0       0
 (/home/taiel) 
$ echo $?
0
	Program: [echo $?] exited, status: 0 
 (/home/taiel) 
$ asda
	Program: [asda] exited, status: 1 
 (/home/taiel) 
$ echo $?
1
	Program: [echo $?] exited, status: 0 

Lo que sucede es que el código de salida de un pipe es independiente del código de salida de los procesos que se les pasan, es decir, que la única forma que el código de salida del propio pipe no sea 0, es que efectivamente al redirigir haya ocurrido un error.

### Variables de entorno temporarias

- ¿Por qué es necesario hacerlo luego de la llamada a fork(2)?

La necesidad de hacerlo luego de llamada a fork es para justamente no cambiar el environment del proceso padre (la propia shell).

En algunos de los wrappers de la familia de funciones de exec(3) (las que finalizan con la letra e), se les puede pasar un tercer argumento (o una lista de argumentos dependiendo del caso), con nuevas variables de entorno para la ejecución de ese proceso. Supongamos, entonces, que en vez de utilizar setenv(3) por cada una de las variables, se guardan en un arreglo y se lo coloca en el tercer argumento de una de las funciones de exec(3).

    - ¿El comportamiento resultante es el mismo que en el primer caso? Explicar qué sucede y por qué.

    Si utilizamos setenv(3) + una llamada a exec(3), que no termine en e, los llamados heredan el environment del proceso actual. En cambio usar un llamado a una función exec(3) que sí terminé en e, reemplaza totalmente el environment del proceso actual, siendo el environment del nuevo programa únicamente el pasado por parámetro.

    - Describir brevemente (sin implementar) una posible implementación para que el comportamiento sea el mismo.

    La forma sería obtener el environment actual, agregarlo a las variables de entorno del cmd y ese nuevo arreglo pasarselo al exec(3) terminado en e.

### Pseudo-variables

---

### Comandos built-in

¿Entre cd y pwd, alguno de los dos se podría implementar sin necesidad de ser built-in? ¿Por qué? ¿Si la respuesta es sí, cuál es el motivo, entonces, de hacerlo como built-in? (para esta última pregunta pensar en los built-in como true y false)

CD necesariamente tiene que ser builtin porque cambia el directorio del proceso actual, es decir, de la propia shell. En cambio pwd podría no ser built in porque simplemente printea el directorio actual, no modifica nada de la shell, haciendo posible que sea un programa externo.

Como tanto true y false son programas que siempre devuelven lo mismo (0 y 1 respectivamente) y no cambian el estado de la shell podrían ser programas externos, pero se implementan builtin por una cuestión de comodidad, principalmente para cuando se implementan scripts sh.

### Historial

---
