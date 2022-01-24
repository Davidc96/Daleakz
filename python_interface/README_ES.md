# DaLeakz: CLI Software

En esta carpeta se encuentra el CLI para comunicarse con el Arduino para poder transferir archivos. El script se llama daleakz.py y está escrito en
Python3.

# Requisitos

- Python3
- PySerial
- pyfiglet

# Instalación

Para hacer funcionar el script, es necesario instalar Python3 y Pyserial con:
```
python3 -m pip install pyserial pyfiglet
```

Para ejecutar el script:
```
python3 -h daleakz.py
```

# Funcionamiento

```
 ____        _               _
|  _ \  __ _| |    ___  __ _| | __ _____
| | | |/ _` | |   / _ \/ _` | |/ /|_  (_)
| |_| | (_| | |__|  __/ (_| |   <  / / _
|____/ \__,_|_____\___|\__,_|_|\_\/___(_)

    _            _       _                           _   _ ____  ____
   / \   _ __ __| |_   _(_)_ __   ___     __ _ ___  | | | / ___|| __ )
  / _ \ | '__/ _` | | | | | '_ \ / _ \   / _` / __| | | | \___ \|  _ \
 / ___ \| | | (_| | |_| | | | | | (_) | | (_| \__ \ | |_| |___) | |_) |
/_/   \_\_|  \__,_|\__,_|_|_| |_|\___/   \__,_|___/  \___/|____/|____/


Created by @Davidc96

usage: daleakz.py [-h] [-f --file] read|write|list port

Send or receive files to Arduino using COM Port

positional arguments:
  read|write|list  action you would like to use
  port             Linux: /dev/USBX Windows: COMX

optional arguments:
  -h, --help       show this help message and exit
  -f --file        Name of the file (Not need it in list)
```

# Detalles técnicos

Debido a la limitación que tiene Arduino a la hora de crear variables, por culpa de la memoria, es necesario enviar y recibir el contenido
con multiples comandos de read o write. La máxima transferencia para que funcione es de 50 bytes por comando.

Para evitar cualquier problema con los NULL Bytes, lo que se hace es convertir un fichero a base64 para mostrar caracteres printables a cambio de que el tamaño del fichero aumente.

Otra dificultat que tiene Arduino es a la hora de crear los nombres de los ficheros donde estos no pueden contener según que longitud o que caracter especial por lo que se hace es generar un nombre aleatorio y luego se mapea el nombre original con el nuevo nombre. Ese mapeo se almacena en filedb.json

En la MicroSD, todos los ficheros se almacenarán en la raiz sin capacidad de crear carpetas ni nada.
