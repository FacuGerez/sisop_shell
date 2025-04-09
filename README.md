# Comandos

- [ ] ejecutar binarios buscando ejecutable en path TAI
- [ ] redirecciones JULIAN
- [ ] pipes simples y múltiples FACU
- [ ] expansión de variables FACU
- [ ] variables de entorno temporarias LAUTARO
- [ ] psuedovariables LAUTARO
- [ ] comandos built in TAI

- [ ] cd
- [ ] exit
- [ ] pwd

- [ ] procesos en segundo plano JULIAN

# Tareas

    Soportar la ejecución de binarios
    Con búsqueda en $PATH
    Con y sin argumentos
    Responder: ¿cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?
    Responder: ¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?

En principio hago un fork -> busco $path + nombre del comando -> spliteo (si hay args) -> paso argumentos e invoco al ejecutable con argumentos.