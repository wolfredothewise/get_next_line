# get_next_line

> A small, robust C function that returns one line at a time from a file
> descriptor.

<p align="center">
  <a href="#english">English</a> ·
  <a href="#español">Español</a>
</p>

## English

### Important note

This project was developed in **2025**. The 42 subject and toolchain may have
changed since then, so this repository may not match the current version
exactly. The core exercise and the implementation approach are still
essentially the same.

### About

`get_next_line` is a 42 School project focused on file descriptors, repeated
`read(2)` calls, dynamic memory, and static storage. The public function reads
only as much as necessary and returns the next complete line on each call.

The returned string:

- includes the trailing `\n` when the input line has one;
- does not add a newline when the file ends without one;
- returns `NULL` at end of file or when an error occurs.

This repository contains the mandatory part of the project. It keeps one
static buffer for the current input stream; the bonus multi-file-descriptor
variant is not included.

### Files

| File | Purpose |
| --- | --- |
| `get_next_line.c` | Public API, buffered reading, line extraction, and remaining-data management |
| `get_next_line_utils.c` | Small string and error-handling helpers |
| `get_next_line.h` | Public declaration, `BUFFER_SIZE`, and shared prototypes |
| `en.subject.pdf` | Assignment subject used for the project |

### Requirements

- A POSIX-compatible C compiler
- `unistd.h` and `stdlib.h`
- A valid file descriptor
- A positive `BUFFER_SIZE`

No Makefile is included, so the project can be compiled directly:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  get_next_line.c get_next_line_utils.c your_main.c \
  -o get_next_line_demo
```

You can choose another buffer size to test different read patterns:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 \
  get_next_line.c get_next_line_utils.c your_main.c \
  -o get_next_line_demo
```

### Minimal usage

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int		fd;
	char	*line;

	fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

### How it works

1. `read(2)` fills a temporary buffer of `BUFFER_SIZE` bytes.
2. Data is appended to the accumulated string until a newline is found or
   end-of-file is reached.
3. The first line is copied and returned to the caller.
4. Any bytes after that line are retained for the next call.
5. Allocated memory is released when the stream is exhausted or a read error
   occurs.

### Recommended checks

Before considering the implementation complete, test at least:

- an empty file;
- one line with and without a final newline;
- several lines in one read;
- lines longer and shorter than `BUFFER_SIZE`;
- `BUFFER_SIZE=1` and a large buffer;
- an invalid file descriptor;
- a read error and allocation failures under a diagnostic tool.

For a local memory check, compile with AddressSanitizer when supported:

```bash
cc -Wall -Wextra -Werror -g -fsanitize=address \
  -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c \
  your_main.c -o get_next_line_asan
./get_next_line_asan
```

### Red-team mini review

The most useful red-team exercise here is to treat every input boundary as
hostile, without trying to exploit a real service:

| Attack surface | Safe test | Expected result |
| --- | --- | --- |
| File descriptor | `-1`, a closed descriptor, or a descriptor opened for the wrong operation | Return `NULL` and do not leak the accumulated data |
| Input shape | Empty input, no final newline, very long lines, and repeated newlines | Return exactly the available lines, preserving newline behavior |
| Read size | `BUFFER_SIZE=1`, `42`, and a large value | Identical logical output for every valid size |
| Resource pressure | Run under AddressSanitizer and an allocation-failure test harness | No use-after-free, double-free, or unreleased stream state |

This is defensive testing only: use files and descriptors you own, and do not
test against systems without permission.

### Limitations

- The current implementation stores state for one active file descriptor.
- There is no Makefile or automated test suite in this repository.
- The project is educational code, not a production-ready replacement for a
  complete stream abstraction.



<details>
<summary id="español">Español</summary>

## Español

### Nota importante

Este proyecto se desarrolló en **2025**. El enunciado de 42 y la toolchain
pueden haber cambiado desde entonces, por lo que este repositorio puede no
coincidir exactamente con la versión actual. El ejercicio principal y el
enfoque de implementación siguen siendo prácticamente los mismos.

### Descripción

`get_next_line` es un proyecto de 42 School centrado en descriptores de
archivo, llamadas repetidas a `read(2)`, memoria dinámica y almacenamiento
estático. La función pública lee solo lo necesario y devuelve una línea nueva
en cada llamada.

La cadena devuelta:

- incluye el `\n` final cuando la línea de entrada lo contiene;
- no añade un salto de línea si el archivo termina sin él;
- devuelve `NULL` al llegar al final del archivo o cuando ocurre un error.

Este repositorio contiene la parte obligatoria. Mantiene un único buffer
estático para el flujo actual; no incluye la variante bonus para varios
descriptores simultáneos.

### Archivos

| Archivo | Función |
| --- | --- |
| `get_next_line.c` | API pública, lectura con buffer, extracción de líneas y gestión de datos restantes |
| `get_next_line_utils.c` | Funciones auxiliares de cadenas y errores |
| `get_next_line.h` | Declaración pública, `BUFFER_SIZE` y prototipos compartidos |
| `en.subject.pdf` | Enunciado utilizado para el proyecto |

### Requisitos y compilación

Se necesita un compilador C compatible con POSIX. Como no se incluye
Makefile, se puede compilar directamente:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  get_next_line.c get_next_line_utils.c your_main.c \
  -o get_next_line_demo
```

Para comprobar distintos patrones de lectura, cambia `BUFFER_SIZE`, por
ejemplo a `1` o a un valor grande.

### Uso mínimo

```c
int	fd;
char	*line;

fd = open("input.txt", O_RDONLY);
while ((line = get_next_line(fd)) != NULL)
{
	printf("%s", line);
	free(line);
}
close(fd);
```

### Funcionamiento

1. `read(2)` llena un buffer temporal de `BUFFER_SIZE` bytes.
2. Los datos se acumulan hasta encontrar `\n` o llegar al final.
3. Se devuelve la primera línea.
4. Los bytes posteriores se conservan para la siguiente llamada.
5. La memoria se libera al terminar el flujo o cuando falla una lectura.

### Mini revisión red team

Prueba los límites de forma defensiva: descriptores inválidos o cerrados,
archivos vacíos, líneas muy largas, archivos sin salto final, saltos
consecutivos, varios valores de `BUFFER_SIZE` y fallos de memoria. El
resultado lógico debe ser el mismo para cada tamaño válido y no debe haber
fugas, dobles liberaciones ni accesos a memoria inválidos.

Haz estas pruebas únicamente con archivos y descriptores propios o con
autorización explícita.

### Limitaciones

- La implementación actual mantiene el estado de un solo descriptor activo.
- No hay Makefile ni una suite de tests automatizada en este repositorio.
- Es código educativo y no sustituye una abstracción completa de streams en
  producción.

</details>
